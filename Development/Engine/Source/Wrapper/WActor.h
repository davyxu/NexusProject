#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace NekoSharp::BoostSharp;

namespace Wrapper
{
#pragma warning( disable:4490 )

	public value class PropertyReflectorKey : System::IEquatable<PropertyReflectorKey>
	{
	public:
		PropertyReflectorKey( NekoEngine::RTTIClass* ClassInfo );
		virtual int GetHashCode() override;
		virtual bool Equals( System::Object^ value ) override;
		virtual bool Equals( PropertyReflectorKey value ) override;
		virtual bool Equals( PropertyReflectorKey% value1, PropertyReflectorKey% value2 ) override;

	internal:
		NekoEngine::RTTIClass* mClassInfo;
	};


	public ref class PropertyReflector
	{
	public:
		PropertyReflector( NekoEngine::RTTIClass* ClassInfo );

		void AttachInstance( void* Instance );

	private:
		void OnGetValue(System::Object^ sender, PropertySpecEventArgs^ e);

		void OnSetValue(System::Object^ sender, PropertySpecEventArgs^ e);

		
	public:
		PropertyBag^ mBag;

	internal:
		NekoEngine::RTTIClass* mClassInfo;
		void* mInstance;
	};

	public ref class ReflectObject
	{
	public:
		ReflectObject( NekoEngine::RTTIClass* ClassInfo );

		virtual void* GetInstance( ){ return 0; }


		PropertyReflector^ mReflector;
	};


	public ref class ReflectorManager : public ManualSingleton<ReflectorManager^>
	{
	public:
		ReflectorManager( );

		PropertyReflector^ GetReflector( NekoEngine::RTTIClass* ClassInfo );

		System::Object^ GetPropertyObject( ReflectObject^ RefObj );
		

	private:
		Hashtable^ mReflectorMap;
	};



	public ref class Actor : public ReflectObject
	{
	public:
		Actor( );

		virtual void* GetInstance( ) override;
	internal:
		NekoEngine::Actor* mActor;
		
	};


}