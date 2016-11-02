#pragma once

namespace NekoEngine
{
	class FontDriver : public IFontDriver
	{
	public:
		FontDriver(  );

		virtual ~FontDriver();

		virtual IFontObject* CreateFontType( const wchar* FontPathName, dword FontSize );

		virtual void Release();
		
		bool WriteFont( dword i, FT_Face& FF, word* Buffer, Point &CharPrintOffset, const dword MaxTextureSize, int& MaxLineHeight, dword FontSize );

		IFontObject* GetFont( const WString& FontPathName, dword FontSize );

		dword GetTextureSize( ){ return mTextureSize; }

	private:		
		FontFace* GetFace( const WString& FontPathName );
		void FontDriver::CopyGlyph( dword DestX, dword DestY, dword TextureSize, word* DestBuffer, FT_Face& Face );
		

	private:
		FT_Library	mLibrary;

		typedef dynamic_hash<WString, FontFace*> FontFaceMap;
		FontFaceMap mFontFaceMap;

		dword mTextureSize;			// 默认纹理大小
	};


}