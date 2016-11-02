#pragma once

//----------------------------------------------------------------------------
// MaxSkeleton
//----------------------------------------------------------------------------

class MaxSkeleton;
struct SkeletonAnimTrack;

class MaxBone
{
public:
	typedef vector< int > ChildArray;

	MaxSkeleton*	mSkeleton;
	INode*			mNode;
	int				mParentID;
	ChildArray		mChildArray;
	NekoEngine::SkeletonAnimTrack*			mTrack;

	MaxBone( MaxSkeleton* skeleton, INode* node, int parentid );

	void AddChild( int childid );

	virtual char*	GetName( );

	void Serialize(NekoEngine::BoneContent* Bone );

	Matrix3 GetRelativeTransform( TimeValue t );
};

typedef vector< MaxBone* > BoneArray;

class MaxSkeleton : public MaxObject
{
public:
	MaxSkeleton( );

	virtual ~MaxSkeleton( );

	void Serialize( NekoEngine::SkeletonMeshResource* Skin );

	int GetBoneNumber( ){ return mBoneArray.size();}

private:
	friend class MaxMesh;
	friend class MaxCollector;
	void	AddBone( INode* node, int parentid );
	int		GetBoneID( const char* bonename );
	char*	GetBoneName( int boneid );
	

private:
	friend class MaxSkeletonAnimation;
	friend class MaxModel;
	BoneArray						mBoneArray;
};

