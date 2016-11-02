#include "ExportPCH.h"
#include "PluginInclude.h"
//////////////////////////////////////////////////////////////////////////
//MaxBone
//////////////////////////////////////////////////////////////////////////

MaxBone::MaxBone( MaxSkeleton* skeleton, INode* node, int parentid )
: mSkeleton( skeleton )
, mNode( node )
, mParentID( parentid )
, mTrack( null )
{
}

void MaxBone::AddChild( int childid )
{
	mChildArray.push_back( childid );
}

char* MaxBone::GetName( )
{
	return mNode->GetName( );
}



void MaxBone::Serialize(NekoEngine::BoneContent* Bone )
{
	Matrix3	relativetransform	= MaxNode( mNode ).GetRelativeTransform( 0, true );
	Point3	relativetranslation	= relativetransform.GetTrans( );
	Quat	relativerotation	= Quat( relativetransform );

	Matrix3	localtransform		= Inverse( MaxNode( mNode ).GetTransform( 0, true ) );
	Point3	localtranslation	= localtransform.GetTrans( );
	Quat	localrotation		= Quat( localtransform );

	relativetranslation.x	= - relativetranslation.x;
	relativerotation.x		= - relativerotation.x;
	relativerotation.w		= - relativerotation.w;
	localtranslation.x		= - localtranslation.x;
	localrotation.x			= - localrotation.x;
	localrotation.w			= - localrotation.w;

	Bone->mParentID = mParentID;
	Bone->mName = NekoEngine::StringConverter().AnsiToUnicode( GetName() );
	Bone->mRelativeTranslation			= Conv( relativetranslation );
	Bone->mRelativeRotation				= Conv( relativerotation );
	Bone->mLocalTranslationBoneSpace	= Conv( localtranslation );
	Bone->mLocalRotationBoneSpace		= Conv( localrotation );
}

Matrix3 GetNodeTransform( INode* node, TimeValue t )
{
	Matrix3 transform = node->GetNodeTM( t );
	// make transformation uniform
	transform.NoScale();
	return transform;
}

Matrix3 MaxBone::GetRelativeTransform( TimeValue t )
{
	if ( mNode->GetParentNode() == null )
		return GetNodeTransform( mNode, t );

	return GetNodeTransform( mNode, t ) * ::Inverse( GetNodeTransform( mNode->GetParentNode( ), t ) );
}



//----------------------------------------------------------------------------
// MaxSkeleton
//----------------------------------------------------------------------------

MaxSkeleton::MaxSkeleton(  )
{

}

MaxSkeleton::~MaxSkeleton( )
{
	for ( int k = 0; k < mBoneArray.size( ); k ++ )
		delete mBoneArray[k];

	mBoneArray.clear();
}

void MaxSkeleton::AddBone( INode* node, int parentid )
{
	int boneid = (int) mBoneArray.size( );

	MaxBone* bone = new MaxBone( this, node, parentid );
	mBoneArray.push_back( bone );

	for ( int i = 0; i < node->NumberOfChildren( ); i ++ )
	{
		if ( MaxNode( node->GetChildNode( i ) ).IsBone( ) == false )
			continue;

		bone->AddChild( (int) mBoneArray.size( ) );

		AddBone( node->GetChildNode( i ), boneid );
	}
}

int MaxSkeleton::GetBoneID( const char* bonename )
{
	for ( int i = 0; i < mBoneArray.size( ); i ++ )
	{
		if ( ::stricmp( mBoneArray[i]->GetName( ), bonename ) == 0 )
			return i;
	}

	return -1;
}

char* MaxSkeleton::GetBoneName( int boneid )
{
	return mBoneArray[ boneid ]->GetName( );
}



void MaxSkeleton::Serialize( NekoEngine::SkeletonMeshResource* Skin )
{
	if ( GetBoneNumber() == 0 )
		return;

	
	for ( DWORD i = 0; i < mBoneArray.size();i++)
	{
		MaxBone* MBone = mBoneArray[ i ];
		NekoEngine::BoneContent* Bone = new NekoEngine::BoneContent;
		MBone->Serialize( Bone );
		Skin->AddBone( Bone );
	}	
}

