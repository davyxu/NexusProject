#include "FontDrvPCH.h"
#include "FontFiles.h"

namespace NekoEngine
{
	IFontDriver* FontDrv = null;

	void FontDriverInitialize( )
	{
		FontDrv = new FontDriver;
	}

	FontDriver::FontDriver()
		: mTextureSize( 256 )
	{
		FT_Init_FreeType( &mLibrary );
	}

	FontDriver::~FontDriver( )
	{
		// not call here
	}

	void FontDriver::Release()
	{

		for ( FontFaceMap::iterator it(mFontFaceMap); it;++it)
		{
			FontFace* Face = it.get_value();
			delete Face;
		}

		mFontFaceMap.clear();

		FT_Done_FreeType( mLibrary );

		delete this;
	}


	IFontObject* FontDriver::CreateFontType( const wchar* FontPathName, dword FontSize )
	{
		WString StrFontPathName = StringHelper::Lower( FontPathName );

		FontFace* Face = GetFace( StrFontPathName );

		// 创建新的Face
		if ( Face == null )
		{
			Face = new FontFace( this );
			if ( !Face->Load( mLibrary, FontPathName ) )
			{
				delete Face;

				return null;
			}

			mFontFaceMap.set( StrFontPathName, Face );
		}

		// 没有Font对象时创建
		IFontObject* FontObj = Face->GetFontObject( FontSize );
		if ( FontObj == null )
		{
			FontObj = Face->CreateFontObject( FontSize );
		}

		FT_Face& FF = Face->GetFTFace();

		FT_F26Dot6 ftSize = (FT_F26Dot6)(FontSize * (1 << 6));
		FT_Set_Char_Size( FF, ftSize, 0, 96, 96);

	
		//FontObj->DrawText( Vector2::cZero, Color::cWhite, L"hello");

		//const dword MaxTextureSize = 1024;
		//TextureMipMap MipMap;
		//word* Buffer = (word*)MipMap.Alloc( MaxTextureSize, MaxTextureSize,PF_A8L8 );

		//::memset( Buffer, 0, MipMap.mDataSize );
		//for ( dword i = 19968; i < 40869 ;i ++)

		//Point CharPrintOffset(cGlyphMargin, cGlyphMargin );
		//int MaxLineHeight = cGlyphMargin;


		//for ( dword i = 33; i < 166;i ++)
		//{
		//	if ( !WriteFont(i, FF,  Buffer, CharPrintOffset, MaxTextureSize, MaxLineHeight, FontSize ) )
		//		break;

		//}

		//for ( dword i = 19968; i < 40869 ;i ++)
		//{
		//	if ( !WriteFont(i, FF,  Buffer, CharPrintOffset, MaxTextureSize, MaxLineHeight, FontSize ) )
		//		break;
		//}
		

		//MipMap.SetMip( 0, (_byte*)Buffer, MipMap.mDataSize );

		//mTexture2D = new RenderTexture2D( MipMap );
		//mTexture2D->BeginInitResource();

		//mTexture2D->WaitReady();

		//mTexture2D->mTextureRHI->Save( L"t.png");

		//MipMap.Release();
 

		return FontObj;
	}

	FontFace* FontDriver::GetFace( const WString& FontPathName )
	{
		// 小写路径信息作为key
		FontFace** FaceRef = mFontFaceMap.find( FontPathName );

		if ( FaceRef == null )
			return null;

		return *FaceRef;
	}

	IFontObject* FontDriver::GetFont( const WString& FontPathName, dword FontSize )
	{
		FontFace* Face = GetFace( FontPathName );

		if ( Face == null )
			return null;

		return Face->GetFontObject( FontSize );
	}

	//bool FontDriver::WriteFont( dword i, FT_Face& FF, word* Buffer, Point &CharPrintOffset, const dword MaxTextureSize, int& MaxLineHeight, dword FontSize )
	//{
	//	wchar_t c = wchar_t(i);

	//	if (FT_Load_Char( FF, i, FT_LOAD_RENDER ))
	//		return false;

	//	
	//	int GlyphX = FF->glyph->bitmap.width + cGlyphMargin;
	//	int GlyphY = FF->glyph->bitmap.rows + cGlyphMargin;
	//	
	//	// 字模本身的偏移，例如g要下偏移一点
	//	int GlyphOffsetX = FF->glyph->bitmap_left;
	//	int GlyphOffsetY = FontSize - FF->glyph->bitmap_top;

	//	// 取行里最高的为整行高，避免截断
	//	MaxLineHeight = Math::Max( GlyphY + GlyphOffsetY , MaxLineHeight );
	//	
	//	// 计算字模右边界=当前打印位置+字模大小（带间隔）+字模偏移
	//	dword GlyphRight = CharPrintOffset.x + GlyphX + GlyphOffsetX;
	//	
	//	// 超过右边界情况
	//	if ( GlyphRight > MaxTextureSize )
	//	{
	//		// 归位到左边
	//		CharPrintOffset.x = cGlyphMargin;

	//		// 重新计算新的右边界
	//		GlyphRight = CharPrintOffset.x + GlyphX;

	//		// 往下一行
	//		CharPrintOffset.y += MaxLineHeight;

	//		// 新行行高重新计算
	//		MaxLineHeight = cGlyphMargin;
	//	}

	//	// 计算字模下边界=当前打印位置+字模大小（带间隔）+字模偏移
	//	dword GlyphBottom = CharPrintOffset.y + GlyphY + GlyphOffsetY ;

	//	// 超过下边界，终止填充
	//	if ( GlyphBottom > MaxTextureSize )
	//	{
	//		return false;
	//	}

	//	// 将FF的字模缓冲拷贝到指定缓冲之上
	//	CopyGlyph( CharPrintOffset.x + GlyphOffsetX, CharPrintOffset.y +GlyphOffsetY , MaxTextureSize, Buffer, FF );

	//	// 答应位置转到下一个位置
	//	CharPrintOffset.x = GlyphRight;

	//	return true;
	//}

	//void FontDriver::CopyGlyph( dword DestX, dword DestY, dword TextureSize, word* DestBuffer, FT_Face& Face )
	//{
	//	FT_Bitmap& bitmap = Face->glyph->bitmap;
	//
	//	// 计算拷贝目标的偏移位置
	//	word* Dest = DestBuffer +  DestY * TextureSize  + DestX;

	//	for ( int y = 0; y < bitmap.rows ;y++ )
	//	{
	//		_byte* src = bitmap.buffer + ( y * bitmap.pitch );
	//		_byte* pDest = (_byte*)Dest;

	//		// 拷贝一行
	//		for ( int x = 0; x < bitmap.width; x ++ )
	//		{
	//			*pDest ++ = (*src++);
	//			*pDest ++ = 0xFF;
	//		}

	//		Dest += TextureSize;
	//	}
	//}

}