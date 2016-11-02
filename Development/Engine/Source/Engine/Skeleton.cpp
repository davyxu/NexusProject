#include "EnginePCH.h"
#include "NekoEngine_Model.h"


namespace NekoEngine
{
	SkeletonInstance::SkeletonInstance( SkeletonMeshResource* skeleton )
	: mInfluenceBone(null)
	, mInfluenceTranslation(Vector3::cZero)
	, mInfluenceRotation(Quaternion::cIdentity)
	{
		mRootArray = skeleton->mRootArray;

		mBoneArray.resize( skeleton->mBoneArray.size() );

		for ( dword i = 0;i< skeleton->mBoneArray.size();i++ )
		{
			BoneContent* LibBone = skeleton->mBoneArray[i];

			BoneContent* Bone = new BoneContent;
			*Bone = *LibBone;

			mBoneArray[i] = Bone;
		}

		Tick(0);

	}

	//void SkeletonInstance::Dump( )
	//{
	//	static int t = 0;

	//	TextFile file;
	//	file.Create( StringHelper::FormatString(L"t%d.txt", t++).c_str() );


	//	for ( dword i = 0;i< GetBoneCount();i++)
	//	{
	//		BoneContent* Bone = GetBone( i );
	//		file.WriteLine( L"\r\nBone %d %s", i, Bone->mName.c_str() );

	//		file.WriteLine( L"mRelativeTranslation %s", value_tostringW(  Bone->mRelativeTranslation ).c_str() );
	//		file.WriteLine( L"mRelativeRotation %s", value_tostringW(  Bone->mRelativeRotation ).c_str() );
	//		file.WriteLine( L"mAbsoluteTranslation %s", value_tostringW(  Bone->mAbsoluteTranslation ).c_str() );
	//		file.WriteLine( L"mAbsoluteRotation %s", value_tostringW(  Bone->mAbsoluteRotation ).c_str() );

	//		file.WriteLine( L"mRelativeTranslationInit %s", value_tostringW(  Bone->mRelativeTranslationInit ).c_str() );
	//		file.WriteLine( L"mRelativeRotationInit %s", value_tostringW(  Bone->mRelativeRotationInit ).c_str() );

	//		file.WriteLine( L"mLocalTranslationBoneSpace %s", value_tostringW(  Bone->mLocalTranslationBoneSpace ).c_str() );
	//		file.WriteLine( L"mLocalRotationBoneSpace %s", value_tostringW(  Bone->mLocalRotationBoneSpace ).c_str() );
	//		file.WriteLine( L"mLocalTranslation %s", value_tostringW(  Bone->mLocalTranslation ).c_str() );
	//		file.WriteLine( L"mLocalRotation %s", value_tostringW(  Bone->mLocalRotation ).c_str() );
	//	}
	//	
	//}

	SkeletonInstance::~SkeletonInstance( )
	{
		for ( dword i = 0; i < (dword)mBoneArray.size(); i ++ )
			delete mBoneArray[i];

		mBoneArray.clear();
	}


	void SkeletonInstance::UpdateBone( BoneContent* bone )
	{
		BoneContent* parentbone = GetBone(bone->mParentID);

		if ( parentbone == null )
		{
			bone->mAbsoluteTranslation	= bone->mRelativeTranslation;
			bone->mAbsoluteRotation		= bone->mRelativeRotation;
		}
		else
		{
			bone->mAbsoluteTranslation	 =  bone->mRelativeTranslation;
			bone->mAbsoluteTranslation	*= -parentbone->mAbsoluteRotation;
			bone->mAbsoluteTranslation	+=  parentbone->mAbsoluteTranslation;

			bone->mAbsoluteRotation		 = bone->mRelativeRotation;
			bone->mAbsoluteRotation		*= parentbone->mAbsoluteRotation;
		}

		if ( bone == mInfluenceBone )
		{
			bone->mAbsoluteTranslation	+= mInfluenceTranslation;
			bone->mAbsoluteRotation		*= mInfluenceRotation;
		}

		bone->mLocalTranslation	 =  bone->mLocalTranslationBoneSpace;
		bone->mLocalTranslation	*= -bone->mAbsoluteRotation;
		bone->mLocalTranslation	+=  bone->mAbsoluteTranslation;

		bone->mLocalRotation	 = bone->mLocalRotationBoneSpace;
		bone->mLocalRotation	*= bone->mAbsoluteRotation;

		for ( dword i = 0; i < (dword)bone->mChildArray.size(); i ++ )
		{
			BoneContent* child = GetBone( bone->mChildArray[i] );

			if ( child != null )
				UpdateBone( child );
		}
	}


	AABB SkeletonInstance::CaculateSkeletonBoundingBox(  )
	{
		AABB BoneBox = AABB::cInitBox;
		for ( dword i = 0; i < (dword)mRootArray.size(); i ++ )
		{
			BoneContent* Bone = GetBone( mRootArray.at( i ) );

			CaculateSkeletonBoundingBox( BoneBox, Bone );
		}

		return BoneBox;
	}

	void SkeletonInstance::CaculateSkeletonBoundingBox( AABB& BoneBox, BoneContent* Bone )
	{
		BoneBox += Bone->mAbsoluteTranslation;

		for ( dword i = 0; i < (dword)Bone->mChildArray.size(); i ++ )
		{
			BoneContent* Child = GetBone( Bone->mChildArray[i] );
			
			CaculateSkeletonBoundingBox( BoneBox, Child );
		}
	}


	void SkeletonInstance::Tick( float elapse )
	{
		for ( dword i = 0; i < mRootArray.size(); i ++ )
			UpdateBone( GetBone( mRootArray.at( i ) ) );
	}

	void SkeletonInstance::SetInfluence( BoneContent* bone, const Vector3& translation, const Quaternion& rotation )
	{
		mInfluenceBone			= bone;
		mInfluenceTranslation	= translation;
		mInfluenceRotation		= rotation;
	}

	void SkeletonInstance::ClearInfluence( )
	{
		mInfluenceBone			= null;
		mInfluenceTranslation	= Vector3::cZero;
		mInfluenceRotation		= Quaternion::cIdentity;
	}


	BoneContent* SkeletonInstance::GetBone( dword boneid )
	{
		if ( boneid < mBoneArray.size())
			return mBoneArray.at( boneid );

		return null;
	}

	BoneContent* SkeletonInstance::GetBone( const wchar* bonename )
	{
		for ( dword i = 0; i < mBoneArray.size(); i ++ )
		{
			BoneContent* bone = mBoneArray.at( i );

			if ( StringHelper::CompareLowerCase( bone->mName.c_str(), bonename ) )
				return bone;
		}

		return null;
	}

	NekoEngine::dword SkeletonInstance::GetBoneCount()
	{
		return (dword)mBoneArray.size();
	}
}