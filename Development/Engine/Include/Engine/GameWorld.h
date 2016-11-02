#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup System
	*  @{
	*/


	/** 
		游戏世界, 对Actor进行管理, Actor不使用垃圾回收
	*/
	class GameWorld : public RTTIObject
	{
		DECLARE_RTTI_CLASS( GameWorld, RTTIObject, ClassFlag_Intrinsic| ClassFlag_Serializable )
	public:
		GameWorld( ){}

		GameWorld( dword Index );

		virtual ~GameWorld( );

		/**
			创建一个角色
			@param
				FactoryClass 工厂类
			@param
				Name 角色名
			@param
				Parent 父级
			@return
				创建好的角色
		*/
		Actor* SpawnActor( RTTIClass* FactoryClass , const WString& Name , Actor* Parent = null, void* Parameter = null );
		

		/**
			获取角色工厂实例
			@param
				FactoryClass 工厂类型
			@return
				工厂类
		*/
		ActorFactory* GetActorFactory( RTTIClass* FactoryClass )
		{
			ActorFactory** FactoryRef = mActorFactoryMap.find( FactoryClass );

			if ( FactoryRef == null )
				return null;

			return *FactoryRef;
		}

		/// 获取角色工厂实例,模板自动填充
		template<typename TClass>
		TClass* GetActorFactory( )
		{
			return RTTI_CAST( TClass, GetActorFactory( RTTI_CLASS_INFO(TClass) ) );
		}

	
		/// 获取根角色
		Actor* GetRootActor( ){ return mRootActor; }

		/// 获取渲染列表
		RenderQueue* GetRenderQueue( ){ return mRenderQueue; }

		/// 获取摄像机
		Camera* GetCamera( const WString& Name );

		// 删除对象，可以指定某种类型
		void Kill( const WString& Name, RTTIClass* ClassInfo = RTTI_CLASS_INFO( Actor ) );

		/// 删除一个Actor及其所有子Actor
		void Kill( Actor* A );

		Actor* GetActor( const WString& Name, RTTIClass* ClassInfo = RTTI_CLASS_INFO( Actor )  );

		/// 序列化Actor
		void SerializeActors( BinarySerializer& Ser, Actor* Parent, dword IncludeFlag = 0, ActorArray* RootChildActors = null );		

		/// 隔离一个对象,让其独立于对象管理系统
		static void IsolateFromSystem( Actor* A );

		/// 恢复被隔离的对象
		static void RestoreFromSystem( Actor* A );

	public:

		void Draw( SceneView* RV );

		virtual void Serialize( BinarySerializer& Ser );
		


	private:
		

	private:
		typedef dynamic_hash<RTTIClass*, ActorFactory*> ActorFactoryMap;
		ActorFactoryMap mActorFactoryMap;

		Actor* mRootActor;

		RenderQueue* mRenderQueue;
	};

	/** @} */
	/** @} */
}
