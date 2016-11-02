#include "EnginePCH.h"
#include "NekoEngine_Model.h"

namespace NekoEngine
{
	AnimationMixer::AnimationMixer( ):
	mPause(true),
	mLast(0.0f),
	mCurrent(0.0f),
	mSpeed(1.0f),
	mDeltaTransform(Matrix4::cIdentity)
	{
	}

	AnimationMixer::~AnimationMixer()
	{
		Clear();
	}

	void AnimationMixer::AddAnimation( AnimationInstance* animation, float current, float speed, float weight, float duration, float blend )
	{
		float starttime = 0.0f;

		if ( mAnimationArray.size( ) > 0 )
			starttime = mAnimationArray[ mAnimationArray.size() - 1 ]->mEndTime;

		if ( starttime >= blend )
			starttime -= blend;

		duration /= speed;

		AnimationDesc* animationdesc = new AnimationDesc;
		animationdesc->mCurrent		= current;
		animationdesc->mAnimation	= animation;
		animationdesc->mSpeed		= speed;
		animationdesc->mWeight		= weight;
		animationdesc->mStartTime	= starttime;
		animationdesc->mEndTime		= starttime + duration;
		animationdesc->mBlend		= blend;

		mAnimationArray.push_back( animationdesc );
	}

	void AnimationMixer::DelAnimation( AnimationInstance* animation )
	{
		for ( dword i = 0; i < mAnimationArray.size(); i ++ )
		{
			if ( mAnimationArray[i]->mAnimation == animation )
			{
				mAnimationArray.erase( mAnimationArray.begin() + i );

				return;
			}
		}
	}

	void AnimationMixer::SetSpeed( float speed )
	{
		mSpeed = speed;
	}

	void AnimationMixer::Update( float elapse )
	{
		mDeltaTransform = Matrix4::cIdentity;

		if ( mAnimationArray.size( ) == 0 )
		{
			mLast		= 0.0f;
			mCurrent	= 0.0f;

			return;
		}

		// Elapse 0 ms, no need to update animations.
		if ( elapse == 0.0f )
			return;

		// Update sequence current time.
		float deltatime = mSpeed * elapse / 1000.0f;

		mLast		= mCurrent;

		if (!mPause)
			mCurrent	= mCurrent + deltatime;
		else
		{
			deltatime = 0.0f;
		}

		// Process all animations in sequence.
		for ( dword i = 0; i < mAnimationArray.size( ); i ++ )
		{
			AnimationDesc* animationdesc = mAnimationArray[i];

			// The animation is not ready to start yet, skip it.
			if ( mCurrent < animationdesc->mStartTime )
				continue;

			// The animation has reached its end, skip it.
			if ( mCurrent > animationdesc->mEndTime )
			{
				if (i == mAnimationArray.size() -1)
				{
					mLast		= 0.0f;
					mCurrent	= 0.0f;
				}

				continue;
			}

			AnimationInstance* animation = (AnimationInstance*)animationdesc->mAnimation;

			// Initialize delta transform of animation, when it ready to start.
			if ( animation->mAffectTransform == true )
			{
				if ( mLast <= animationdesc->mStartTime && mCurrent >= animationdesc->mStartTime )
					animation->InitDeltaTransform( );
			}

			float duration	= animation->GetDuration( );
			float last		= animationdesc->mCurrent;
			float current	= animationdesc->mCurrent + deltatime * animationdesc->mSpeed;
			float weight	= animationdesc->mWeight;

			// Animation last time exceed its duration, so loop it.
			if ( ( animationdesc->mEndTime - animationdesc->mStartTime ) * animationdesc->mSpeed > duration + Math::cEpsilon )
			{
				if ( current >= duration )
					current = Math::Fmod( current, duration );
			}
			// None loop.
			else
			{
				// 和动画本身循环没有关系
				if ( current > duration )
				{
					current = Math::Fmod( current, duration );
				}
			}

			// Calculate animation weight by blend time.
			if ( animationdesc->mBlend > 0.0f )
			{
				float time = mCurrent - animationdesc->mStartTime;

				if ( time >= 0.0f && time < animationdesc->mBlend )
					weight *= time / animationdesc->mBlend;
			}

			if (mPause)
				animation->Update(mCurrent - animationdesc->mStartTime,mCurrent - animationdesc->mStartTime,weight);
			else
				animation->Update( last, current, weight );

			// Update animation current time.
			animationdesc->mCurrent = current;



			// Update delta transform of animation.

			if ( weight > 0.5f )
				mDeltaTransform = animation->GetDeltaTransform( );
		}
	}

	void AnimationMixer::Reset( )
	{
		mLast		= 0.0f;
		mCurrent	= 0.0f;

		for ( dword i = 0; i < mAnimationArray.size(); i ++ )
			mAnimationArray[i]->mCurrent = 0.0f;
	}

	void AnimationMixer::Clear( )
	{
		mLast		= 0.0f;
		mCurrent	= 0.0f;

		for ( dword i = 0; i < mAnimationArray.size(); i ++ )
		{
			AnimationDesc* desc = mAnimationArray[ i ];
			delete desc;
		}

		mAnimationArray.clear( );
	}

	dword AnimationMixer::GetAnimationCount( )
	{
		return (dword)mAnimationArray.size();
	}

	AnimationDesc* AnimationMixer::GetAnimationDesc( int index )
	{
		return mAnimationArray[ index ];
	}

	AnimationDesc* AnimationMixer::GetAnimationDesc( AnimationInstance* animation )
	{
		for ( dword i = 0; i < mAnimationArray.size(); i ++ )
		{
			if ( mAnimationArray[i]->mAnimation == animation )
				return mAnimationArray[i];
		}

		return null;
	}

	void AnimationMixer::GetCurrentAnimationDesc( AnimationDesc& animationdesc )
	{
		// Process all animations in sequence.
		for ( dword i = 0; i < mAnimationArray.size(); i ++ )
		{
			const AnimationDesc* desc = mAnimationArray[i];

			// The animation is not ready to start yet, skip it.
			if ( mCurrent < desc->mStartTime )
				continue;

			// The animation has reached its end, skip it.
			if ( mCurrent > desc->mEndTime )
				continue;

			// Found it.
			animationdesc = *desc;

			break;
		}
	}

	float AnimationMixer::GetDuration( )
	{
		float duration = 0.0f;
		for ( dword i = 0; i < mAnimationArray.size(); i ++ )
		{
			const AnimationDesc* desc = mAnimationArray[i];
			duration += desc->mAnimation->GetDuration();
		}

		return duration;
	}

	void AnimationMixer::SetCurrentAnimationDuration( float duration )
	{
		// Process all animations in sequence.
		for ( dword i = 0; i < mAnimationArray.size(); i ++ )
		{
			const AnimationDesc* desc = mAnimationArray[i];

			// The animation is not ready to start yet, skip it.
			if ( mCurrent < desc->mStartTime )
				continue;

			// The animation has reached its end, skip it.
			if ( mCurrent > desc->mEndTime )
				continue;

			// Found it, calculate the delta duration.
			float delta = desc->mStartTime + duration - desc->mEndTime;

			// Adjust the end time of current animation.
			mAnimationArray[i]->mEndTime = desc->mStartTime + duration;

			// Adjust the end time of all subsequence animations.
			for ( dword j = i + 1; j < mAnimationArray.size(); j ++ )
			{
				mAnimationArray[j]->mStartTime	+= delta;
				mAnimationArray[j]->mEndTime	+= delta;
			}

			break;
		}
	}

	const Matrix4& AnimationMixer::GetDeltaTransform( )
	{
		return mDeltaTransform;
	}
}