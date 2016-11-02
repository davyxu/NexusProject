#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup System
	*  @{
	*/


	class RenderQueue;

	class Actor;

	typedef TRefPtr<Actor> ActorPtr; 

	/** 
		角色类
		封装游戏对象功能
		@remark
			支持组件扩展功能
			父子关系管理
	*/
	class Actor	: public ManagedObject
				, public Tickable
				, public MovableObject
	{
		DECLARE_RTTI_CLASS( Actor, ManagedObject, ClassFlag_Intrinsic| ClassFlag_Serializable )

	public:
		Actor();

		virtual ~Actor( );

		//////////////////////////////////////////////////////////////////////////
		// 组件
		//////////////////////////////////////////////////////////////////////////

		/// 添加一个组件
		template<typename TClass>
		TClass* AddComponent( )
		{
			return RTTI_CAST(TClass, AddComponent( RTTI_CLASS_INFO(TClass) ) );
		}

		/// 获取组件
		template<typename TClass>
		TClass* GetComponent( )
		{
			return RTTI_CAST(TClass, GetComponent( RTTI_CLASS_INFO(TClass) ) );
		}

		/**
			添加组件
			@param
				Class 组件类型
			@param
				Index 同类型的索引
			@return
				组件
		*/
		ActorComponent*		AddComponent( RTTIClass* Class, dword Index = 0 );

		/// 清除所有组件
		void				ClearComponent();

		
		/**
			获取组件
			@param
				Class 组件类型
			@param
				Index 同类型的索引
			@return
				组件
		*/
		ActorComponent*		GetComponent( RTTIClass* Class, dword Index = 0 );

		/// 遍历第一个组件
		ActorComponent* 	GetFirstComponent( );

		/// 获取下一个组件
		ActorComponent* 	GetNextComponent();

		/**
			移除组件
			@param
				Class 组件类型
			@param
				Index 同类型的索引
		*/
		void				RemoveComponent( RTTIClass* Class, dword Index = 0 );

		/**
			设置组件属性
			@param
				Class 组件类型
			@param
				Name 属性名
			@param
				Value 属性值
			@return
				是否设置成功
		*/
		bool				SetComponentPropertyByString( RTTIClass* Class, const wchar* Name, const wchar* Value );

		/**
			获取组件属性
			@param
				Class 组件类型
			@param
				Name 属性名
			@param
				Value 属性值
			@return
				是否获取成功
		*/
		bool				GetComponentPropertyAsString( RTTIClass* Class, const wchar* Name, WString& Value );

		/// 设置属性的模板类型自动填充
		template<typename TClass>
		bool SetPropertyByString( const wchar* Name, const wchar* Value )
		{
			return SetComponentPropertyByString( RTTI_CLASS_INFO( TClass ), Name, Value );
		}

		/// 获取属性的模板类型自动填充
		template<typename TClass>
		bool GetPropertyAsString( const wchar* Name, WString& Value )
		{
			return GetComponentPropertyAsString( RTTI_CLASS_INFO( TClass ), Name, Value );
		}

		//////////////////////////////////////////////////////////////////////////
		// 其他属性
		//////////////////////////////////////////////////////////////////////////

		/// 设置是否可见
		void SetVisible( bool Visible ){ mVisible = Visible; }

		/// 获取是否可见
		bool GetVisible( ){ return mVisible; }

		void SetPickable( bool Pickable );

		bool GetPickable( );

		dword GetUserData( ){ return mUserData; }

		void SetUserData( dword UserData ){ mUserData = UserData; }

		/// 获取父级
		Actor* GetParent( );

		/// 添加子角色
		void AddChild( Actor* A );

		/// 子角色是否
		bool ExistChild( Actor* A );

		/// 获取子角色数量
		dword GetChildCount( ){ return mChildActorList.size(); }

		/// 移除属性
		void RemoveChild( Actor* A );

		/// 移除所有的子角色
		void RemoveAllChild( );

		/// 遍历第一个子角色
		Actor* GetChildFirst( );

		/// 遍历下一个子角色
		Actor* GetChildNext( );

		/// 获取自己的本地绑定盒
		const AABB& GetBoundingBox( ){ return mSelfAABB; }

		/// 获取自己和子节点的世界绑定盒
		AABB GetComboWorldBoundingBox( );

		/// 获取自己的世界绑定盒
		AABB GetWorldBoundingBox();

		/// 设置绑定盒
		void SetBoundingBox( const AABB& BoundingBox ){ mSelfAABB = BoundingBox; }

		void SetTestColor( dword Color );

		/**
			拾取判定
			@param
				PickRay 光线
			@param
				Distance 拾取到的距离
			@return
				是否拾取到
		*/
		bool Pick( const Ray& PickRay, float& Distance );

		/**
			判定角色是否在屏幕范围内
			@param
				RV 拾取的视口
			@param
				ScreenArea 屏幕范围
			@return
				是否在屏幕范围内
		*/
		bool Pick( SceneView* RV, const FRect& ScreenArea );

		//////////////////////////////////////////////////////////////////////////
		// 内部使用
		//////////////////////////////////////////////////////////////////////////
		virtual void Tick( float Elapse );

		virtual void Serialize( BinarySerializer& Ser );

		void SetDebugName( const WString& Name ){ mDebugName = Name; }

		WString GetDebugName( );

		void SetFlag( dword Flag );

		dword GetFlag( );

		bool HasFlag( dword Mask );

	private:
		friend class GameWorld;

		/// 查找可绘制组件填充到渲染列表中
		void FindDrawableComponent( RenderQueue* Queue );

		/// 将可见物体加入渲染队列
		void FindVisibleObject( SceneView* RV, RenderQueue* Queue );

		/// 将子节点的绑定盒合并起来
		void RefreshCombineBoundingBox( );

		virtual MovableObject*	GetParentMovable( );

		virtual MovableObject* GetFirstMovable( );

		virtual MovableObject* GetNextMovable( );

		virtual void BeginDispose(  );

		void SetParent( Actor* A );

	
	private:
		friend class WorldPick;
		WString			mDebugName;
		Actor*			mParent;
		GameWorld*		mWorld;		// 归属哪个世界
		RTTIClass*		mFactoryClass;

		// Component = Front { NormalComponent...| ... DrawableComponent } Back
		typedef std::list<ActorComponent*> ComponentList;
		ComponentList mComponentList;
		ComponentList::iterator mFirstGeometryComponent;
		ComponentList::iterator mComponentIterator;


		bool		mVisible;


		dword		mUserData;
		bool		mPickable;// 0: 不拾取，1:设置拾取，等待分配颜色,其他:拾取颜色

		friend dword GetTypeHash( Actor* A )
		{
			return PointerHash(A);
		}
		

		typedef std::list<Actor*> ChildActorList;

		ChildActorList mChildActorList;

		ChildActorList::iterator mChildIterator;


		AABB		mChildAABB;			// 所有子成员合并后的绑定盒
		AABB		mSelfAABB;			// 通过组件更新的自身绑定盒

		dword		mFlag;
	};


	typedef std::vector<Actor*> ActorArray;

	/** @} */
	/** @} */
}
