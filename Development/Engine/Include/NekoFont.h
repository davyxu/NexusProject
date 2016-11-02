#pragma once


namespace NekoEngine
{
	class IFontGlyph
	{
	public:		
		virtual const Point& GetUV( ) = 0;

		virtual const Point& GetSize( ) = 0;

		virtual const Point& GetOffset( ) = 0;

		virtual RenderTexture* GetTexture() = 0;

		virtual dword GetTextureWidth() = 0;

		virtual dword GetTextureHeight() = 0;
	};

	class IFontObject
	{
	public:
		virtual IFontGlyph* GetGlyph( wchar C ) = 0;

		virtual void CommitFontTextureByString( const wchar* Text ) = 0;
	};

	class IFontDriver
	{
	public:
		virtual void Release() = 0;

		virtual IFontObject* CreateFontType( const wchar* FontPathName, dword FontSize ) = 0;		
	};


	extern void FontDriverInitialize( );

	extern IFontDriver* FontDrv;
}
