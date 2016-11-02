#include "EnginePCH.h"
#include "NekoEngine_Model.h"

namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS( SkeletonMeshVertexFactory)
	IMPLEMENT_RTTI_CLASS( SkeletonMeshResource)

	#define USE_SHARE_BONE_ARRAY

	SkeletonMeshResource::SkeletonMeshResource()
	{

	}

	SkeletonMeshResource::~SkeletonMeshResource()
	{
		// 释放骨骼
		for ( dword i = 0; i < (dword)mBoneArray.size(); i ++ )
			delete mBoneArray[i];

		mRootArray.clear( );
		mBoneArray.clear( );

	}

	void SkeletonMeshResource::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		Ser << mBoneArray;

		if ( Ser.IsLoading() )
		{
			// 构建层次关系
			for ( dword BoneID = 0;BoneID < mBoneArray.size() ;BoneID++ )
			{
				BoneContent* ParentBone = GetBone( mBoneArray[BoneID]->mParentID );
				if ( ParentBone != null )
					ParentBone->mChildArray.push_back( BoneID );
				else
					mRootArray.push_back( BoneID );
			}

#ifdef USE_SHARE_BONE_ARRAY
			BuildBoneIndex();
#endif
		}
	}

	dword SkeletonMeshResource::AddBone( BoneContent* Bone )
	{
		mBoneArray.push_back( Bone );

		return mBoneArray.size() - 1;
	}

	BoneContent* SkeletonMeshResource::GetBone( dword boneid )
	{
		if ( boneid < mBoneArray.size())
			return mBoneArray[ boneid ];

		return  null;
	}


	void SkeletonMeshResource::BuildBoneIndex( )
	{
		dword BlendIndicesOffset = mVertexFormat.GetElementOffset( VU_Blend_Indices );
		dword TotalStride = mVertexFormat.GetTotalStride( 0 );

		// 静态模型或者没有保护BlendIndices信息
		if ( BlendIndicesOffset == -1 )
			return;

		for ( dword FactoryIndex = 0; FactoryIndex < GetVertexFactoryCount(); FactoryIndex++)
		{
			// 每个子模型
			SkeletonMeshVertexFactory* Factory = GetVertexFactory<SkeletonMeshVertexFactory>( FactoryIndex );

			for ( dword MeshVertexIndex = 0; MeshVertexIndex< Factory->GetVertexCount(); MeshVertexIndex++ )
			{
				// 每个顶点的数据
				_byte* VertexData = (_byte*)Factory->GetVertexStream()->GetBuffer() + MeshVertexIndex * TotalStride;


				for ( dword InfluenceIndex = 0; InfluenceIndex < 4; InfluenceIndex ++ )
				{
					// 每个顶点的4个权重
					float* VertexBlendIndexPtr = (float*)( VertexData + BlendIndicesOffset) + InfluenceIndex;

					dword VertexBlendIndex = dword( *VertexBlendIndexPtr );

					// 没影响的骨骼不处理
					if ( VertexBlendIndex == -1 )
						continue;

					// 将原来的VB骨骼索引改为相对每个子模型
					// 在骨骼索引中查找

				
					BoneIndexArray::iterator ExistBoneRef = std::find( Factory->mBoneIndices.begin(), Factory->mBoneIndices.end(), VertexBlendIndex );
					if ( ExistBoneRef == Factory->mBoneIndices.end() )
					{
						*VertexBlendIndexPtr = (float)Factory->mBoneIndices.size();

						Factory->mBoneIndices.push_back( VertexBlendIndex );
					}
					else
					{
						// 已经存在的索引
						*VertexBlendIndexPtr = (float)(ExistBoneRef - Factory->mBoneIndices.begin() );
					}
				}
			}
		}
	}


	BoneShaderData* SkeletonMeshResource::GenBoneIndexMatrix( BoneShaderBuffer& DataArray, SkeletonInstance* Skeleton, dword MeshContentIndex)
	{
		Assert( IsInGameThread() );


		if ( DataArray.size() == 0   )
			DataArray.resize( GetVertexFactoryCount() );

		BoneShaderData& BoneData = DataArray[ MeshContentIndex ];

		SkeletonMeshVertexFactory* Factory = GetVertexFactory<SkeletonMeshVertexFactory>( MeshContentIndex );

#ifdef USE_SHARE_BONE_ARRAY
		// 扩展共享骨骼空间
		if ( BoneData.mBoneLocalMatrixArray.size() == 0 && Factory->mBoneIndices.size() )
			BoneData.mBoneLocalMatrixArray.resize( Factory->mBoneIndices.size() );

		// 共享骨骼矩阵
		for ( BoneData.mSharedBoneCount = 0;BoneData.mSharedBoneCount < Factory->mBoneIndices.size(); BoneData.mSharedBoneCount ++ )
		{
			BoneContent* Bone = Skeleton->GetBone( Factory->mBoneIndices[BoneData.mSharedBoneCount] );
			BoneData.mBoneLocalMatrixArray[BoneData.mSharedBoneCount] = BoneData.mBoneLocalMatrixArray[BoneData.mSharedBoneCount].FromTranslationRotation( Bone->mLocalTranslation, -Bone->mLocalRotation ).Transpose();
		}
#else
		BoneData.mSharedBoneCount = Skeleton->GetBoneCount();

		if ( BoneData.mBoneLocalMatrixArray.size() == 0 && Skeleton->GetBoneCount() > 0 )
			BoneData.mBoneLocalMatrixArray.resize( Skeleton->GetBoneCount() );

		for ( dword  i = 0;i< Skeleton->GetBoneCount();i++ )
		{
			BoneContent* Bone = Skeleton->GetBone( i );
			BoneData.mBoneLocalMatrixArray[i] = BoneData.mBoneLocalMatrixArray[i].FromTranslationRotation( Bone->mLocalTranslation, -Bone->mLocalRotation ).Transpose();
			
		}
#endif

		return &BoneData;
	}

}