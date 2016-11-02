#pragma once

namespace NekoEngine
{
	// 一个字体一个Face
	class FontDriver;

	class FontFace
	{
	public:
		FontFace( FontDriver* Driver );

		virtual ~FontFace( );
	
		bool Load( FT_Library& FTLib, const wchar* FontFileName );

		void Release( );

		FontObject* CreateFontObject( dword Size );

		FontObject* GetFontObject( dword Size );

		FT_Face& GetFTFace( ){ return mFTFace;}

	private:
		friend class FontObject;
		FontDriver* mDriver;

		WString mFileName;

		FT_Face	mFTFace;

		// 各种不同Size的FontObject
		typedef std::map<dword, FontObject*> FontObjectMap;
		FontObjectMap mFontObjectMap;
	};
}