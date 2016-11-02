#include "CorePCH.h"
#include "NekoCore_Reflection.h"


namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// RTTIClassInfo
	//////////////////////////////////////////////////////////////////////////

	RTTIClass::RTTIClass( const NameRef& ClassName, 
						ClassConstructor CreateFunc, 
						RTTIClass* ParentClass,
						dword ClassFlag )

		: mClassName( ClassName )
		, mCreateEntry( CreateFunc )				
		, mClassUnique( 0 )
		, mPropertyCollection( null )
		, mParentClass( ParentClass )
		, mChildClass( null )
		, mGroup( null )
		, mClassFlag( ClassFlag )
	{
		// 根节点
		if ( ParentClass == null )
		{
			// 永远没有同级节点
			mNextClass = null;
		}
		else
		{
			// 将老的头作为我的下一级
			mNextClass = ParentClass->mChildClass;

			// 将我设为父亲的第一个子
			ParentClass->mChildClass = this;
		}
		
	}

	RTTIClass::~RTTIClass()
	{
		SafeDelete( mPropertyCollection );
	}


	RTTIObject* RTTIClass::CreateInstance( )
	{
		Assert( mCreateEntry != null );

		return mCreateEntry(  );
	}

	const wchar* RTTIClass::GetRTTIClassString()
	{
		return mClassName.ToStringFast();
	}

	const NameRef& RTTIClass::GetRTTIClassName()
	{
		return mClassName;
	}

	bool RTTIClass::IsA( RTTIClass* ClassInfo )
	{
		return ClassInfo->GetRTTIClassName() == this->GetRTTIClassName();			
	}

	bool RTTIClass::IsA( const NameRef& ClassName )
	{
		return ClassName == this->GetRTTIClassName();			
	}

	PropertyCollection* RTTIClass::GetPropertyCollection()
	{
		if ( mPropertyCollection == null )
			mPropertyCollection = new PropertyCollection;

		return mPropertyCollection;
	}

	bool RTTIClass::IsKindOf( RTTIClass* ClassInfo )
	{
		RTTIClass* ThisClass = this;
		while ( ThisClass != null )
		{
			if ( ClassInfo == ThisClass )
				return true;

			ThisClass = ThisClass->mParentClass;
		}

		return false;
	}

	bool RTTIClass::HasFlag( dword ClassFlag )
	{
		return (mClassFlag & ClassFlag) == ClassFlag;
	}



	//////////////////////////////////////////////////////////////////////////
	// Class Instance Function
	//////////////////////////////////////////////////////////////////////////

	RTTIClass* RTTIObject::sRTTIInfo = null;
	RTTIClass* RTTIObject::StaticClass( )
	{
		if ( sRTTIInfo ) 
			return  sRTTIInfo;

		sRTTIInfo = new RTTIClass( UTEXT("RTTIObject"), RTTIObject::StaticConstruct, null, ClassFlag_Intrinsic );

		RTTIObject::Register( sRTTIInfo );

		return sRTTIInfo;
	}


	bool RTTIObject::IsA( RTTIClass* rtticlass )
	{
		return rtticlass == GetClass();
	}

	bool RTTIObject::EqualTo( RTTIObject* rttiobj )
	{
		return GetClass() == rttiobj->GetClass();
	}

	bool RTTIObject::IsA( const NameRef& rttitype )
	{
		return GetClass()->mClassName == rttitype;
	}


	//////////////////////////////////////////////////////////////////////////
	// RTTI Object Management
	//////////////////////////////////////////////////////////////////////////

	RTTIObject::ClassInfoMap RTTIObject::mClassInfoMap;
	RTTIObject::ClassInfoMap::iterator RTTIObject::mIterator;
	RTTIObject::GroupMap RTTIObject::mGroupMap;

	void RTTIObject::StaticInit( )
	{
		RTTI_REGISTER( RTTIObject );
	}


	void RTTIObject::StaticExit()
	{
		ClearClass( );

		ClearGroup( );
	}



	RTTIObject* RTTIObject::CreateInstance( const wchar* ClassName )
	{
		return CreateInstance( NameRef( ClassName, NameBuild_Find) );
	}

	RTTIObject* RTTIObject::CreateInstance( const NameRef& ClassName )
	{
		RTTIClass* ClassInfo = FindClass( ClassName );

		if ( ClassInfo == null )
			return null;

		return ClassInfo->CreateInstance( );
	}



	void RTTIObject::Register( RTTIClass* ClassInfo)
	{
		// 在table中找已经存在的
		Assert( FindClass( ClassInfo->mClassName ) == null && L"Duplicate Class Name" )


		mClassInfoMap.insert( ClassInfo, ClassInfo );
	}

	void RTTIObject::UnRegister( RTTIClass* ClassInfo )
	{
		// 从本Package删除
		mClassInfoMap.remove( ClassInfo );

		// 释放内存
		delete ClassInfo;
	}

	RTTIClass* RTTIObject::FindClass( const NameRef& ClassName )
	{
		RTTIClass Finder( ClassName, null, null, 0 );
		RTTIClass** Result = mClassInfoMap.find( &Finder );
		if ( Result == null )
			return null;

		return *Result;
	}

	void RTTIObject::ClearClass()
	{
		ClassInfoMap::iterator It = mClassInfoMap.begin();
		while( It.valid() )
		{
			RTTIClass* ClassInfo = It;
			It = mClassInfoMap.remove( ClassInfo );
			delete ClassInfo;
		}

		mClassInfoMap.clear();
	}

	RTTIClass* RTTIObject::GetFirstClass()
	{
		mIterator = mClassInfoMap.begin();

		return GetNextClass();
	}

	RTTIClass* RTTIObject::GetNextClass()
	{
		if ( mIterator == mClassInfoMap.end( ) )
			return null;

		RTTIClass* ClassInfo = mIterator;
		++mIterator;

		return ClassInfo;
	}


	dword RTTIObject::GetClassCount()
	{
		return mClassInfoMap.size();
	}


	void RTTIObject::Dump( Logger* logger )
	{
		//logger->Info( L"=========RTTIObject Dump=========");
		//logger->Info( L"GroupName	ConfigName	ClassName");
		//RTTIClassInfo* p = GRTTIClassInfoHeader;
		//StringConverter conv;
		//while( p )
		//{
		//	logger->Info( L"%s			%s", 
		//					conv.AnsiToUnicode(p->mGroupName.GetStringA()),
		//					conv.AnsiToUnicode(p->mClassName.GetStringA())
		//					);
		//	
		//	p = p->mListNext;
		//}

		//logger->Info( L"=========RTTIObject Dump=========");
	}

	bool RTTIObject::IsKindOf( RTTIClass* ClassInfo )
	{
		RTTIClass* ThisClass = GetClass();

		if ( ThisClass == null )
			return false;
		
		return ThisClass->IsKindOf( ClassInfo );
	}

	RTTIGroup* RTTIObject::GetGroup( const NameRef& Name )
	{
		GroupMap::iterator it = mGroupMap.find( Name );
		if ( it != mGroupMap.end() )
			return it->second;

		RTTIGroup* NewGroup = new RTTIGroup;
		mGroupMap[ Name ] = NewGroup;

		return NewGroup;
	}

	void RTTIObject::ClearGroup( )
	{
		for ( GroupMap::iterator it = mGroupMap.begin();
			it != mGroupMap.end();
			++it )
		{
			RTTIGroup* Group = it->second;
			delete Group;
		}

		mGroupMap.clear();
	}

	void RTTIObject::BindGroup( RTTIClass* ClassInfo, RTTIGroup* Group )
	{
		Assert( ClassInfo != null && Group != null );

		ClassInfo->mGroup = Group;
		Group->AddClass( ClassInfo );
	}

	bool RTTIObject::SeekAccessObjectProperty( const wchar* Name, PropertyValue* PV, bool IsGet, bool SeekWholeFamily )
	{
		for ( RTTIClass* ClassInfo = GetClass(); 
			ClassInfo != null; 
			ClassInfo = ClassInfo->mParentClass)
		{
			PropertyCollection* PC = ClassInfo->GetPropertyCollection();

			PropertyValue* TargetValue = PC->Query( Name );
			if ( TargetValue != null )
			{
				return TargetValue->AccessValue( this, PV, IsGet );
			}

			if ( !SeekWholeFamily )
				break;
		}

		return false;
	}



	bool RTTIObject::SetObjectPropertyByString( const wchar* Name, const WString& Value, bool SeekWholeFamily )
	{
		return SeekAccessObjectProperty( Name, &PropertyStringValue( Value.c_str() ), false,  SeekWholeFamily );
	}


	bool RTTIObject::GetObjectPropertyAsString( const wchar* Name, WString& Value, bool SeekWholeFamily )
	{
		return SeekAccessObjectProperty( Name, &PropertyStringValue( Value ), true, SeekWholeFamily );
	}


	bool RTTIObject::SetObjectProperty( const wchar* Name, PropertyValue& PV, bool SeekWholeFamily )
	{
		return SeekAccessObjectProperty( Name, &PV, false, SeekWholeFamily );
	}

	bool RTTIObject::GetObjectProperty( const wchar* Name, PropertyValue& PV, bool SeekWholeFamily  )
	{
		return SeekAccessObjectProperty( Name, &PV, true, SeekWholeFamily );
	}

	void RTTIObject::SerializeConfig( PropertySerializer& Ser )
	{
		PropertyCollection* PC = GetClass()->GetPropertyCollection( );
		if ( PC == null )
			return;

		PC->SerializeConfig( this, Ser );
	}



	BinarySerializer& operator<<( BinarySerializer& Ser, RTTIObject& Obj )
	{
		Assert( Ser.IsSaving() ) 
			Ser.WriteObject( &Obj ); 
		Obj.Serialize( Ser );
		return Ser;
	}

	BinarySerializer& operator<<( BinarySerializer& Ser, RTTIObject*& Obj )
	{
		return Ser.SerializeObject( Obj );
	}


	//////////////////////////////////////////////////////////////////////////
	// RTTIGroup
	//////////////////////////////////////////////////////////////////////////
	RTTIGroup::RTTIGroup()
		: mIterator( 0 )
	{

	}

	RTTIClass* RTTIGroup::GetFirstClass()
	{
		mIterator = 0;

		return GetNextClass();
	}

	RTTIClass* RTTIGroup::GetNextClass()
	{
		if ( mIterator >= mClassArray.size() )
			return null;

		return mClassArray[ mIterator++ ];
	}

	NekoEngine::dword RTTIGroup::GetClassCount()
	{
		return mClassArray.size();
	}

	bool RTTIGroup::ClassExist( RTTIClass* ClassInfo )
	{
		ClassArray::iterator it = std::find( mClassArray.begin(), mClassArray.end(), ClassInfo );

		return it != mClassArray.end();
	}

	void RTTIGroup::AddClass( RTTIClass* ClassInfo )
	{
		Assert( ClassExist( ClassInfo ) == false && "Class already exist in the group" );

		mClassArray.push_back( ClassInfo );
	}




}

