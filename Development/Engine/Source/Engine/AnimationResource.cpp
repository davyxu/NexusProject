#include "EnginePCH.h"
#include "NekoEngine_Model.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	//RawAnimation
	//////////////////////////////////////////////////////////////////////////

	


	SkeletonAnimTrack::~SkeletonAnimTrack()
	{
		for ( dword i = 0; i< mKeyframeArray.size();i++)
		{
			delete mKeyframeArray[i];
		}

		mKeyframeArray.clear();
	}

	IMPLEMENT_RTTI_CLASS( SkeletonAnimResource)


	SkeletonAnimResource::SkeletonAnimResource( )
	: mDuration( 0.0f )
	{

	}

	SkeletonAnimResource::~SkeletonAnimResource()
	{
		for ( dword i = 0; i< mTrackArray.size();i++)
		{
			delete mTrackArray[i];
		}

		mTrackArray.clear();
	}


	NekoEngine::dword SkeletonAnimResource::AddTrack( SkeletonAnimTrack* Track )
	{
		mTrackArray.push_back( Track );

		return mTrackArray.size() - 1;
	}

	void SkeletonAnimResource::GetKeyframe( SkeletonAnimTrack* track, float time, SkeletonAnimKeyframe& frame1, SkeletonAnimKeyframe& frame2 )
	{
		int i = 0, j = track->mKeyframeArray.size() - 1;

		while ( i < j - 1 )
		{
			int m = ( i + j ) / 2;

			if ( time >= track->mKeyframeArray[ m ]->mTime )
				i = m;
			else
				j = m;
		}

		frame1 = *track->mKeyframeArray[i];
		frame2 = *track->mKeyframeArray[j];
	}

	void SkeletonAnimResource::SetKeyframe( SkeletonAnimTrack* track, float time, Vector3& translation, Quaternion& rotation )
	{
		SkeletonAnimKeyframe frame1, frame2;
		GetKeyframe( track, time, frame1, frame2 );

		if ( frame1.mTime == frame2.mTime )
		{
			translation	= frame1.mTranslation;
			rotation	= frame1.mRotation;
		}
		else
		{
			float factor = ( time - frame1.mTime ) / ( frame2.mTime - frame1.mTime );

			translation	= Vector3::Lerp( frame1.mTranslation, frame2.mTranslation, factor );
			rotation	= Quaternion::Slerp( frame1.mRotation, frame2.mRotation, factor );
		}
	}


	void SkeletonAnimResource::Serialize( BinarySerializer& Ser )
	{
		Ser << mDuration
			<< mTrackArray;
	}


}