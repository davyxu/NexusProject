#include "CorePCH.h"
#include "NekoCore_Stream.h"
#include "NekoCore_Reflection.h"



namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// PropertySerializerError
	//////////////////////////////////////////////////////////////////////////


	PropertySerializerErrorCatcher::PropertySerializerErrorCatcher( PropertySerializer& Ser )
		: mSer( Ser )
	{
		mSer.mErrorObjectStack.push_back( this );
	}

	PropertySerializerErrorCatcher::~PropertySerializerErrorCatcher()
	{
		mSer.mErrorObjectStack.pop_back();
	}

	void PropertySerializerErrorCatcher::WriteError( const WString& Section, const WString& Key )
	{
		mErrorArray.push_back( ErrorDesc( Section, Key ) );
	}

	bool PropertySerializerErrorCatcher::GetErrorDesc( dword Index, WString& ErrorSec, WString& ErrorKey )
	{
		if ( Index >= mErrorArray.size() )
			return false;

		ErrorDesc& Desc = mErrorArray[Index];

		ErrorSec = Desc.mSection;
		ErrorKey = Desc.mKey;

		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	// PropertySerializer
	//////////////////////////////////////////////////////////////////////////

	PropertySerializer::PropertySerializer( const wchar* FileName, bool IsLoading )
		: mIniFile( new IniFile )
		, mSectionData( null )		
		, mSaveFileName( null )
	{
		if ( IsLoading )
		{
			mIniFile->Parse( FileName );
		}
		else
		{
			mSaveFileName = new WString( FileName );
		}
	}

	PropertySerializer::~PropertySerializer()
	{
		if ( mSaveFileName != null )
		{
			mIniFile->Dump( mSaveFileName->c_str() );
			delete mSaveFileName;
		}

		delete mIniFile;
	}

	PropertySerializerErrorCatcher* PropertySerializer::GetErrorObject( )
	{
		if ( mErrorObjectStack.size() == 0)
			return NULL;

		return mErrorObjectStack.back();
	}


	bool PropertySerializer::SerializeSection( const WString& Name )
	{
		if ( IsSaving() )
		{
			mSectionData = mIniFile->AddSection( Name );
		}
		else
		{
			mSectionData = mIniFile->QuerySection( Name );
		}		

		return mSectionData != null;
	}

	bool PropertySerializer::IsLoading()
	{
		return mSaveFileName == null;
	}

	bool PropertySerializer::IsSaving()
	{
		return mSaveFileName != null;
	}

	const WString& PropertySerializer::GetSection()
	{
		if ( mSectionData == null )
			return StringHelper::cBlank;

		return mSectionData->GetName();
	}


}
