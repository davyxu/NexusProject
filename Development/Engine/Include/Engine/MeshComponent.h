#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Model
	*  @{
	*/

	struct BoneShaderData;
	class MeshResource;

	/** 
		通用模型组件
	*/
	class MeshComponent : public GeometryComponent
	{
	DECLARE_RTTI_ABSTRACT( MeshComponent, GeometryComponent, ClassFlag_Intrinsic )
	public:
		MeshComponent( );

		virtual ~MeshComponent( );

		/// 设置模型的材质对象
		void SetMaterialInstance( MaterialInstance* MatIns );

		MaterialInstancePtr GetMaterial( );

		/// 需要实现:获取模型资源类
		virtual MeshResource* GetMeshResource( ) = 0;

		/// 绘制
		virtual void Draw( SceneView* RV );

		/// 需要实现:拾取
		virtual bool Pick( const Ray& PickRay, float& Distance ){ return false; }

		/// 需要实现:拾取
		virtual bool Pick( SceneView* RV, const FRect& ScreenArea ){ return false; }

		virtual bool IsTransparent( );

		virtual bool IsEnableDepthWrite( );

		virtual float GetViewDepthSquared(Camera* C  );

		static void StaticInitProperty();

		virtual void Serialize( BinarySerializer& Ser );

	protected:

		struct MeshRenderParameter 
		{
			Matrix4					mLocalToScreen; // 转换到屏幕
			Matrix4					mLocalToWorld;	// 本地转换
			Matrix4					mWorldToLocal;	// 世界转本地
			MaterialInstance*		mMaterialInstance;
			VertexFactory*			mVertexFactory;			
			BoneShaderData*			mBoneData;
			dword					mPickID;
			SceneView*				mRenderView;
		};

		virtual void CheckResourceStage( );

		virtual bool IsResourceReady( ) = 0;

		/// 绘制前需要填充的渲染参数
		virtual void OnRenderParameter( dword Index, MeshRenderParameter& Parameter ){ }

	protected:
		ResourcePtrMarker<MaterialInstancePtr>		mMaterialInstance;

		/// 渲染线程绘制模型
		static void DrawMesh_RenderThread( MeshRenderParameter& Parameter );

		/// 属性系统设置模型回调
		void OnPropertyMaterial( WString* Value, bool IsGet );

	};
}
