#pragma once


namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Reflection
	*  @{
	*/

	class RTTIObject;
	class PropertyCollection;
	class PropertyValue;

	/**
		属性组
	*/

	class PropertyGroup
	{
	public:
		explicit PropertyGroup( const WString& Name ) : mGroupName( Name ){ }

		/** 
			设置属性组
			@return
				便于连缀表达式
		*/
		PropertyGroup& SetName( const WString& Name ){ mGroupName = Name; return *this;}

		/// 获取属性组名
		const WString& GetName( ){ return mGroupName; }

		/// 获取这组内的属性
		PropertyValue* GetPropery( dword Index ){ return mPropertyValueArray[Index]; }

		/// 获取这组内的属性数量
		dword GetPropertyCount( ){ return mPropertyValueArray.size(); }

		/// 添加属性到本组
		void AddProperty( PropertyValue* PV ){ mPropertyValueArray.push_back( PV ); }

	private:
		WString mGroupName;

		typedef std::vector<PropertyValue*> PropertyValueArray;
		PropertyValueArray mPropertyValueArray;		
	};

	/** 
		属性值
	*/
	class PropertyValue
	{
	public:
		PropertyValue( )			
			: mReadOnly( false )
			, mGroup( null )
		{

		}

		virtual ~PropertyValue( ){ }


		/** 
			设置或者获取一个属性值
			@param
				Instance 实例
			@param
				PV 属性
			@param
				IsGet 是否为获取属性/设置属性
		*/
		virtual bool AccessValue( RTTIObject* Instance, PropertyValue* PV, bool IsGet ) = 0;

		/** 
			设置一个属性值
			@param
				Instance 实例
			@param
				PV 属性
		*/
		inline bool Set( RTTIObject* Instance, PropertyValue* PV)
		{
			return AccessValue( Instance, PV, false );
		}

		/** 
			获取一个属性值
			@param
				Instance 实例
			@param
				PV 属性
		*/
		inline bool Get( RTTIObject* Instance, PropertyValue* PV)
		{
			return AccessValue( Instance, PV, true );
		}

		/**
			使用属性序列化器序列化属性
			@param
				Instance 实例
			@param
				Ser	属性序列化器
			@return
				给序列化器绑定错误捕捉器后可以获得正确返回值.否则始终为true
		*/
		virtual bool SerializeConfig( RTTIObject* Instance, PropertySerializer& Ser, bool UseGroupAsSection ) = 0;

		/** 
			取得属性名称
		*/
		virtual const wchar* GetName( ){ return mName; }

		/** 
			属性是否为常量
		*/
		virtual bool IsConstant( ) = 0;

		/** 
			属性内容转为字符串
		*/
		bool ToString( RTTIObject* Instance, WString& Value );

		/** 
			解析字符串并设置属性
		*/
		bool Parse( RTTIObject* Instance, const wchar* Value );

		/** 
			是否匹配属性类型
		*/
		bool MatchType( PropertyValue* PV );

		/** 
			设置属性只读
			@return
				便于连缀表达式
		*/
		PropertyValue& SetReadOnly( bool Value ){ mReadOnly = Value; return *this; }

		/** 
			获取属性只读
		*/
		bool GetReadOnly( ){ return mReadOnly; }

		/** 
			取得属性类型名
		*/
		const wchar* GetTypeName( ){ return mTypeName; }

		/// 设置属性组
		PropertyValue& SetGroup( PropertyGroup* Group ){ mGroup = Group;  return *this; }

		/// 获取属性组
		PropertyGroup* GetGroup( ){ return mGroup; }

	protected:
		const wchar* mTypeName;

		const wchar* mName;

		PropertyGroup* mGroup;

		bool mReadOnly;

		friend class PropertyCollection;
		PropertyCollection* mCollection;
	};



	/** 
		值类型基类
	*/
	template<typename TType>
	class TPropertyValueBase : public PropertyValue
	{
	public:
		virtual void OnSetValue( RTTIObject* Instance, const TType& Value ) = 0;

		virtual void OnGetValue( RTTIObject* Instance, TType& Value ) = 0;

		virtual bool SerializeConfig( RTTIObject* Instance, PropertySerializer& Ser, bool UseGroupAsSection )
		{
			if ( UseGroupAsSection && mGroup != null )
			{
				// 切换section
				if ( Ser.GetSection( ) != mGroup->GetName() )
					Ser.SerializeSection( mGroup->GetName() );
			}
			

			if ( Ser.IsLoading() )
			{
				if ( !IsConstant() )
				{
					TType Value;

					// 设置key
					Ser << mName << Value;

					OnSetValue( Instance , Value );
				}
			}
			else
			{
				TType Value;

				OnGetValue( Instance, Value );

				Ser << mName << Value;
			}

			// 如果需要返回值作用,需要为Serializer绑定一个Error对象
			PropertySerializerErrorCatcher* ErrorObj = Ser.GetErrorObject();
			
			if ( ErrorObj == null )
				return true;

			return ErrorObj->GetErrorCount() == 0;
		}

		virtual bool AccessValue( RTTIObject* Instance, PropertyValue* PV, bool IsGet )
		{
			if ( IsGet && PV->IsConstant() )
				return false;

			if ( !IsGet && IsConstant() )
				return false;

			// 从外部字符串类型解析为本类型
			PropertyStringValue* PStr = dynamic_cast<PropertyStringValue*>( PV );
			if ( PStr != null )
			{
				if ( IsGet )
				{
					TType Value;
					OnGetValue( Instance, Value );

					PStr->mStringRef = value_tostringW( Value  );
				}
				else
				{
					TType NewValue;
					if ( !value_parse( PStr->GetString(), NewValue ) )
						return false;

					OnSetValue( Instance, NewValue );

				}
				
				return true;
			}


			Assert( PV->MatchType( this ) );
			if ( !PV->MatchType( this ) )
				return false;

			// 赋值给外部值类型/ 从外部值类型设置
			TPropertyValueRef<TType>* PVRef = dynamic_cast<TPropertyValueRef<TType>*>( PV );
			if ( PVRef != null )
			{
				if ( IsGet )
				{
					TType Value;
					OnGetValue( Instance, Value );

					*PVRef->mValueRef = Value;
				}
				else
				{
					OnSetValue( Instance , *PVRef->mValueRef );
				}
				
				return true;
			}

			return false;
		}
	};

	/** 
		类成员属性
	*/
	template<typename TType>
	class TPropertyMember : public TPropertyValueBase<TType>
	{
	public:
		TPropertyMember( const wchar* TypeName, const wchar* Name, dword Offset, dword Size )
		{
			mOffset		= Offset;
			mSize		= Size;
			mTypeName	= TypeName;
			mName		= Name;
		}

		virtual const wchar* GetName( ){ return mName; }

		// 获得某个实例的属性指针
		TType* GetPropertyAddress( RTTIObject* Instance )
		{
			return (TType*)(dword(Instance) + mOffset);
		}

		virtual bool IsConstant( ){ return false; }


		virtual void OnSetValue( RTTIObject* Instance, const TType& Value )
		{
			*GetPropertyAddress( Instance ) = Value;
		}

		virtual void OnGetValue( RTTIObject* Instance, TType& Value )
		{
			Value = *GetPropertyAddress( Instance );
		}

	private:
		dword	mOffset;
		dword	mSize;
	};

	extern WString GPropertyNullString;

	/** 
		字符串变量,用于ToString及Parse
	*/
	class PropertyStringValue : public PropertyValue
	{
	public:
		PropertyStringValue( WString& Value )
			: mStringRef( Value )
			, mConstString( null )
		{
			mTypeName = L"#RefString";
			mName = L"#RefString";
		}

		PropertyStringValue( const wchar* Value )
			: mStringRef( GPropertyNullString )
			, mConstString( Value )
		{
			mTypeName = L"#ConstString";
			mName = L"#ConstString";
		}

		virtual bool IsConstant( ){ return mConstString != null; }

		virtual bool SerializeConfig( RTTIObject* Instance, PropertySerializer& Ser, bool UseGroupAsSection )
		{
			if ( UseGroupAsSection && mGroup != null )
			{
				// 切换section
				if ( Ser.GetSection( ) != mGroup->GetName() )
					Ser.SerializeSection( mGroup->GetName() );
			}


			if ( Ser.IsLoading() )
			{
				if ( IsConstant() )
					return false;
			
				// 设置key
				Ser << mName << mStringRef;
			}
			else
			{								
				Ser << mName << GetString();
			}

			// 如果需要返回值作用,需要为Serializer绑定一个Error对象
			PropertySerializerErrorCatcher* ErrorObj = Ser.GetErrorObject();

			if ( ErrorObj == null )
				return true;

			return ErrorObj->GetErrorCount() == 0;
		}


		virtual bool AccessValue( RTTIObject* Instance, PropertyValue* PV, bool IsGet )
		{
			if ( IsGet && PV->IsConstant() )
				return false;

			if ( !IsGet && IsConstant() )
				return false;

			// 必须为字符串类型
			Assert( PV->MatchType( this ) );
			if ( !PV->MatchType( this ) )
				return false;

			// 字符串类型
			PropertyStringValue* PStr = dynamic_cast<PropertyStringValue*>( PV );
			if ( PStr != null )
			{
				if ( IsGet )
				{
					if ( IsConstant() )
						PStr->mStringRef = mConstString;
					else
						PStr->mStringRef = mStringRef;
				}
				else
				{
					mStringRef = PStr->GetString();
				}
				

				return true;
			}

			return false;
		}

		const wchar* GetString( )
		{
			if ( mConstString )
				return mConstString;

			return mStringRef.c_str();
		}

	public:
		WString& mStringRef;
		const wchar* mConstString;		
	};


	/** 
		引用类型携带信息
	*/
	template<typename TType>
	class TPropertyValueRef : public TPropertyValueBase<TType>
	{
	public:
		// 引用类型
		TPropertyValueRef( TType& Value, const wchar* TypeName )
			: mValueRef( &Value )
			, mConstant( false )
		{
			mTypeName = TypeName;
			mName = L"#Value";
		}

		// 常量引用类型
		TPropertyValueRef( const TType& Value, const wchar* TypeName )
			: mValueRef( (TType*)&Value )
			, mConstant( true )
		{

			mTypeName = TypeName;
		}

		virtual bool IsConstant( ){ return mConstant; }
		
		virtual void OnSetValue( RTTIObject* Instance, const TType& Value )
		{
			*mValueRef = Value;
		}

		virtual void OnGetValue( RTTIObject* Instance, TType& Value )
		{
			Value = *mValueRef;
		}


	public:
		TType* mValueRef;
		bool mConstant;
	};

	
	/** 
		类成员函数类型
	*/
	template<typename TType, typename TClass, typename TFuncType>
	class TPropertyDelegate : public TPropertyValueBase<TType>
	{
	public:
		// 引用类型
		TPropertyDelegate( const wchar* TypeName, const wchar* Name, TFuncType Func )
			: mFunc( Func )
		{
			mTypeName = TypeName;
			mName = Name;
		}



		virtual bool IsConstant( ){ return false; }

		virtual void OnSetValue( RTTIObject* Instance, const TType& Value )
		{			
			TClass* Callee = RTTIObject::Cast<TClass>( Instance );
			Assert( Callee != null );
			if ( Callee == null )
				return;

			( Callee->*mFunc)( (TType*)&Value, false );
		}

		virtual void OnGetValue( RTTIObject* Instance, TType& Value )
		{
			TClass* Callee = RTTIObject::Cast<TClass>( Instance );
			Assert( Callee != null );
			if ( Callee == null )
				return;

			( Callee->*mFunc)( &Value, true );
		}

	private:
		TFuncType mFunc;
	};

	/** 
		回调类型
	*/
	template<typename TType, typename TClass, typename TFuncType >
	class TPropertyCallback : public TPropertyValueBase<TType>
	{
	public:
		// 引用类型
		TPropertyCallback( const wchar* TypeName, const wchar* Name, TFuncType Func )
			: mFunc( Func )
		{
			mTypeName = TypeName;
			mName = Name;
		}

		virtual bool IsConstant( ){ return false; }

		virtual void OnSetValue( RTTIObject* Instance, const TType& Value )
		{			
			TClass* Callee = RTTIObject::Cast<TClass>( Instance );
			
			Assert( Callee != null );
			if ( Callee == null )
				return;

			mFunc( Callee, (TType*)&Value, false );
		}

		virtual void OnGetValue( RTTIObject* Instance, TType& Value )
		{
			TClass* Callee = RTTIObject::Cast<TClass>( Instance );

			mFunc( Callee, &Value, true );
		}

	private:
		TFuncType mFunc;
	};


	/// 值类型
	#define PROPERTY_VALUE(TType, TValue ) TPropertyValueRef<TType>( TValue, UTEXT(#TType) )

	/// 类，结构体的成员变量
	#define PROPERTY_MEMBER_VAR( TClass, TType, TValue) new TPropertyMember<TType>( UTEXT(#TType),UTEXT(#TValue) + 1,StructOffset(TClass,TValue), sizeof(TType) )

	/// 类成员函数
	#define PROPERTY_DELEGATE( TClass, TType, ValueName,Func ) PropertyCollection::NewDelegateProperty<TType>( (TClass*)null, UTEXT(#TType), ValueName, Func )

	/// 静态函数
	#define PROPERTY_CALLBACK( TClass, TType, ValueName,Func ) PropertyCollection::NewCallbackProperty<TType>( (TClass*)null, UTEXT(#TType), ValueName, Func )	

	

	class PropertyActionListener
	{
	public:
		virtual bool OnPropertyGet( PropertyCollection* PC, RTTIObject* Instance, PropertyValue* PV ) = 0; 

		virtual bool OnPropertySet( PropertyCollection* PC, RTTIObject* Instance, PropertyValue* PV ) = 0; 
	};

	class PropertyCollection
	{
	public:
		PropertyCollection( );

		virtual ~PropertyCollection( );

		/// 添加属性信息
		PropertyValue* Add( PropertyValue* Property, PropertyGroup* Group = null );
		
		/// 设置属性值
		bool Set( RTTIObject* Instance, const wchar* Name, PropertyValue& PV );

		/// 获取属性值
		bool Get( RTTIObject* Instance, const wchar* Name, PropertyValue& PV );

		/// 属性值转为字符串
		bool ToString( RTTIObject* Instance, const wchar* Name,WString& Value );

		/// 字符串设置属性值
		bool Parse( RTTIObject* Instance, const wchar* Name,const wchar* Value );

		/// 遍历，第一个属性值
		PropertyValue* GetFirstProperty( );

		/// 遍历，下一个属性值
		PropertyValue* GetNextProperty( );

		/// 查找某个特定属性值
		PropertyValue* Query( const wchar* Name );


		/**
			使用属性序列化器序列化所有的属性
			@param
				Instance 实例
			@param
				Ser	属性序列化器			
		*/
		void SerializeConfig( RTTIObject* Instance, PropertySerializer& Ser );

		/// 携带TClass类型及TFuncType的Dummy函数
		template<typename TType, typename TClass, typename TFuncType >
		static PropertyValue* NewDelegateProperty( TClass* TemplateDummy, const wchar* TypeName, const wchar* Name, TFuncType Func)
		{
			return new TPropertyDelegate<TType,TClass, TFuncType>( TypeName, Name, Func );
		}

		/// 携带TClass类型及TFuncType的Dummy函数
		template<typename TType, typename TClass, typename TFuncType >
		static PropertyValue* NewCallbackProperty( TClass* TemplateDummy, const wchar* TypeName, const wchar* Name, TFuncType Func)
		{
			return new TPropertyCallback<TType,TClass, TFuncType>( TypeName, Name, Func );
		}

		/// 添加一个属性组
		PropertyGroup* AddGroup( const WString& Name );

		/// 遍历属性组
		PropertyGroup* GetGroup( dword Index );

		/// 获取属性组数量
		dword GetGroupCount( ){ return mGroupArray.size(); }
	
		/// 根据属性组名称查询
		PropertyGroup* QueryGroup( const WString& Name );

	private:
		bool DoAction( RTTIObject* Instance, const wchar* Name, PropertyValue* PV, bool IsGet );		
		
	private:
		typedef dynamic_hash<WString, PropertyValue*> PropertyMap;
		PropertyMap mPropertyMap;
		PropertyMap::iterator* mIterator;

		typedef std::vector<PropertyGroup*> GroupArray;
		GroupArray mGroupArray;
	};


	/** @} */
	/** @} */
}

//struct MyClass 
//{
//	bool Value1;
//	Vector3 Value2;
//
//	const Vector3& GetValue2( )
//	{
//		return Value2;
//	}
//
//	void SetValue2( const Vector3& v )
//	{
//		Value2 = v;
//	}
//
//
//	void OnValueChange( Vector3* Value, bool IsGet )
//	{
//		if ( IsGet )
//		{
//			*Value = GetValue2();
//		}
//		else
//		{
//			SetValue2( *Value );
//		}
//
//	}
//
//	static void OnChange( MyClass* Instance, Vector3* Value, bool IsGet )
//	{
//
//	}
//};
//
//MyClass b;
//b.Value1 = false;
//b.Value2 = Vector3( 4,5,6);
//
//PropertyCollection PS;
//PS.Add( CLASS_PROPERTY(MyClass,bool, Value1) );
////PS.Add( CLASS_PROPERTY(MyClass,Vector3, Value2) );
//
//
//PS.Add( CALLBACK_PROPERTY( MyClass, Vector3, L"Value3", &MyClass::OnChange ) );
//PS.Add( DELEGATE_PROPERTY( MyClass, Vector3, L"Value2", &MyClass::OnValueChange ) );
//
//PS.Parse( &b, L"Value3",L"a");
//
//PS.Parse( &b, L"Value1", L"true");
//
//PS.Set( &b, L"Value2", PROPERTY_VALUE( Vector3, Vector3(1,2,3) ));
//
//WString a;
//PS.ToString(&b,L"Value2", a );
//
//Vector3 d;
//PS.Get(&b,L"Value2", PROPERTY_VALUE(Vector3, d ));
//
//
//SimpleLog log;
//for ( PropertyValue* MV = PS.GetFirstProperty();
//	 MV != null;
//	 MV = PS.GetNextProperty() )
//{
//	WString str;
//	MV->ToString( &b, str );
//	log.Debug(L"%s = %s", MV->GetName(),  str.c_str());
//}