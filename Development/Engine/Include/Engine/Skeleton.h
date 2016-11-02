#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Model
	*  @{
	*/


	/** 
		骨骼
	*/
	class SkeletonInstance 
	{
	public:
		SkeletonInstance( SkeletonMeshResource* skeleton );
		virtual ~SkeletonInstance( );

		/// 根据名称获取骨骼ID
		dword GetBoneID( const wchar* bonename );

		/// 获取骨骼
		BoneContent* GetBone( dword boneid );

		/// 根据名称获得骨骼对象
		BoneContent* GetBone( const wchar* bonename );

		/// 获取骨骼数量
		dword GetBoneCount( );

		/// 设置影响点
		void SetInfluence( BoneContent* bone, const Vector3& translation, const Quaternion& rotation );

		/// 清除影响点
		void ClearInfluence( );

		/// 计算骨骼绑定盒
		AABB CaculateSkeletonBoundingBox( );

	public:
		virtual void Tick( float elapse );

	private:
		void CaculateSkeletonBoundingBox( AABB& BoneBox, BoneContent* Bone );
		


	private:
		void UpdateBone( BoneContent* bone );
		

		friend class SkeletonAnimController;
		friend class ActorHelperComponent;
		BoneContent*	mInfluenceBone;
		Vector3			mInfluenceTranslation;
		Quaternion		mInfluenceRotation;
		BoneArray		mBoneArray;
		MappingArray	mRootArray;
	};

	/** @} */
	/** @} */
}
