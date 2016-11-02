#include "FontDrvPCH.h"
#include "FontFiles.h"

namespace NekoEngine
{
	FontFace::FontFace( FontDriver* Driver )
		: mDriver( Driver )
	{

	}

	FontFace::~FontFace()
	{
		Release();

		FT_Done_Face( mFTFace );
	}

	bool FontFace::Load( FT_Library& FTLib, const wchar* FontFileName )
	{
		StringConverter conv;

		if ( FT_New_Face( FTLib, conv.UnicodeToAnsi(FontFileName), 0, &mFTFace ) != 0 )
			return false;


		mFileName = FontFileName;

		return true;
	}

	void FontFace::Release( )
	{
		for ( FontObjectMap::iterator it = mFontObjectMap.begin();
			it != mFontObjectMap.end();
			++it )
		{
			FontObject* Obj = it->second;
			delete Obj;
		}

		mFontObjectMap.clear();
	}

	FontObject* FontFace::GetFontObject( dword Size )
	{
		FontObjectMap::iterator it = mFontObjectMap.find( Size );
		if ( it != mFontObjectMap.end() )
			return it->second;

		return null;
	}

	FontObject* FontFace::CreateFontObject( dword Size )
	{
		FontObject* FontObj = new FontObject( *this, Size );

		mFontObjectMap.insert( std::pair<dword, FontObject*>( Size, FontObj ) );

		return FontObj;
	}





}