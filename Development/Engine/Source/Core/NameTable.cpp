#include "CorePCH.h"
#include "NekoCore_Reflection.h"

namespace NekoEngine
{
	NameDataArray NameRef::GNameRefArray(NAMECONST_RESERVED_COUNT);
	NameData* NameRef::GNameDataHash[] = { 0 };
	bool NameRef::GNameTableInitialized = false;

	static NameData* AllocNameData( const wchar* name, dword hashcode )
	{
		dword buffersize = sizeof(NameData) -  ( NameData::NAME_SIZE - StringHelper::Length( name ) - 1 ) * sizeof( wchar );
		NameData* newname = (NameData*)malloc( buffersize );

		if ( newname == null )
			return null;

		StringHelper::Copy( newname->mName, name );
		newname->mHashCode = hashcode;

		return newname;
	}

	//////////////////////////////////////////////////////////////////////////
	// NameRef
	//////////////////////////////////////////////////////////////////////////
	NameRef::NameRef( dword c ) 
		: mIndex( c )
		, mNumber( 0 )
		, mContent( L"" )
	{
		RefreshDebugView( );
	}

	NameRef::NameRef()
		: mContent(L"")
		, mIndex( NameConst_None )
		, mNumber( 0 )
	{

	}

	NameRef::NameRef( dword Index, dword Number )
		: mIndex( Index )
		, mNumber( Number )
		, mContent( L"" )
	{

	}

	bool SplitNameWithCheck(const wchar* OldName, wchar* NewName, dword NewNameLen, dword& NewNumber)
	{
		bool bSucceeded = FALSE;

		// get string length
		const wchar* LastChar = OldName + (StringHelper::Length(OldName) - 1);

		// if the last char is a number, then we will try to split
		const wchar* Ch = LastChar;
		if (*Ch >= '0' && *Ch <= '9')
		{
			// go backwards, looking an underscore or the start of the string
			// (we don't look at first char because '_9' won't split well)
			while (*Ch >= '0' && *Ch <= '9' && Ch > OldName)
			{
				Ch--;
			}

			// if the first non-number was an underscore (as opposed to a letter),
			// we can split
			if (*Ch == '_')
			{
				// check for the case where there are multiple digits after the _ and the first one
				// is a 0 ("Rocket_04"). Can't split this case. (So, we check if the first char
				// is not 0 or the length of the number is 1 (since ROcket_0 is valid)
				if (Ch[1] != '0' || LastChar - Ch == 1)
				{
					// attempt to convert what's following it to a number
					if ( !value_parse( Ch + 1, NewNumber ) )
						NewNumber = 0;

					// copy the name portion into the buffer
					StringHelper::Copy(NewName, OldName, Math::Min((dword)(Ch - OldName ), NewNameLen));

					// mark successful
					bSucceeded = true;
				}
			}
		}

		// return success code
		return bSucceeded;
	}



	// TODO: 自动能将Name中例如：Rock#12解析为Name = Rock, Number = 12
	NameRef::NameRef( const wchar* Name, NameBuildType BuildType, dword Number, bool SpliteName )
		: mIndex( NameConst_None )
		, mNumber( 0 )
		, mContent( L"" )
	{
		Assert( GNameTableInitialized );

		if ( !GNameTableInitialized )
		{
			mIndex = NameConst_None;
			return;
		}

		wchar SpliteBuffer[NameData::NAME_SIZE];

		dword SpliteNumber;

		if ( Number == NAME_NO_NUMBER && SpliteName )
		{
			if ( SplitNameWithCheck( Name, SpliteBuffer, ArraySize(SpliteBuffer), SpliteNumber ) )
			{
				Name = SpliteBuffer;
				Number = SpliteNumber;
			}
		}

		Assert(Name);

		// ParseName时有错误
		if( !Name[0] )
		{
			mIndex = NameConst_None;
			Number = NAME_NO_NUMBER;
			return;
		}

		dword hashcode = CheckSum::StrHashNoCase( Name );

		dword hashindex = GenHashIndex( hashcode );

		mNumber = Number;

		// 在table中找已经存在的
		for ( NameData* hash = GNameDataHash[hashindex]; hash != null; hash = hash->mNext )
		{
			if ( StringHelper::CompareLowerCase(Name, hash->mName ) )
			{
				this->mIndex = hash->mIndex;
				RefreshDebugView( );
				return;
			}
		}

		if ( BuildType == NameBuild_Find )
		{
			mIndex = NameConst_None;
			return;
		}

		NameData* newname = AllocNameData( Name, hashcode );

		if ( newname == null )
		{
			mIndex = NameConst_None;
			return;
		}

		newname->mNext = GNameDataHash[ hashindex ];
		GNameDataHash[ hashindex ] = newname;

		newname->mIndex = mIndex = GNameRefArray.size() + 1;

		GNameRefArray.push_back( newname );

		RefreshDebugView( );
	}

	void NameRef::RefreshDebugView( )
	{
		if ( IsValid()  )
		{
			mContent = ToStringFast();
		}

	}

	NameRef& NameRef::operator = ( const NameRef& Other )
	{
		mIndex = Other.mIndex;
		mNumber = Other.mNumber;

		RefreshDebugView( );

		return *this;
	}

	bool NameRef::IsValid() const
	{
		return mIndex>0 && (mIndex - 1)<GNameRefArray.size() && GNameRefArray[mIndex - 1]!=NULL;
	}

	dword NameRef::GetIndex() const
	{
		return mIndex;
	}

	dword NameRef::GetNumber( ) const
	{
		return mNumber;
	}

	dword NameRef::GetHash() const
	{
		Assert( IsValid() );

		return GNameRefArray[ mIndex - 1 ]->mHashCode;
	}

	const wchar* NameRef::ToStringFast() const
	{
		if ( !IsValid() )
			return L"";
		//Assert( mNumber == 0 && "Number used in NameRef, Use WString ToString( ) to get full name")

		return GNameRefArray[ mIndex - 1 ]->mName;
	}

	WString NameRef::ToStringSafe( ) const
	{
		if ( !IsValid() )
			return L"";

		if ( mNumber == NAME_NO_NUMBER )
			return GNameRefArray[ mIndex - 1 ]->mName;

		return StringHelper::FormatString(L"%s_%d", GNameRefArray[ mIndex - 1 ]->mName, mNumber );
	}


	dword NameRef::GenHashIndex( dword hashcode )
	{
		return hashcode & (ArraySize(GNameDataHash) - 1 );
	}






	//////////////////////////////////////////////////////////////////////////
	// Name Table Management
	//////////////////////////////////////////////////////////////////////////

	bool NameRef::AddNameConst( const wchar* name, dword index  )
	{
		Assert( GNameTableInitialized );

		if ( !GNameTableInitialized )
		{
			return false;
		}

		Assert( index < NAMECONST_RESERVED_COUNT &&
			"Name Const over the const range!" );

		dword hashcode = CheckSum::StrHashNoCase( (const wchar*)name );

		dword hashindex = GenHashIndex( hashcode );

		NameData* hash = GNameDataHash[hashindex];

		for (  ;hash != null; hash = hash->mNext )
		{
			bool match = StringHelper::CompareLowerCase(name, hash->mName );


			// 匹配，并且这个名字被用于非Const，得把他纠正为Const
			if ( match && hash->mIndex > NAMECONST_RESERVED_COUNT)
			{
				hash->mIndex = index;
				Assert("Warning! The name is used as none-const, but now it defined as const!\r\n"
					"NameRef can solve it, but we recommand you to change name defination!" == 0);

				return false;
			}
		}


		if ( hash == null )
		{
			hash = AllocNameData( name, hashcode );
		}

		// 还是空，看来是错大了
		if ( hash == null )
			return false;

		// 冲突链接
		hash->mNext = GNameDataHash[ hashindex ];
		GNameDataHash[ hashindex ] = hash;

		hash->mIndex = index;

		
		if ( GNameRefArray[index - 1] != null )
		{
			Assert( "NameRef Const Index Duplicated!" );

			return false;
		}

		GNameRefArray[index - 1] = hash;

		return true;
	}


	void NameRef::StaticInit( )
	{
		if ( GNameTableInitialized )
			return;

		::memset( GNameDataHash, 0, sizeof GNameDataHash );


		GNameTableInitialized = true;
	}

	void NameRef::StaticExit( )
	{
		for ( size_t i = 0;i< GNameRefArray.size();i++)
		{
			free( GNameRefArray[ i ] );
			GNameRefArray[ i ] = null;
		}

		GNameTableInitialized = false;
	}

	bool NameRef::CheckNameTable( Logger* logger )
	{
		
		// Verify no duplicate names.
		for( int i=0; i<ArraySize(GNameDataHash); i++ )
			for( NameData* Hash=GNameDataHash[i]; Hash; Hash=Hash->mNext )
				for( NameData* Other=Hash->mNext; Other; Other=Other->mNext )
				{
					if( StringHelper::CompareLowerCase( (const wchar*)Hash->mName,(const wchar*)Other->mName)==0 )
					{
						if ( logger != null )
							logger->Fatal( L"Global Name Const'%s' was duplicated", Hash->mName );

						return false;
					}
				}

		return true;
	}

	void NameRef::Dump( Logger* logger )
	{
		logger->Info( L"=========NameRef Dump=========");
		logger->Info( L"Index	HashIndex	Name");

		StringConverter conv;

		for (NameDataArray::iterator it = GNameRefArray.begin();
			it != GNameRefArray.end();
			++it )
		{
			NameData* name = *it;
			if ( name == null )
				continue;

			logger->Info( L"%d	0x%x	%s", name->mIndex, 
										 GenHashIndex( name->mHashCode ),
										 name->mName );
				
		}

		logger->Info( L"=========NameRef Dump=========");
	}

}