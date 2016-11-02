#include "EnginePCH.h"
#include "NekoEngine_Model.h"

namespace NekoEngine
{
	SkeletonAnimController::SkeletonAnimController(SkeletonAnimResource* AnimResource, SkeletonInstance* Skeleton)
	{
		mAnimationLoader	= AnimResource;
		mSkeleton			= Skeleton;

		mCurrent			= 0.0f;
		mSpeed				= 1.0f;
		mCircle				= true;
		mAffectTransform	= false;
		mFixRoot			= false;

		mLastTranslation	= Vector3::cZero;
		mLastRotation		= Quaternion::cIdentity;
		mAccumTranslation	= Vector3::cZero;
		mAccumRotation		= Quaternion::cIdentity;
		mDeltaTranslation	= Vector3::cZero;
		mDeltaRotation		= Quaternion::cIdentity;
	}


	void SkeletonAnimController::Update(float ElapsedTime )
	{
		if (mAnimationLoader == null || mSkeleton == null)
			return;

		float last = mCurrent;

		float duration = GetDuration();

		if ( duration > 0.0f )
		{
			mCurrent += ElapsedTime * 0.001f * mSpeed;

			if ( mCircle == true )
			{
				if ( mCurrent >= duration )
					mCurrent = Math::Fmod( mCurrent, duration );
			}
			else
			{
				if ( mCurrent > duration )
					mCurrent = duration;
			}
		}

		Update(last,mCurrent,1.0f);
	}


	void SkeletonAnimController::Update(float last,float current,float weight)
	{
		if (!mAnimationLoader || !mSkeleton)
			return;

		for ( dword i = 0; i < mAnimationLoader->mTrackArray.size(); i ++ )
		{
			SkeletonAnimTrack* track = mAnimationLoader->mTrackArray[i];

			BoneContent* bone = mSkeleton->GetBone( track->mBoneID );

			if ( bone == null )
				continue;

			BoneContent* parnetbone = mSkeleton->GetBone( bone->mParentID );

			// Skip other root bones.
			if ( parnetbone == null && track->mBoneID != 0 )
				continue;

			Vector3 currenttranslation; Quaternion currentrotation;
			mAnimationLoader->SetKeyframe( track, current, currenttranslation, currentrotation );

			// Not affect root transform, just update bone's transform.
			if ( parnetbone != null || ( mAffectTransform == false && mFixRoot == false ) )
			{
				// Blend to previous animation with weight.
				currenttranslation	= Vector3::Lerp( bone->mRelativeTranslation, currenttranslation, weight );
				currentrotation		= Quaternion::Slerp( bone->mRelativeRotation, currentrotation, weight );

				bone->mRelativeTranslation	= currenttranslation;
				bone->mRelativeRotation		= currentrotation;
			}
			// Affect root transform, not update bone's transform, but update skeleton's transform.
			else if ( mAffectTransform == true )
			{
				if ( current == GetDuration() || current < last )
				{
					BoneContent* root0 = mSkeleton->GetBone( mSkeleton->mRootArray.at( 0 ) );
					Vector3 translation1 = mAnimationLoader->mTrackArray[0]->mKeyframeArray[0]->mTranslation;
					Quaternion rotation1 = mAnimationLoader->mTrackArray[0]->mKeyframeArray[0]->mRotation;

					Vector3 translation2 = root0->mRelativeTranslation;
					Quaternion rotation2 = root0->mRelativeRotation;

					mAccumRotation		= rotation1 / rotation2;
				}

				if ( current >= last )
				{
					mDeltaTranslation	= currenttranslation - mLastTranslation;
					mDeltaRotation		= currentrotation / mLastRotation;
				}
				else
				{
					int k0 = 0, k1 = track->mKeyframeArray.size( ) - 1;

					Vector3 translation0 = track->mKeyframeArray[k0]->mTranslation;
					Quaternion rotation0 = track->mKeyframeArray[k0]->mRotation;

					Vector3 translation1 = track->mKeyframeArray[k1]->mTranslation;
					Quaternion rotation1 = track->mKeyframeArray[k1]->mRotation;

					mDeltaTranslation = ( currenttranslation - translation0 ) + ( translation1 - mLastTranslation );
					mDeltaRotation	  = ( currentrotation / rotation0 ) * ( rotation1 / mLastRotation );
				}

				mLastTranslation = currenttranslation;
				mLastRotation	 = currentrotation;
			}
		}
	}


	float SkeletonAnimController::GetDuration( )
	{
		if (mAnimationLoader == null)
			return 0.0f;

		return mAnimationLoader->mDuration;
	}

	float SkeletonAnimController::GetCurrent( )
	{
		return mCurrent;
	}

	void SkeletonAnimController::SetCurrent( float current, bool autoupdate )
	{
		mCurrent = current;

		float duration = GetDuration();

		if ( mCircle == true )
		{
			if ( mCurrent > duration )
				mCurrent = Math::Fmod( mCurrent, duration );
		}
		else
		{
			if ( mCurrent > duration )
				mCurrent = duration;
		}

		if ( autoupdate == true )
			Update( 0 );
	}

	float SkeletonAnimController::GetSpeed( )
	{
		return mSpeed;
	}

	void SkeletonAnimController::SetSpeed( float speed )
	{
		mSpeed = speed;
	}

	bool SkeletonAnimController::GetLoop( )
	{
		return mCircle;
	}

	void SkeletonAnimController::SetLoop( bool circle )
	{
		mCircle = circle;
	}

	void SkeletonAnimController::SetBlendOption( bool affecttransform, bool fixroot )
	{
		mAffectTransform	= affecttransform;
		mFixRoot			= fixroot;
	}


	void SkeletonAnimController::InitDeltaTransform( )
	{
		if (mAnimationLoader == null)
			return;

		BoneContent* root0 = mSkeleton->GetBone( mSkeleton->mRootArray.at( 0 ) );

		Vector3 translation1		= mAnimationLoader->mTrackArray[0]->mKeyframeArray[0]->mTranslation;
		Quaternion rotation1		= mAnimationLoader->mTrackArray[0]->mKeyframeArray[0]->mRotation;

		Vector3 translation2		= root0->mRelativeTranslation;
		Quaternion rotation2		= root0->mRelativeRotation;

		mAccumTranslation	= translation1;
		mAccumRotation		= Quaternion::cIdentity;
		mLastTranslation	= translation2;
		mLastRotation		= rotation2;
	}

	Matrix4 SkeletonAnimController::GetDeltaTransform( )
	{
		Matrix4 transform;
		transform  = Matrix4( ).Translation( -mAccumTranslation );
		transform *= Matrix4( ).Rotation( -( mAccumRotation * mDeltaRotation ).Normalize( ) );
		transform *= Matrix4( ).Translation( mDeltaTranslation );
		transform *= Matrix4( ).Rotation( mAccumRotation.Normalize( ) );
		transform *= Matrix4( ).Translation( mAccumTranslation );

		mAccumRotation		*= mDeltaRotation;
		mDeltaTranslation	 = Vector3::cZero;
		mDeltaRotation		 = Quaternion::cIdentity;

		return transform;
	}

}