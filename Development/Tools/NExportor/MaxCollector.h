#pragma once

//----------------------------------------------------------------------------
// MaxInterface
//----------------------------------------------------------------------------


class MaxCollector
{
public:	
	class MaxModel*				mMaxModel;
	class MaxSkeleton*			mMaxSkeleton;
	class MaxSkeletonAnimation*	mMaxSkeletonAnimation;
	class MaxMaterial*			mMaxMaterial;

	bool mExportSelected;

	MaxCollector( );

	virtual ~MaxCollector( );

	void CollectData( bool ExportSelected );
	void Clear( );

	
	static void StaticInit( );
	static void StaticExit( );

private:
	void ParseNode( INode* node );
};

inline NekoEngine::Vector3 Conv( const Point3& v )
{
	return NekoEngine::Vector3( v.x, v.y, v.z );
}

inline NekoEngine::Quaternion Conv( const Quat& v )
{
	return NekoEngine::Quaternion( v.x, v.y, v.z, v.w );
}

extern MaxCollector* GMaxCollector;