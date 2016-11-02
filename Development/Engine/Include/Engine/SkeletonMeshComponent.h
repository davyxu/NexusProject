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
		骨骼动画组件实现
	*/
	class SkeletonMeshComponent : public MeshComponent
	{
		DECLARE_RTTI_CLASS( SkeletonMeshComponent, MeshComponent, ClassFlag_Intrinsic )
	public:
		SkeletonMeshComponent( );

		virtual ~SkeletonMeshComponent( );

		/// 设置蒙皮资源
		void SetSkin( const SkeletonMeshResourcePtr& Skin );

		/// 设置动画资源
		void SetAnimation( const SkeletonAnimResourcePtr& Anim );

		/// 获得骨骼
		SkeletonInstance* GetSkeleton( ){ return mSkeletonInstance; }

		/// 获取动画控制器
		SkeletonAnimController* GetAnimationController( ){ return mSkeletonAnimController; }

	public:
		virtual bool IsResourceReady( );

		virtual MeshResource* GetMeshResource( );

		virtual void OnRenderParameter( dword Index, MeshRenderParameter& Parameter );

		virtual bool Pick( SceneView* RV, const FRect& ScreenArea );

		virtual bool Pick( const Ray& PickRay, float& Distance );

		static void StaticInitProperty();

		virtual void Tick( float elapse );

		virtual void Serialize( BinarySerializer& Ser );

	protected:
		virtual void CheckResourceStage( );

		void OnPropertySkin( WString* Value, bool IsGet );

		void OnPropertyAnimation( WString* Value, bool IsGet );
		

	private:
		enum {

			SM20_MAX_BONE_MATRIX = 50,		// 256 / 4 = 64 ~= 50
		};



	private:
		ResourcePtrMarker<SkeletonMeshResourcePtr>			mSkin;
		ResourcePtrMarker<SkeletonAnimResourcePtr>	mAnimation;
		SkeletonInstance*						mSkeletonInstance;
		SkeletonAnimController*					mSkeletonAnimController;

		BoneShaderBuffer						mBoneShaderBuffer;// 为每个Mesh实例准备的骨骼蒙皮数据
	};

	/** @} */
	/** @} */	
}
