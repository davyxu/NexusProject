#include "CorePCH.h"
#include "NekoCore_Stream.h"

namespace NekoEngine
{

	//////////////////////////////////////////////////////////////////////////
	// IniSection
	//////////////////////////////////////////////////////////////////////////
	IniSection::IniSection( const WString& Name )
		: mName( Name )
	{

	}

	void IniSection::AddData( const WString& Key, const WString& Data )
	{
		mSettingsMultiMap.insert( SettingsMultiMap::value_type( Key, Data ) );
	}


	bool IniSection::Read(const WString& Key, WString& Data )
	{	
		SettingsMultiMap::iterator it = mSettingsMultiMap.find( Key );
		if ( it == mSettingsMultiMap.end() )
			return false;

		Data = it->second;

		return true;
	}

	bool IniSection::Read( const WString& Key, std::vector<WString>& DataArray )
	{	
		SettingsMultiMap::iterator it = mSettingsMultiMap.find( Key );

		if ( it == mSettingsMultiMap.end() )
			return false;

		for ( ;it != mSettingsMultiMap.end(); ++it )
		{
			DataArray.push_back( it->second );
		}

		return true;
	}

	void IniSection::DumpContent( TextFile& TFile )
	{
		for ( SettingsMultiMap::iterator it = mSettingsMultiMap.begin(); 
			it != mSettingsMultiMap.end();
			++it )
		{			
			const WString& Key = it->first;
			WString& Data = it->second;

			TFile.WriteLine(L"%s = %s", Key.c_str(), Data.c_str() );
		}
	}

	bool IniSection::GetFirstPair( WString& Key, WString& Data )
	{
		mIterator = mSettingsMultiMap.begin();

		return GetNextPair( Key, Data );
	}

	bool IniSection::GetNextPair( WString& Key, WString& Data )
	{
		if ( mIterator == mSettingsMultiMap.end() )
			return false;

		Key = mIterator->first;
		Data = mIterator->second;

		++mIterator;

		return true;
	}

	void IniSection::SetSearchKey( const WString& Key )
	{
		mIterator = mSettingsMultiMap.find( Key );		
	}

	bool IniSection::GetNextPairData( WString& Data )
	{
		if ( mIterator == mSettingsMultiMap.end() )
			return false;

		Data = mIterator->second;

		++mIterator;

		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	// IniFile
	//////////////////////////////////////////////////////////////////////////


	IniFile::IniFile()
	{

	}

	IniFile::IniFile( const IniFile& Other )
	{
		*this = Other;
	}


	IniFile::~IniFile()
	{
		for ( SectionMap::iterator it = mSectionMap.begin();
			it != mSectionMap.end();
			++it )
		{
			IniSection* SectionData = it->second;
			delete SectionData;
		}

		mSectionMap.clear();
	}

	bool IniFile::Parse( DataStream* Stream, bool CopyMem )
	{
		TextFile TFile;
		if ( !TFile.Open( Stream, CopyMem ) ) 
			return false;

		return ParseContent( TFile );
	}


	bool IniFile::Parse( const wchar* FileName )
	{
		TextFile TFile;
		if ( !TFile.Open( FileName ) ) 
			return false;

		return ParseContent( TFile );
	}

	bool IniFile::Dump( DataStream* Stream )
	{
		TextFile TFile;
		if ( !TFile.Create( Stream ) ) 
			return false;

		DumpContent( TFile );

		return true;
	}


	bool IniFile::Dump( const wchar* FileName )
	{
		TextFile TFile;
		if ( !TFile.Create( FileName ) ) 
			return false;

		DumpContent( TFile );

		return true;
	}

	bool IniFile::ParseContent( TextFile& TFile )
	{
		WString LineText;

		IniSection* SectionData = null;

		while ( TFile.ReadLine(LineText) )
		{
			if ( LineText.length() == 0 )
				continue;

			if ( LineText.at(0) == '#' )
				continue;


			{
				// ÕÒSection
				size_t Left =LineText.find(L'[');
				size_t Right = LineText.rfind(L']');
				if( Left != WString::npos && Right != WString::npos && Left != Right+1)
				{
					SectionData = AddSection( StringHelper::Trim( LineText.substr(Left+1,Right-Left-1 ) ) );

					continue;
				}
			}
			

			if ( SectionData == null )
				continue;


			size_t Equal = LineText.find(L'=');

			if( Equal== WString::npos )
				continue;

			WString Key = LineText.substr(0,Equal);
			WString Data = LineText.substr(Equal+1, WString::npos);

			StringHelper::Trim( Key );
			StringHelper::Trim( Data );

			SectionData->AddData( Key, Data );
			
		}

		return true;
	}

	void IniFile::DumpContent( TextFile& TFile )
	{
		for ( SectionMap::iterator it = mSectionMap.begin();
			it != mSectionMap.end();
			++it )
		{
			IniSection* SectionData = it->second;
			TFile.WriteLine(L"[%s]", SectionData->GetName().c_str() );
			
			SectionData->DumpContent( TFile );

			TFile.WriteLine(L"\r\n");
		}
	}

	IniSection* IniFile::AddSection( const WString& Section )
	{
		SectionMap::iterator it = mSectionMap.find( Section );

		if ( it == mSectionMap.end() )
		{
			IniSection* SectionData = new IniSection( Section );
			mSectionMap[ Section ] = SectionData;

			return SectionData;
		}
		
		return it->second;
	}

	IniSection* IniFile::QuerySection( const WString& Section )
	{
		SectionMap::iterator it = mSectionMap.find( Section );

		if ( it == mSectionMap.end() )
		{
			return null;
		}

		return it->second;
	}

	

	bool IniFile::Read( const WString& Section, const WString& Key, WString& Data )
	{
		IniSection* SectionData = QuerySection( Section );
		if ( SectionData == null )
			return false;

		

		return SectionData->Read( Key, Data );
	}

	IniSection* IniFile::GetFirstSection()
	{
		mIterator = mSectionMap.begin();

		return GetNextSection();
	}

	IniSection* IniFile::GetNextSection()
	{
		if ( mIterator == mSectionMap.end() )
			return null;

		IniSection* SectionData = mIterator->second;

		++mIterator;

		return SectionData;
	}

	IniFile& IniFile::operator=( const IniFile& Other )
	{
		for ( SectionMap::const_iterator it = Other.mSectionMap.begin();
			it != Other.mSectionMap.end();
			++it )
		{
			IniSection* SectionData = AddSection( it->first );

			SectionData->mSettingsMultiMap = it->second->mSettingsMultiMap;
		}

		return *this;
	}




	
}

