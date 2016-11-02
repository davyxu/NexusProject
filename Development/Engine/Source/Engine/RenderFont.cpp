#include "EnginePCH.h"
#include "NekoEngine_Render.h"
#include "Engine/RenderFont.h"

#include <windows.h>
#include "gdiplus.h"
#pragma comment( lib, "gdiplus" )

namespace NekoEngine
{

	RenderFontTexture::RenderFontTexture(  dword TextureSize )
		: mTextureSize( TextureSize )
	{

	}

	


	void RenderFontTexture::InitRHI()
	{
		__super::InitRHI( );

		mTextureRHI = RHI->CreateTexture2D( 
			mTextureSize, 
			mTextureSize,
			1,
			PF_A8R8G8B8 );
	}

	void RenderFontTexture::SyncData( const RenderFontTextureParameter& Parameter )
	{
		Gdiplus::Bitmap* backbitmap = (Gdiplus::Bitmap*)Parameter.mGDIPlusBitmap;

		Gdiplus::BitmapData bitmapdata;
		backbitmap->LockBits( &Gdiplus::Rect( 0, 0, Parameter.mCharSize.x, Parameter.mCharSize.y ),
			Gdiplus::ImageLockModeRead, PixelFormat32bppRGB, &bitmapdata );


		const _byte* SrcBuffer = (const _byte*) bitmapdata.Scan0;

		dword Pitch = 0;

		Rect LockRect( Parameter.mCharPos.x, Parameter.mCharPos.y, Parameter.mCharPos.x + Parameter.mCharSize.x,  Parameter.mCharPos.y + Parameter.mCharSize.y);

		dword* DestBuffer = (dword*)mTextureRHI->Lock( 0,  true, Pitch, &LockRect );

		// 每个像素4bytes， 所以跨越应该是4分之一
		Pitch /= 4;

		for ( int y = 0; y < Parameter.mCharSize.y; y ++ )
		{
			const dword* scanline = (const dword*) ( SrcBuffer + bitmapdata.Stride * y );

			for ( int x = 0; x < Parameter.mCharSize.x; x ++ )
			{
				dword OrgColor = scanline[x];

				// 取RGB的灰度值作为颜色及Alpha
				_byte Gray = Color::RGB32ToGray( GetRValue(OrgColor), GetGValue(OrgColor),GetBValue(OrgColor) );

				dword RGBColor = OrgColor & 0x00FFFFFF;
				
				DestBuffer[ x + y * Pitch ] = Gray | (Gray << 8) | (Gray << 16) |(Gray << 24);
			}
		}

		mTextureRHI->Unlock( 0 );


		backbitmap->UnlockBits( &bitmapdata );

		Gdiplus::Graphics* graphics = (Gdiplus::Graphics*)Parameter.mGDIGraphics;
		delete graphics;

		delete backbitmap;



	}

	void RenderFontTexture::WaitSync()
	{
		mSyncFence.BeginFence();

		mSyncFence.Wait( );
	}

	RenderFontTexture* RenderFont::AllocTexture( )
	{
		RenderFontTexture* fonttexture = new RenderFontTexture(	_TEXTURE_SIZE );

		fonttexture->BeginInitResource();

		mFontTextureArray.push_back( fonttexture );

		mPrintHeaderX	= cGlyphMargin;
		mPrintHeaderY	= cGlyphMargin;
		mMaxLineHeight	= cGlyphMargin;

		return fonttexture;
	}

	bool RenderFont::GetCharWritePos( dword CharWidth, dword CharHeight, Point& CharWritePos )
	{
		int CharX = CharWidth + cGlyphMargin;
		int CharY = CharHeight + cGlyphMargin;

		mMaxLineHeight = Math::Max( CharY, mMaxLineHeight );

		int CharRight = mPrintHeaderX + CharX;

		if ( CharRight > _TEXTURE_SIZE )
		{
			mPrintHeaderX = cGlyphMargin;

			CharRight = mPrintHeaderX + CharX;

			mPrintHeaderY += mMaxLineHeight;

			mMaxLineHeight = cGlyphMargin;
		}

		int CharBottom = mPrintHeaderY + CharY;

		if ( CharBottom > _TEXTURE_SIZE )
		{
			return false;
		}

		CharWritePos.x = mPrintHeaderX;
		CharWritePos.y = mPrintHeaderY;

		mPrintHeaderX = CharRight;

		return true;
	}
	

	void RenderFont::CacheChar( dword code )
	{
		// If the font code is created, just return.
		if ( mCodeList[ code ].mTextureIndex != -1 )
			return;

		const wchar* string = (wchar*) &code;

		// Get the size of this font code.
		Point CharSize;
		::GetTextExtentPoint( (HDC) mGDIMemDC, string, StringHelper::Length(string), (SIZE*) &CharSize );


		// Draw a blank rectangle first.
		::Rectangle( (HDC) mGDIMemDC, 0, 0, CharSize.x,  CharSize.y);
		::TextOut( (HDC) mGDIMemDC, 0, 0, string, StringHelper::Length( string ));

		// Create GDI+ graphics object.
		Gdiplus::Graphics* graphics = new Gdiplus::Graphics( (HDC) mGDIMemDC );

		// Get bitmap from graphics object, used for create texture.
		Gdiplus::Bitmap* backbitmap = new Gdiplus::Bitmap( CharSize.x, CharSize.y, graphics );

		Gdiplus::Graphics* backgraphics = Gdiplus::Graphics::FromImage( backbitmap );

		// Copy font to bitmap.
		HDC graphicsdc = backgraphics->GetHDC( );
		::BitBlt( graphicsdc, 0, 0,  CharSize.x,  CharSize.y, (HDC) mGDIMemDC, 0, 0, SRCCOPY );
		backgraphics->ReleaseHDC( graphicsdc );

		// Delete GDI+ graphics.
		delete backgraphics;





		// Create font texture.
		{
			RenderFontTexture* fonttexture = null;

			Point CharWritePos;

			while( !GetCharWritePos( CharSize.x, CharSize.y, CharWritePos ) )
			{
				fonttexture = AllocTexture();
			}

			// 仍然在范围内
			if ( fonttexture == null )
			{
				fonttexture = mFontTextureArray.back();
			}


			RenderFontTextureParameter Parameter;
			Parameter.mGDIGraphics		= graphics;
			Parameter.mGDIPlusBitmap	= backbitmap;
			Parameter.mCharPos			= CharWritePos;
			Parameter.mCharSize			= CharSize;

			// 放入渲染线程写纹理
			ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
				SyncBitmap,
				RenderFontTexture*,Tex,fonttexture,
				RenderFontTextureParameter, Param, Parameter,
			{
				Tex->SyncData(Param);
			});				

			FontCode& fontcode = mCodeList[ code ];

			// Set font code information.
			fontcode.mTextureIndex	= mFontTextureArray.size( ) - 1;
			fontcode.mWidth			= CharSize.x;
			fontcode.mHeight		= CharSize.y;
			fontcode.mStartX		= CharWritePos.x;
			fontcode.mStartY		= CharWritePos.y;

		}


	}




	static ULONG GDIPlusToken = 0;

	void RenderFont::StaticInit( )
	{
		Gdiplus::GdiplusStartupInput gdistartup;
		Gdiplus::GdiplusStartup( &GDIPlusToken, &gdistartup, null );
	}

	void RenderFont::StaticExit( )
	{
		Gdiplus::GdiplusShutdown( GDIPlusToken );
	}


	RenderFont::RenderFont( const wchar* fontname, const wchar* filename, dword size, dword border, bool blod, bool italic, bool underline )
	{
		// Get device context from desktop window.
		HDC dc = ::GetDC( null );

		// Create gui font from font file, register it into system font library.
		//if ( !StringHelper::Compare(filename , L"") )
		//{
		//	BinFile fontfile;

		//	if ( GetResourceManagerSingleton( ) != null )
		//	{
		//		GetResourceManagerSingleton( )->LoadResource( filename, fontfile );
		//	}
		//	else
		//	{
		//		fontfile.Open( filename );
		//	}

		//	if ( fontfile.GetLength( ) != 0 )
		//	{
		//		dword fontnumber = 0;
		//		::AddFontMemResourceEx( fontfile.GetBuffer( ), fontfile.GetLength( ), 0, &fontnumber );
		//	}
		//}

		// Create a compatible memory device context.
		mGDIMemDC = ::CreateCompatibleDC( (HDC) dc );

		// Create gui font from system font library.
		mGDIFont = ::CreateFont( - MulDiv( size, 96, 72 ), 0, 0, 0, blod ? FW_BOLD : FW_NORMAL,
			italic, underline, 0, DEFAULT_CHARSET, 0, 0, 5, 0, fontname );
		mOldFont = ::SelectObject( (HDC) mGDIMemDC, (HGDIOBJ) mGDIFont );
		::SetMapMode( (HDC) mGDIMemDC, MM_TEXT );

		// Get font information.
		TEXTMETRIC textmetric;
		::GetTextMetrics( (HDC) mGDIMemDC, &textmetric );

		mFontName		= fontname;
		mFontSize		= size;
		mBlod			= blod;
		mItalic			= italic;
		mUnderline		= underline;
		mMaxCharWidth	= textmetric.tmMaxCharWidth * 2;	// 某些西文字体没有对应的中文,因此扩大面积
		mMaxCharHeight	= textmetric.tmHeight * 2;

		// 创建本字体最大文字尺寸的MemBitMap
		mGDIMemBitmap = ::CreateCompatibleBitmap( (HDC) dc, mMaxCharWidth, mMaxCharHeight);
		mOldMemBitmap = ::SelectObject( (HDC) mGDIMemDC, (HGDIOBJ) mGDIMemBitmap );

		::SetTextColor( (HDC) mGDIMemDC, RGB( 255, 255, 255 ) );
		::SetBkColor( (HDC) mGDIMemDC, RGB( 0, 0, 0) );


		// Release the device context of desktop window.
		::ReleaseDC( null, (HDC) dc );

		// Initialize font code list.
		for ( dword i = 0; i < 65536; i ++ )
			mCodeList[i].mTextureIndex = -1;

		mFontColor		= Color::cWhite;
		mBorderColor	= Color::cYellow;

		AllocTexture();
	}

	RenderFont::~RenderFont( )
	{
		// Reset GDI font.
		if ( mOldFont != null )
			::SelectObject( (HDC) mGDIMemDC, (HGDIOBJ) mOldFont );

		// Reset GDI bitmap.
		if ( mOldMemBitmap != null )
			::SelectObject( (HDC) mGDIMemDC, (HGDIOBJ) mOldMemBitmap );

		// Delete bitmap.
		if ( mGDIMemBitmap != null )
			::DeleteObject( (HGDIOBJ) mGDIMemBitmap );

		// Delete memory device context.
		if ( mGDIMemDC != null )
			::DeleteDC( (HDC) mGDIMemDC );

		// Delete GDI font.
		if ( mGDIFont != null )
			::DeleteObject( (HGDIOBJ) mGDIFont );

		// Release all font textures.
		for ( dword i = 0; i < mFontTextureArray.size( ); i ++ )
		{
			RenderFontTexture* Tex = mFontTextureArray[i];
			Tex->BeginReleaseResource();
			Tex->WaitRelease();

			delete Tex;
		}

		mFontTextureArray.clear();
	}



	void RenderFont::Save( const wchar* FileName )
	{
		GRenderThread.Flush();

		RenderFontTexture* fonttexture = mFontTextureArray.back();
		fonttexture->GetTextureRHI()->Save(FileName);
	}

	RenderFontTexture* RenderFont::GetFontTexture( wchar character, Vector2& uv, Vector2& uvsize, Vector2& charsize )
	{
		CacheChar( character );

		FontCode& fontcode = mCodeList[ character ];

		if ( fontcode.mTextureIndex == -1 )
			return null;

		uv = Vector2( (float)fontcode.mStartX / _TEXTURE_SIZE, (float)fontcode.mStartY / _TEXTURE_SIZE  );
		uvsize = Vector2( (float) fontcode.mWidth / _TEXTURE_SIZE, (float) fontcode.mHeight / _TEXTURE_SIZE );		
		charsize = Vector2( (float)fontcode.mWidth, (float)fontcode.mHeight );

		return mFontTextureArray[fontcode.mTextureIndex];
	}


	const wchar* RenderFont::GetFontName( ) const
	{
		return mFontName.c_str();
	}

	dword RenderFont::GetFontSize( ) const
	{
		return mFontSize;
	}

	bool RenderFont::IsBlod( ) const
	{
		return mBlod;
	}

	bool RenderFont::IsItalic( ) const
	{
		return mItalic;
	}

	bool RenderFont::IsUnderline( ) const
	{
		return mUnderline;
	}



	dword RenderFont::GetCharacterWidth( wchar character )
	{
		// Create font texture if needed.
		CacheChar( character );

		return mCodeList[ character ].mWidth;
	}

	dword RenderFont::GetStringWidth( const wchar* string )
	{
		dword length = 0; 

		for ( wchar* buffer = (wchar*) (const wchar*) string; *buffer != 0; buffer ++ )
		{
			// Create font texture if needed.
			CacheChar( *( (word*) buffer ) );

			// Increase string length by add width of the font code.
			length += mCodeList[ *( (word*) buffer ) ].mWidth;
		}

		return length;
	}

	void RenderFont::SetFontColor( dword color )
	{
		mFontColor = color;
	}

	dword RenderFont::GetFontColor( ) const
	{
		return mFontColor;
	}

	void RenderFont::SetBorderColor( dword color )
	{
		mBorderColor = color;
	}

	dword RenderFont::GetBorderColor( ) const
	{
		return mBorderColor;
	}



}