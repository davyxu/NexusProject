#pragma once


//----------------------------------------------------------------------------
// MaxSkeletonAnimation
//----------------------------------------------------------------------------

struct SkeletonAnimationExportOption 
{
	bool	mUseSampling;

	float	mSamplingRate;

	SkeletonAnimationExportOption( )
		: mUseSampling( true )
		, mSamplingRate( 0.1f )
	{

	}
};

class MaxSkeletonAnimation 
{
public:
	MaxSkeleton*	mSkeleton;

	NekoEngine::SkeletonAnimResource	mSkeletonAnimationResource;

	MaxSkeletonAnimation( MaxSkeleton* skeleton );
	~MaxSkeletonAnimation( );

	bool Save( const wchar_t* FileName );

	void BuildTrackBySampling( float SamplingRate);

	void BuildTrackByKeyFrame( );

private:
	bool IsKeyframe( INode* node, int tick );


	
};