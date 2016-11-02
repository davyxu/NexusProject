#pragma once

using namespace System;

using namespace System::Collections;

namespace Wrapper
{
	public ref class Vector3
	{
	public:
		float X;
		float Y;
		float Z;

		Vector3( float X ,float Y, float Z )
		{
			this->X = X;
			this->Y = Y;
			this->Z = Z;
		}

		virtual String^ ToString( ) override 
		{
			return String::Format("{0}, {1}, {2}",X, Y, Z);
		}
	};

	generic<typename TClass>
	public ref class Singleton
	{
	public:
		static TClass Instance( )
		{
			if ( mInstance == nullptr )
			{
				mInstance = Activator::CreateInstance<TClass>();
			}
			return mInstance;
		}

	private:
		static TClass mInstance;
	};

	generic<typename TClass>
	public ref class ManualSingleton
	{
	public:
		ManualSingleton( )
		{
			mInstance = (TClass)this;
		}

		static TClass Instance( )
		{
			return mInstance;
		}

	private:
		static TClass mInstance;
	};


	public ref class Utility : public Singleton<Utility^>
	{
	public:
		Utility( );

		static const wchar_t* ToCharPtr( String^ Str );

		Type^ ToManagedType( String^ TypeName );

	private:
		Hashtable^ mTypeMapping;
	};
}