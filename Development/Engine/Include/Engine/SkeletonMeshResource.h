#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Model
	*  @{
	*/

	struct BoneContent;

	typedef std::vector< BoneContent* > BoneArray;
	typedef std::vector<dword> MappingArray;

	/** 
		骨头
	*/
	struct BoneContent
	{
		WString			mName;
		int 			mParentID;
		MappingArray	mChildArray;

		Vector3			mRelativeTranslation;
		Quaternion		mRelativeRotation;
		Vector3			mAbsoluteTranslation;
		Quaternion		mAbsoluteRotation;

		Vector3			mRelativeTranslationInit;
		Quaternion		mRelativeRotationInit;

		Vector3			mLocalTranslationBoneSpace;
		Quaternion		mLocalRotationBoneSpace;
		Vector3			mLocalTranslation;
		Quaternion		mLocalRotation;

		friend BinarySerializer& operator <<( NekoEngine::BinarySerializer& Ser, BoneContent*& Data )
		{
			if ( Ser.IsLoading() )
			{
				Data = new BoneContent;
			}

			Ser << Data->mName 
				<< Data->mParentID
				<< Data->mRelativeTranslation
				<< Data->mRelativeRotation
				<< Data->mLocalTranslationBoneSpace
				<< Data->mLocalRotationBoneSpace;				

			if ( Ser.IsLoading() )
			{
				Data->mRelativeTranslationInit = Data->mRelativeTranslation;
				Data->mRelativeRotationInit = Data->mRelativeRotation;
			}

			return Ser;
		}
	};

	/** 
		传输给Shader的，这一次一个局部Skin需要用到的骨骼矩阵
	*/
	struct BoneShaderData
	{
		typedef std::vector<Matrix4> BoneLocalMatrix;

		dword					mSharedBoneCount;
		BoneLocalMatrix			mBoneLocalMatrixArray;
	};

	typedef std::vector<BoneShaderData> BoneShaderBuffer;

	/// 蒙皮要使用到的骨骼索引
	typedef std::vector<dword> BoneIndexArray;			// 记录每个子模型使用的骨骼信息

	/// 骨骼模型顶点扩充
	class SkeletonMeshVertexFactory : public MeshVertexFactory
	{
		DECLARE_RTTI_CLASS( SkeletonMeshVertexFactory, MeshVertexFactory, ClassFlag_Intrinsic )

	public:
		BoneIndexArray		mBoneIndices;
	};
	
	/** 
		骨骼模型资源实现
	*/
	class SkeletonMeshResource : public MeshResource
	{
		DECLARE_RTTI_CLASS( SkeletonMeshResource, MeshResource, ClassFlag_Intrinsic|ClassFlag_Serializable )
	public:
		SkeletonMeshResource( );

		virtual ~SkeletonMeshResource( );

		/// 添加骨头
		dword AddBone( BoneContent* Bone );

		/// 获取骨头
		BoneContent* GetBone( dword boneid );

		/// 生成共享蒙皮的骨骼索引
		BoneShaderData* GenBoneIndexMatrix( BoneShaderBuffer& DataArray, SkeletonInstance* Skeleton, dword MeshContentIndex);

	public:
		virtual void Serialize( BinarySerializer& Ser );

	protected:
		void BuildBoneIndex( );
		
	private:
		friend class SkeletonInstance;
		BoneArray		mBoneArray;
		MappingArray	mRootArray;
	};

	typedef TRefPtr<SkeletonMeshResource> SkeletonMeshResourcePtr;

	/** @} */
	/** @} */
}

