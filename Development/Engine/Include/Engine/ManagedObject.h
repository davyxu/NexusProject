#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup System
	*  @{
	*/

	/// 托管对象垃圾回收策略
	enum eGarbageCollectPolicy
	{		
		GCP_Full,				///< 完全扫描回收
		GCP_Finalize,			///< 析构回收
	};

	class ManagedObject;


	/** 
		资源加载的观察者
	*/
	class ObjectSerializeObserver : public AsyncTask
	{
	public:
		ObjectSerializeObserver(  ManagedObject* Obj, BinarySerializer* Ser  );

		virtual ~ObjectSerializeObserver( );

		/// 加载线程处理中
		virtual void OnAsyncTaskProcess();

		/// 游戏线程处理结束
		virtual void OnAsyncTaskFinished( );

		/// 资源是否加载结束
		bool IsFinished( );

		/// 同步方式完成任务,并删除自己
		void StartSync( );

		/// 异步投递任务
		void PostAsync( );
	private:
		ManagedObject*		mObject;	
		BinarySerializer*	mSerializer;
	};

	enum eObjectSpawnType
	{
		OST_Unknown,
		OST_Constructed,
		OST_Loaded,
		OST_Savable,
	};
	
	/** 
		托管对象
		支持后台资源加载,卸载控制
		@remark
			ManagedObject不需要主动Dispose,原因:其他人还有引用时,资源删除也是不合理的
	*/
	class ManagedObject	: public RTTIObject
						, public RefObject
	{
	public:
		ManagedObject( );

		DECLARE_RTTI_CLASS( ManagedObject, RTTIObject, ClassFlag_Intrinsic )

		virtual ~ManagedObject( );

		/// 包资源加载OK,此标志由系统自动完成
		bool IsPackageResourceReady( );

		/// 包资源加载完毕后调用,派生类可以自行决定是否需要由ManualResource加载过程,例如GPU资源
		virtual void PostLoad( ){ }

		/// 手动资源加载是否OK, 由派生类提供,例如GPU资源
		virtual bool IsManualResourceReady( ){ return true; }

		/// 包资源及手动资源都准备好了
		virtual bool IsResourceReady( ){ return IsManualResourceReady() && IsPackageResourceReady(); }

		/// 对象没有外部引用时,调用资源析构
		virtual void BeginDispose( ){}

		/// 对象是否正在Dispose
		bool IsDisposing( ){ return mIsDisposing; }

		/// 带有Dispose的对象是否已经卸载完资源可以删除?
		virtual bool CanDestroyNow( ){ return true; }

		/// 设置名称
		void SetName( const WString& Name );

		/// 获取名称
		const WString& GetName( ){ return mName; }

		/// 使用ConstructObject构造的对象做资源时,必须调用此函数以便标记资源OK
		void MarkPackageResourceReady( ){ mPackageResourceReady = true; }


	
	private:
		friend class ObjectSerializeObserver;
		friend class ObjectManager;
		WString					mName;
		dword					mObjectIndex;
		ManagedObject*			mHashNext;
		bool					mIsDisposing;

		// 资源加载阶段是否完成
		bool					mPackageResourceReady;		

		// 创建时的类型
		eObjectSpawnType		mSpawnType;
	};


	/**
		托管对象管理器
	*/
	class ObjectManager : public RTTIObject
	{
	public:
		DECLARE_RTTI_CLASS( ObjectManager, RTTIObject, ClassFlag_Intrinsic )

		enum{ OBJECT_HASH_BINS = 4096 };

		ObjectManager( );

		virtual ~ObjectManager( );

		/**
			构建一个自动管理对象	
			@param
				ClassInfo 对象的类型
			@param
				Name 对象名称
			@return
				创建好的对象
			@remark
				名称为空时自动根据类型确定名称
		*/
		ManagedObject* ConstructObject( RTTIClass* ClassInfo, const WString& Name );

	
		/**
			创建一个自动管理对象,模板填充类型
			@param
				Name 对象名称
			@return
				创建好的对象
			@remark
				名称为空时自动根据类型确定名称
		*/
		template<typename TClass>
		TClass* ConstructObject( const WString& Name )
		{
			return RTTI_CAST(TClass, ConstructObject( RTTI_CLASS_INFO(TClass), Name ) );
		}

		
		/**
			销毁对象
			@param
				Obj 对象
		*/
		void DestructObject( ManagedObject* Obj );

		/**
			将对象添加到管理中
			@param
				Obj 对象
			@param
				Name 名称
			@return
				
		*/
		bool LinkObject( ManagedObject* Obj, const WString& Name );
		
		/**
			查找对象
			@param
				ClassInfo 对象的类型
			@param
				Name 对象名称
			@return
				查找到的对象
		*/
		ManagedObject* FindObject( RTTIClass* ClassInfo, const WString& Name );

		/**
			查找此类型中的第一个对象
			@param
				ClassInfo 对象的类型
			@param
				Name 对象名称
			@return
				查找到的对象
		*/
		ManagedObject* FindFirstObjectByKind( RTTIClass* ClassInfo, const WString& Name );

		/**
			查找此类型中的下一个对象
			@param
				ClassInfo 对象的类型
			@param
				Name 对象名称
			@return
				查找到的对象
		*/
		ManagedObject* FindNextObjectByKind( RTTIClass* ClassInfo, ManagedObject* KindOfObject );


		/// 获取对象的日志类
		Logger* GetLogger( ){ return GObjectLogger; }

		
		/**
			加载对象
			@param
				Name 资源名/对象名
			@return
				加载好的对象
		*/
		template<typename TClass>
		TClass* LoadObject( const WString& Name, bool Background = true )
		{
			return RTTI_CAST(TClass, LoadObject(  RTTI_CLASS_INFO(TClass), Name, Background  ) );
		}

		/**
			加载对象
			@param
				ClassInfo 对象的类型
			@param
				Name 资源名/对象名
			@return
				对象
		*/
		ManagedObject* LoadObject( RTTIClass* ClassInfo, const WString& Name, bool Background );


		/// 手动将对象名称置入管理
		void AddNamedObject( ManagedObject* MO);

		/// 断开对象管理,此时对象无法查找,需要自己管理内存
		void RemoveNamedObject( ManagedObject* MO);



		/// 写入资源	
		bool SaveToFile( ManagedObject* MO, dword FileVersion );

		/// 获取资源路径
		const WString& GetContentPath( ){ return mContentPath; }
		
		/**
			设置资源路径
			@param
				Value 相对于可执行文件的路径
		*/
		void SetContentPath( const WString& Value );

		/// 配置读取
		virtual void SerializeConfig( PropertySerializer& Ser );

		/// 检查资源加载
		void CheckResourceProcessing( );

		/**
			进行垃圾回收
			@param
				Policy 回收策略
		*/
		void CollectGarbage( eGarbageCollectPolicy Policy );

		static void StaticInitProperty( );

	private:
		WString MakeUniqueObjectName( RTTIClass* ClassInfo );
		

	private:
		
		typedef std::vector<ManagedObject*> ObjectArray;
		typedef std::vector<dword>	   FreeObjectArray;
		typedef std::list<ManagedObject*>	ObjectList;
		
		WString			mContentPath;

		// 对象的Hash池,名字做Key
		ManagedObject*		GObjectHash[OBJECT_HASH_BINS];

		// 对象池,并非连续,有空位
		ObjectArray		GObjects;

		// 对象实际数量
		dword			GObjectCount;

		// 空余对象位
		FreeObjectArray	GFreeObjectArray;


		Logger*			GObjectLogger;

		// 析构队列
		ObjectList		GDisposingList;

		// 增量垃圾回收进度
		dword			GPurgeObjectIndex;

		typedef std::set<ObjectSerializeObserver*> ObjectSerializeList;

		ObjectSerializeList		GLoadingList;
	};

	extern ObjectManager* GObjectManager;

	/** @} */
	/** @} */	
}
