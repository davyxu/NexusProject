#include "UnitTestCorePCH.h"

struct MyClass : public RTTIObject
{
	bool Value1;
	Vector3 Value2;

	const Vector3& GetValue2( )
	{
		return Value2;
	}

	void SetValue2( const Vector3& v )
	{
		Value2 = v;
	}


	void OnValueChange( Vector3* Value, bool IsGet )
	{
		if ( IsGet )
		{
			*Value = GetValue2();
		}
		else
		{
			SetValue2( *Value );
		}

	}

	static void OnChange( MyClass* Instance, Vector3* Value, bool IsGet )
	{

	}
};


void PropertyTest( )
{
	MyClass b;
	b.Value1 = false;
	b.Value2 = Vector3( 4,5,6);

	PropertyCollection PS;
	PS.Add( PROPERTY_MEMBER_VAR(MyClass, bool, Value1) );
	//PS.Add( CLASS_PROPERTY(MyClass,Vector3, Value2) );


	PS.Add( PROPERTY_CALLBACK( MyClass, Vector3, L"Value3", &MyClass::OnChange ) );
	PS.Add( PROPERTY_DELEGATE( MyClass, Vector3, L"Value2", &MyClass::OnValueChange ) );

	PS.Parse( &b, L"Value3",L"a");

	PS.Parse( &b, L"Value1", L"true");

	PS.Set( &b, L"Value2", PROPERTY_VALUE( Vector3, Vector3(1,2,3) ));

	WString a;
	PS.ToString(&b,L"Value2", a );

	Vector3 d;
	PS.Get(&b,L"Value2", PROPERTY_VALUE(Vector3, d ));


	SimpleLog log;
	for ( PropertyValue* MV = PS.GetFirstProperty();
		MV != null;
		MV = PS.GetNextProperty() )
	{
		WString str;
		MV->ToString( &b, str );
		log.Debug(L"%s = %s", MV->GetName(),  str.c_str());
	}
}

