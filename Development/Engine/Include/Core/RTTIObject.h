#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Reflection
	*  @{
	*/

	typedef class RTTIObject* (*ClassConstructor)( );

	#define RTTIOBJECT_HASHBOUND 4096
	#define RTTIPACKAGE_HASHBOUND 32

	class RTTIClass;

	/** 
		RTTI组
	*/
	class RTTIGroup
	{
	public:
		RTTIGroup( );

		/** 
			遍历第一个组中类
		*/
		RTTIClass* GetFirstClass( );

		/** 
			遍历第一个组中下一个类
		*/
		RTTIClass* GetNextClass( );

		/** 
			获取本组内类数量
		*/
		dword GetClassCount( );

		/** 
			添加一个类
		*/
		void AddClass( RTTIClass* ClassInfo );

		/** 
			类是否存在
		*/
		bool ClassExist( RTTIClass* ClassInfo );

	private:
		typedef std::vector<RTTIClass*> ClassArray;
		ClassArray mClassArray;
		dword	   mIterator;
	};

	/** 
		类静态标示
	*/
	enum eClassFlag
	{
		ClassFlag_None			= 0x00000000,	///< 正常C++类
		ClassFlag_Serializable	= 0x00000001,	///< 可以序列化
		ClassFlag_Abstract		= 0x00000002,	///< 抽象类
		ClassFlag_HasProperty	= 0x00000004,	///< 拥有属性，自动初始化属性
		ClassFlag_Intrinsic		= 0x10000000,	///< 内建类，非脚本类
	};

	/** 
		RTTI类信息
	*/
	class RTTIClass
	{
	public:
		/// 类名称
		NameRef			mClassName;

		/// 分配到哪个包，用于遍历创建识别
		NameRef			mPackageName; 

		/// 类分配实例的参考索引
		dword			mClassUnique;

		/// 父类
		RTTIClass*		mParentClass; 

		/// 第一个子类
		RTTIClass*		mChildClass;  

		/// 同级派生类
		RTTIClass*		mNextClass;	  

		/// 归属哪个组
		RTTIGroup*		mGroup;

		/// 类属性
		dword			mClassFlag;	  

		/// 创建类实例
		RTTIObject* CreateInstance(  );

		/** 
			构造函数
			@param
				ClassName 类名
			@param
				CreateFunc 类创建函数
			@param
				ParentClass 父类
			@param
				ClassFlag 类标示

		*/
		RTTIClass(	const NameRef& ClassName, 
					ClassConstructor CreateFunc, 
					RTTIClass* ParentClass, dword ClassFlag );

		virtual ~RTTIClass( );

		/** 
			取Hash值
		*/
		dword GetHash( ) const { return mClassName.GetIndex(); }

		/** 
			取RTTI类型名字符串
		*/
		const wchar* GetRTTIClassString( );

		/** 
			取RTTI类型名
		*/
		const NameRef& GetRTTIClassName( );

		/** 
			判断类相等
		*/
		bool IsA( RTTIClass* ClassInfo );

		/** 
			判断类相等
		*/
		bool IsA( const NameRef& ClassName );

		/** 
			判断类是某一个类型
		*/
		bool IsKindOf( RTTIClass* ClassInfo );

		/** 
			返回是否拥有某种类标示
		*/
		bool HasFlag( dword ClassFlag );

		friend dword GetTypeHash( RTTIClass* A )
		{
			return PointerHash(A);
		}

		/** 
			取得类属性接口
		*/
		PropertyCollection* GetPropertyCollection( );

	private:
		friend class RTTIObject;

		ClassConstructor mCreateEntry;
		PropertyCollection*	mPropertyCollection;
	};



	class BinarySerializer;

	/** 
		RTTI对象基类
		@remark
			配合宏,可以自动注册动态类型识别类
	*/
	class RTTIObject
	{
	public:
		/** 
			取类信息
		*/
		virtual RTTIClass* GetClass( ){ Assert( sRTTIInfo != null && "RTTIObject not initialized, call Object::StaticInit first"); return sRTTIInfo; } 

		static RTTIClass* StaticClass( );
		static RTTIObject* StaticConstruct( ){ return new RTTIObject;}

		/** 
			二进制序列化
		*/
		virtual void Serialize( BinarySerializer& Ser ){}

		/**
			配置序列化
		*/
		virtual void SerializeConfig( PropertySerializer& Ser );

		/** 
			对象操作符重载
		*/
		friend BinarySerializer& operator << ( BinarySerializer& Ser, RTTIObject*& Obj );
		friend BinarySerializer& operator << ( BinarySerializer& Ser, RTTIObject& Obj ); 
		static RTTIClass* sRTTIInfo;
	public:
		virtual ~RTTIObject( ){}
		//////////////////////////////////////////////////////////////////////////
		// Class Instance Function
		//////////////////////////////////////////////////////////////////////////

		/** 
			类实例相等
		*/
		bool EqualTo( RTTIObject* ClassObj );

		/** 
			类相等
		*/
		bool IsA( RTTIClass* ClassInfo );

		/** 
			类相等
		*/
		bool IsA( const NameRef& rttitype );

		/** 
			归属相同
		*/
		bool IsKindOf( RTTIClass* ClassInfo );

		/** 
			类型转换
		*/
		template<typename TClass>
		TClass* Cast( )
		{
			return IsKindOf( TClass::StaticClass() ) ? (TClass*)this:null;
		}

		/** 
			静态类型转换
		*/
		template<typename TClass>
		static TClass* Cast( RTTIObject* Obj )
		{
			if( Obj == null )
				return null;

			return Obj->IsKindOf( TClass::StaticClass() ) ? (TClass*)Obj:null;
		}


		//////////////////////////////////////////////////////////////////////////
		// Property Access
		//////////////////////////////////////////////////////////////////////////

		bool SetObjectPropertyByString( const wchar* Name, const WString& Value, bool SeekWholeFamily = true );

		bool SetObjectProperty( const wchar* Name, PropertyValue& PV, bool SeekWholeFamily = true );

		bool GetObjectPropertyAsString( const wchar* Name, WString& Value, bool SeekWholeFamily = true );

		bool GetObjectProperty( const wchar* Name, PropertyValue& PV, bool SeekWholeFamily = true );

		template<typename TOptionType>
		bool GetObjectPropertyT( const wchar* Name, TOptionType& Value, bool SeekWholeFamily = true )
		{
			return GetObjectProperty( Name, TPropertyValueRef<TOptionType>( Value, typeconv<TOptionType>().value_typenameW() ), SeekWholeFamily );
		}

		/// 检查给定值是否与属性相等
		template<typename TOptionType>
		bool CheckObjectPropertyT( const wchar* Name, TOptionType Value, bool SeekWholeFamily = true )
		{
			TOptionType Ret;
			if ( !GetObjectPropertyT( Name, Ret, SeekWholeFamily ) )
				return false;

			return Value == Ret;
		}

		/// 自动模板匹配类型
		template<typename TOptionType>
		bool SetObjectPropertyT( const wchar* Name, TOptionType Value, bool SeekWholeFamily = true )
		{	
			return SetObjectProperty( Name, TPropertyValueRef<TOptionType>( Value, typeconv<TOptionType>().value_typenameW() ), SeekWholeFamily );
		}
		//////////////////////////////////////////////////////////////////////////
		// RTTI Object Management
		//////////////////////////////////////////////////////////////////////////

		// Private Usage
		static void StaticInit( );

		static void StaticExit( );

		static void Dump( Logger* logger );

		/// 创建类实例
		static RTTIObject*	CreateInstance( const wchar* ClassName );

		/// 创建类实例
		static RTTIObject*	CreateInstance( const NameRef& ClassName );

		/// 注册类信息
		static void			Register( RTTIClass* ClassInfo );

		/// 反注册类信息
		static void			UnRegister( RTTIClass* ClassInfo );

		/// 查找类信息
		static RTTIClass*	FindClass( const NameRef& ClassName );

		/// 清除所有类信息
		static void			ClearClass( );	

		/// 遍历第一个类信息
		static RTTIClass*	GetFirstClass( );

		/// 遍历下一个类信息
		static RTTIClass*	GetNextClass( );

		/// 获取类数量
		static dword		GetClassCount( );

		/// 取得类组
		static RTTIGroup*	GetGroup( const NameRef& Name );

		/// 清除所有类组
		static void			ClearGroup( );

		/// 绑定类到类组
		static void			BindGroup( RTTIClass* ClassInfo, RTTIGroup* Group );

		/// 搜索对象所有继承层次访问属性
		bool SeekAccessObjectProperty( const wchar* Name, PropertyValue* PV, bool IsGet, bool SeekWholeFamily );

	private:
		
		struct RTTIObjectHashFunction
		{
			// 按照资源id索引
			dword operator()( const RTTIClass* key ) const
			{
				return key->GetHash( );
			}

			bool operator()(const RTTIClass* key1, const RTTIClass* key2 ) const
			{
				return key1->mClassName == key2->mClassName;
			}
		};

		typedef static_hash<RTTIClass*,RTTIClass*,RTTIObjectHashFunction, RTTIOBJECT_HASHBOUND> ClassInfoMap;

		static ClassInfoMap::iterator mIterator;
		static ClassInfoMap mClassInfoMap;

		typedef std::map<NameRef, RTTIGroup*> GroupMap;
		static GroupMap mGroupMap;
	};

	#define DECLARE_RTTI_BASE(TClass, TSuperClass, StaticClassFlag) \
		public:\
		typedef	TSuperClass Super;\
		enum { StaticClassFlags = StaticClassFlag };\
		virtual RTTIClass* GetClass( ){ Assert( sRTTIInfo != null && "RTTIObject not initialized, call Object::StaticInit first"); return sRTTIInfo; } \
		static RTTIClass* StaticClass( );\
		private:\
		static RTTIClass* sRTTIInfo;\
		friend BinarySerializer& operator << ( BinarySerializer& Ser, TClass*& Obj ) \
		{  return Ser.SerializeObject( Obj ); }\
		friend BinarySerializer& operator << ( BinarySerializer& Ser, TClass& Obj ) \
		{ Assert( Ser.IsSaving() ) \
		Ser.WriteObject( &Obj ); \
		Obj.Serialize( Ser );\
		return Ser;	} \
		public:

	/// 声明 类，父类，类标志
	#define DECLARE_RTTI_CLASS( TClass, TSuperClass, StaticClassFlag ) \
			DECLARE_RTTI_BASE( TClass, TSuperClass, StaticClassFlag ) \
			static RTTIObject* StaticConstruct( ){ return new TClass;}\
			public:

	/// 声明抽象 类，父类，类标志
	#define DECLARE_RTTI_ABSTRACT( TClass, TSuperClass, StaticClassFlag  ) \
			DECLARE_RTTI_BASE(TClass, TSuperClass, StaticClassFlag | ClassFlag_Abstract ) \
			static RTTIObject* StaticConstruct( ){ return null;}\
			public:

	/// 类实现
	#define IMPLEMENT_RTTI_CLASS( TClass )\
		RTTIClass* TClass::sRTTIInfo = null;\
		RTTIClass* TClass::StaticClass( )	\
		{if ( sRTTIInfo ) return  sRTTIInfo;\
		sRTTIInfo = new RTTIClass( UTEXT(#TClass), TClass::StaticConstruct, Super::StaticClass(), StaticClassFlags ); \
		RTTIObject::Register( sRTTIInfo ); \
		return sRTTIInfo;} \

	/// 注册类
	#define RTTI_REGISTER( TClass ) TClass::StaticClass( );

	/// 注册类,并加组
	#define RTTI_REGISTER_GROUP( TClass, GroupInstance ) RTTIObject::BindGroup( TClass::StaticClass( ), GroupInstance );

	/// 类型转换
	#define RTTI_CAST( TClass, TInstance ) RTTIObject::Cast<TClass>( TInstance )

	// 静态RTTI
	#define RTTI_CLASS_NAME( TClass ) TClass::StaticClass()->mClassName
	#define RTTI_CLASS_INFO( TClass ) ( TClass::StaticClass() )

	// 实例RTTI
	#define RTTI_INSTANCE_STRING( TInstance ) ( TInstance->GetClass()->GetRTTIClassString() )
	#define RTTI_INSTANCE_NAME( TInstance ) ( TInstance->GetClass()->mClassName )
	#define RTTI_INSTANCE_CLASS( TInstance ) ( TInstance->GetClass() )


	// 静态类型
	// RTTIObject::StaticClass() 获取静态类信息
	
	// 实例类型
	//class SomeClass : public RTTIObject
	//{
	//	DECLARE_RTTIOBJECT(SomeClass)
	//};

	//IMPLEMENT_RTTIOBJECT( SomeClass, "SomeClass", "SomeClass" )

	//SomeClass a;
	//a.GetObjectClassInfo();		// 获取实例类信息
	//a.GetObjectClassName();		// 获取实例类名字符串
	//a.GetObjectClassNameRef(); // 获取实例类名引用


	/** @} */
	/** @} */
}