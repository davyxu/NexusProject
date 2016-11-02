#include "CorePCH.h"
#include "NekoCore_Reflection.h"
#include "NekoCore_Stream.h"


namespace NekoEngine
{
	Logger* BinarySerializer::mLogger = null;

	// Neko Engine Persistence File
	const dword NEKO_ENGINE_PERSISTENCE_FILE = ENGINE_FOURCC( 'N','E','P','F');

	const dword PERSISTENCE_VERSION = 2;// 序列文件头版本

	void BinarySerializer::StaticInit()
	{
		mLogger = new SimpleLog;
		mLogger->SetPrefix(L"Archive");
	}

	void BinarySerializer::StaticExit()
	{
		delete mLogger;
	}


	BinarySerializer::BinarySerializer( DataStream* Stream, bool IsLoading )
		: mIsLoading( IsLoading )
		, mStream( Stream )
		, mVersion( 0 )
		, mStreamOK( false )
	{
		
	}

	void BinarySerializer::SerializerFileHeader( dword  Version )
	{
		mVersion = Version;

		if ( IsLoading() )
		{
			dword Tag = 0;
			dword PersistenceVersion = 0;
			*this << Tag << PersistenceVersion;

			if ( Tag != NEKO_ENGINE_PERSISTENCE_FILE )
			{
				mLogger->Error(L"Invalid Stream Content, format unknown" );
				return;
			}

			if ( PersistenceVersion !=  PERSISTENCE_VERSION )
			{
				mLogger->Error(L"Stream content's persistence %d is not support by system version %d", PersistenceVersion, PERSISTENCE_VERSION );
				return;
			}
		}
		else
		{
			*this << NEKO_ENGINE_PERSISTENCE_FILE << PERSISTENCE_VERSION;
		}

		*this << mVersion;
		mStreamOK = true;
	}

	
	void BinarySerializer::WriteObject( RTTIObject* Obj )
	{
		Assert( IsSaving() );
		Assert( mStreamOK && "SerializerHeader not call?" );

		*this << RTTI_INSTANCE_STRING( Obj );
	}


	RTTIObject* BinarySerializer::ReadObject(  )
	{
		Assert( IsLoading() )
		Assert( mStreamOK && "SerializerHeader not call?" );

		WString ClassString;

		*this << ClassString;

		NameRef ClassNameRef( ClassString.c_str(), NameBuild_Find );

		RTTIClass* ClassInfo = RTTIObject::FindClass( ClassNameRef );
		if ( ClassInfo == null )
		{				
			mLogger->Error(L"StreamObject %s not found in RTTI System, Class not registered?",
				ClassString.c_str()
				);
			return null;
		}

		// 创建实例
		RTTIObject* NewObject = ClassInfo->CreateInstance();
		if ( NewObject == null )
		{
			mLogger->Error(L"ReadObject: RTTIObject::CreateInstance failed ClassName:%s",
				ClassString.c_str() );
			return null;
		}

		return NewObject;
	}

	DataStream* BinarySerializer::GetStream()
	{
		return mStream;
	}






}