#include "ExportPCH.h"
#include "PluginInclude.h"

//----------------------------------------------------------------------------
// MaxSkeletonAnimation
//----------------------------------------------------------------------------

MaxSkeletonAnimation::MaxSkeletonAnimation(MaxSkeleton* skeleton )
: mSkeleton( skeleton )
{
}

MaxSkeletonAnimation::~MaxSkeletonAnimation( )
{

}

bool MaxSkeletonAnimation::IsKeyframe( INode* node, int tick )
{
	Control* control	= node->GetTMController( );
	Control* pcontrol	= control->GetPositionController( );
	Control* rcontrol	= control->GetRotationController( );
	Control* scontrol	= control->GetScaleController( );

	for ( int i = 0; i < control->NumSubs( ); i ++ )
	{
		if ( control->SubAnim( i )->IsKeyAtTime( tick, KEYAT_POSITION | KEYAT_ROTATION | KEYAT_SCALE ) )
			return true;
	}

	if ( pcontrol != null && pcontrol->IsKeyAtTime( tick, KEYAT_POSITION ) )
		return true;

	if ( rcontrol != null && rcontrol->IsKeyAtTime( tick, KEYAT_ROTATION ) )
		return true;

	if ( scontrol != null && scontrol->IsKeyAtTime( tick, KEYAT_SCALE ) )
		return true;

	if ( MaxNode( node ).IsRoot( ) )
		return false;

	return IsKeyframe( node->GetParentNode( ), tick );
}

void MaxSkeletonAnimation::BuildTrackByKeyFrame( )
{
	TimeValue start	= GMaxInterface->GetAnimRange( ).Start( );
	TimeValue end	= GMaxInterface->GetAnimRange( ).End( );
	int tickperframe = ::GetTicksPerFrame( );
	int fps = ::GetFrameRate();
	DWORD minframe = start / tickperframe;
	DWORD maxframe = end / tickperframe;
	mSkeletonAnimationResource.SetDuration( (float) ( maxframe - minframe ) / (float) fps ) ;

	for ( int b = 0; b < mSkeleton->mBoneArray.size( ); b ++ )
	{
		MaxBone* bone = mSkeleton->mBoneArray[b];

		NekoEngine::SkeletonAnimTrack* Track = new NekoEngine::SkeletonAnimTrack;
		Track->mBoneID = b;

		for ( int tick = start ; tick <= end ; tick ++ ) //GetTicksPerFrame( ) )
		{
			if ( IsKeyframe( bone->mNode, tick ) == false )
				continue;

			Matrix3	Transform = MaxNode( bone->mNode ).GetRelativeTransform( tick, true );

			NekoEngine::SkeletonAnimKeyframe* Frame = new NekoEngine::SkeletonAnimKeyframe;
			Frame->mTime			= ( (float) tick / (float) tickperframe - minframe ) / (float)fps;
			Frame->mTranslation		= Conv( Transform.GetTrans( ) );
			Frame->mRotation		= Conv( Quat( Transform ) );

			Frame->mTranslation.x	= - Frame->mTranslation.x;
			Frame->mRotation.x		= - Frame->mRotation.x;
			Frame->mRotation.w		= - Frame->mRotation.w;

			Track->mKeyframeArray.push_back( Frame );
		}

		if ( (int) Track->mKeyframeArray.size( ) == 0 )
			delete Track;
		else
			mSkeletonAnimationResource.AddTrack( Track );
	}

}

bool MaxSkeletonAnimation::Save( const wchar_t* FileName )
{
	using namespace NekoEngine;

	NekoEngine::FileStream Stream( FileName ,FAM_Write );
	BinarySerializer Ser( &Stream, false );
	Ser.SerializerFileHeader( FILE_VER_ANIMATION );
	Ser << mSkeletonAnimationResource;

	return true;
}




void MaxSkeletonAnimation::BuildTrackBySampling( float SamplingRate )
{
	TimeValue start	= GMaxInterface->GetAnimRange( ).Start( );
	TimeValue end	= GMaxInterface->GetAnimRange( ).End( );
	TimeValue step		= ::GetTicksPerFrame();
	TimeValue displacement = 0 * ::GetTicksPerFrame();
	float fps = GetFrameRate();
	DWORD minframe = start / ::GetTicksPerFrame( );
	DWORD maxframe = end / ::GetTicksPerFrame( );
	mSkeletonAnimationResource.SetDuration( (float) ( maxframe - minframe ) / (float) fps ) ;


	const float AnimationSampleFreq = SamplingRate;
	float LastSamplePoint = -AnimationSampleFreq - 0.1;

	for ( TimeValue tick = start; tick <= end + step; tick += step) 
	{

		TimeValue LerpTime;
		LerpTime = tick + displacement;
		LerpTime = ((LerpTime - start) % (end - start + step)) + start;

		
		float animtime = (tick - start)/ (step * fps );

		if ( animtime < LastSamplePoint + AnimationSampleFreq )
		{
			continue;
		}

		LastSamplePoint = animtime;

		for ( int BoneID = 0; BoneID < mSkeleton->mBoneArray.size( ); BoneID ++ )
		{
			MaxBone* bone = mSkeleton->mBoneArray[BoneID];

			if ( bone->mTrack == null )
			{
				bone->mTrack = new NekoEngine::SkeletonAnimTrack;
				bone->mTrack->mBoneID = BoneID;
				mSkeletonAnimationResource.AddTrack( bone->mTrack );
			}

			Matrix3 transform = bone->GetRelativeTransform( LerpTime );

			NekoEngine::SkeletonAnimKeyframe* Frame = new NekoEngine::SkeletonAnimKeyframe;

			Frame->mTime			= animtime;
			Frame->mTranslation		= Conv(transform.GetTrans( ));
			Frame->mRotation		= Conv( Quat( transform ) );

			Frame->mTranslation.x	= - Frame->mTranslation.x;
			Frame->mRotation.x		= - Frame->mRotation.x;
			Frame->mRotation.w		= - Frame->mRotation.w;

			bone->mTrack->mKeyframeArray.push_back( Frame );
			
		}
	}

}

