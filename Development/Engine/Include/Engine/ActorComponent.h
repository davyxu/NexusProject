#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup System
	*  @{
	*/


	class Actor;

	/** 
		组件基类
	*/
	class ActorComponent : public RTTIObject
	{
	DECLARE_RTTI_CLASS( ActorComponent, RTTIObject, ClassFlag_Intrinsic )

	public:
		ActorComponent( );

		virtual void Tick( float Elapse );

		/// 挂接到Actor时
		virtual void OnAttach( ){}

		/// 从Actor断开时
		virtual void OnDetach( ){}

		/// 是否可绘制
		virtual bool IsDrawable( ) { return false; }

		/// 获取归属的Actor
		Actor* GetOwner( ){ return mOwner; }

	protected:
		friend class Actor;
		Actor*			mOwner;

	private:
		friend class Actor;
		dword			mIndex;
	};

	class SceneView;


	/** 
		可绘制组件
		@remark
			自动添加到绘制列表
	*/
	class GeometryComponent : public ActorComponent
	{
	DECLARE_RTTI_ABSTRACT( GeometryComponent, ActorComponent, ClassFlag_Intrinsic )

	public:
		
		GeometryComponent( );

		/// 需要实现的绘制过程
		virtual void Draw( SceneView* RV ) = 0;

		virtual bool IsDrawable( ) { return true; }

		/// 需要实现: 是否有透明层,决定绘制顺序
		virtual bool IsTransparent( ){ return false; }

		/// 需要实现: 是否需要写入深度.平面时返回false
		virtual bool IsEnableDepthWrite( ){ return true; }

		/// 对象到摄像机的路径
		virtual float GetViewDepthSquared( Camera* C ){ return Math::cMaxFloat; }

		/// 获取渲染分组
		dword GetRenderQueueGroup( ){ return mRenderQueueGroup; }

		/// 设置渲染分组
		void SetRenderQueueGroup( dword GroupID ){ mRenderQueueGroup = GroupID; }

		dword		mPickID;

	private:
		dword	mRenderQueueGroup;
	};

	
	/** 
		绘制网格和轴线的组件,提供给世界Root使用
	*/
	class GridAxisComponent : public GeometryComponent
	{
		DECLARE_RTTI_CLASS( GridAxisComponent, GeometryComponent, ClassFlag_Intrinsic )
	public:
		virtual void Draw( SceneView* RV );
	};

	class SkeletonInstance;
	class Canvas;
	struct BoneContent;

	/** 
		在主RT的第一个View中绘制帧率信息	
	*/
	class ActorHelperComponent : public GeometryComponent
	{
		DECLARE_RTTI_CLASS( ActorHelperComponent, GeometryComponent, ClassFlag_Intrinsic )
	public:
		ActorHelperComponent( );

		static void StaticInitProperty( );

		virtual void Draw( SceneView* RV );

		Color mBoundingBoxColor;
		Color mBoneStartColor;
		Color mBoneEndColor;

		bool mShowBoundingBox;
		bool mShowSkeleton;

	private:
		void DrawSkeleton( SkeletonInstance* Skeleton, Canvas* Painter );

		void DrawSkeleton( SkeletonInstance* Skeleton, Canvas* Painter, BoneContent* Bone );

	};


	/** @} */
	/** @} */
}
