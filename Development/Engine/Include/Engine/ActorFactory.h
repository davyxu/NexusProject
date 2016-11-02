#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup System
	*  @{
	*/


	class GameWorld;

	/** 
		角色工厂
	*/
	class ActorFactory : public RTTIObject
	{
	DECLARE_RTTI_ABSTRACT(ActorFactory, RTTIObject, ClassFlag_Intrinsic )
	public:
		/// 需要实现,创建角色后可设置属性,添加组件
		virtual Actor* CreateActor( const WString& Name, void* Parameter ) = 0;

	protected:
		friend class GameWorld;
		GameWorld* mOwner;
	};

	/** 
		静态模型角色工厂
	*/
	class ActorFactoryStaticMesh : public ActorFactory
	{
	DECLARE_RTTI_CLASS( ActorFactoryStaticMesh, ActorFactory, ClassFlag_Intrinsic )

	public:
		virtual Actor* CreateActor( const WString& Name, void* Parameter );	
	};

	/** 
		骨骼模型角色工厂
	*/
	class ActorFactorySkeletonMesh : public ActorFactory
	{
	DECLARE_RTTI_CLASS( ActorFactorySkeletonMesh, ActorFactory, ClassFlag_Intrinsic )

	public:
		virtual Actor* CreateActor( const WString& Name, void* Parameter );	
	};

	/** 
		编辑器用摄像机
	*/
	class ActorFactoryArcBallCamera: public ActorFactory
	{
		DECLARE_RTTI_CLASS( ActorFactoryArcBallCamera, ActorFactory, ClassFlag_Intrinsic )

	public:
		virtual Actor* CreateActor( const WString& Name, void* Parameter );	
	};

	/** 
		第一人称方式摄像机工厂
	*/
	class ActorFactoryFPSCamera: public ActorFactory
	{
		DECLARE_RTTI_CLASS( ActorFactoryFPSCamera, ActorFactory, ClassFlag_Intrinsic )

	public:
		virtual Actor* CreateActor( const WString& Name, void* Parameter );	
	};

		/** 
		第一人称方式摄像机工厂
	*/
	class ActorFactoryFocusCamera: public ActorFactory
	{
		DECLARE_RTTI_CLASS( ActorFactoryFocusCamera, ActorFactory, ClassFlag_Intrinsic )

	public:
		virtual Actor* CreateActor( const WString& Name, void* Parameter );	
	};

	/** @} */
	/** @} */
}
