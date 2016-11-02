#include "WrapperPCH.h"
//#include <vcclr.h>
#include "PrivateInclude.h"

namespace Wrapper
{

	PropertyReflectorKey::PropertyReflectorKey( NekoEngine::RTTIClass* ClassInfo )
		: mClassInfo( ClassInfo )
	{

	}
	int PropertyReflectorKey::GetHashCode()
	{
		return int( mClassInfo );
	}

	bool PropertyReflectorKey::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<PropertyReflectorKey>( value ) );
	}

	bool PropertyReflectorKey::Equals( PropertyReflectorKey value )
	{
		return ( mClassInfo == value.mClassInfo );
	}

	bool PropertyReflectorKey::Equals( PropertyReflectorKey% value1, PropertyReflectorKey% value2 )
	{
		return ( value1.mClassInfo == value2.mClassInfo );
	}

	PropertyReflector::PropertyReflector( NekoEngine::RTTIClass* ClassInfo )
		: mClassInfo( ClassInfo )
	{
		mBag = gcnew PropertyBag();

		mBag->GetValue += gcnew PropertySpecEventHandler(this,&PropertyReflector::OnGetValue);
		mBag->SetValue += gcnew PropertySpecEventHandler(this,&PropertyReflector::OnSetValue);

		Utility^ GUtility = Utility::Instance( );

		NekoEngine::PropertyCollection* PC = ClassInfo->GetPropertyCollection();
		for (	NekoEngine::PropertyValue* PV = PC->GetFirstProperty(); 
				PV != null;
				PV = PC->GetNextProperty() )
		{
			mBag->Properties->Add( gcnew CustomProperty( gcnew String(PV->GetName()), GUtility->ToManagedType( gcnew String( PV->mTypeName ) ) ) );
		}
	}

	void PropertyReflector::OnGetValue(System::Object^ sender, PropertySpecEventArgs^ e)
	{
		NekoEngine::PropertyCollection* PC = mClassInfo->GetPropertyCollection();

		const NekoEngine::wchar* ValueName = Utility::ToCharPtr( e->Property->Name );

		if ( e->Property->TypeName->IndexOf("Vector3") != 0)
		{
			NekoEngine::Vector3 d;
			PC->Get( mInstance, ValueName, NekoEngine::TPropertyValueRef<NekoEngine::Vector3>( d, L"Vector3" ) );
			e->Value = gcnew Vector3( d.x, d.y, d.z );
		}
		else
		{
			e->Value = false;
		}


		NekoEngine::WString str;
		PC->ToString( mInstance, Utility::ToCharPtr(e->Property->Name), str );
	}

	void PropertyReflector::OnSetValue(System::Object^ sender, PropertySpecEventArgs^ e)
	{
		const NekoEngine::wchar* ValueResult = Utility::ToCharPtr( e->Value->ToString() );

		NekoEngine::PropertyCollection* PC = mClassInfo->GetPropertyCollection();
		PC->Parse( mInstance, Utility::ToCharPtr( e->Property->Name ), ValueResult );
	}

	void PropertyReflector::AttachInstance( void* Instance )
	{
		mInstance = Instance;
	}

	ReflectObject::ReflectObject( NekoEngine::RTTIClass* ClassInfo )
	{
		mReflector = ReflectorManager::Instance()->GetReflector( ClassInfo  );
	}


	Actor::Actor()
		: ReflectObject( RTTI_CLASS_INFO( NekoEngine::Actor ) )
	{
		
	}

	void* Actor::GetInstance()
	{
		return mActor;
	}

	ReflectorManager::ReflectorManager()
	{
		mReflectorMap = gcnew Hashtable();
	}

	PropertyReflector^ ReflectorManager::GetReflector( NekoEngine::RTTIClass* ClassInfo )
	{

		PropertyReflectorKey Key( ClassInfo );
		if ( mReflectorMap->Contains( Key )	 )
			return (PropertyReflector^)mReflectorMap[Key];

		PropertyReflector^ NewValue = gcnew PropertyReflector( ClassInfo );
		mReflectorMap[Key] = NewValue;

		return NewValue;
	}

	System::Object^ ReflectorManager::GetPropertyObject( ReflectObject^ RefObj )
	{
		RefObj->mReflector->AttachInstance( RefObj->GetInstance() );

		return RefObj->mReflector->mBag;
	}



}