#include "FontDrvPCH.h"
#include "FontFiles.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// FontTexture
	//////////////////////////////////////////////////////////////////////////
	FontTexture::FontTexture( dword Index, dword TextureSize ) : mIndex( Index )
		, mPrintOffset( FontTexture::cGlyphMargin, FontTexture::cGlyphMargin )
		, mMaxLineHeight( 0 )
		, mDirtyCharCount( 0 )
	{
		_byte* Data = mBufferData.Alloc( TextureSize, TextureSize, PF_A8L8 );
		mBufferData.SetMip( 0, Data,  mBufferData.mDataSize );

		mTexture = new RenderTexture2D( mBufferData );


		mTexture->BeginInitResource();
	}

	FontTexture::~FontTexture()
	{
		mBufferData.Release();

		mTexture->BeginReleaseResource();
		mTexture->WaitRelease();

		delete mTexture;
	}


	//////////////////////////////////////////////////////////////////////////
	// FontObject
	//////////////////////////////////////////////////////////////////////////
	FontObject::FontObject( FontFace& Face, dword Size )
		: mFontFace( Face )
		, mFontSize( Size )		
	{
		FT_Face& FTFace = Face.GetFTFace();

		mGlyphsArray.resize( FTFace->num_glyphs );




	}

	FontObject::~FontObject()
	{
		for ( dword i = 0;i<mTextureBufferArray.size();i++)
		{
			FontTexture* Tex = mTextureBufferArray[i];
			delete Tex;
		}
	}

	IFontGlyph* FontObject::GetGlyph( wchar C )
	{
		return CacheSingleChar( C );
	}

	FontTexture* FontObject::GetCurrentFontTexture( )
	{
		if ( mTextureBufferArray.size() == 0 )
			return AllocFontTexture();

		return mTextureBufferArray.back();
	}

	FontTexture* FontObject::AllocFontTexture(  )
	{
		// 提交当前的脏纹理
		FlushDirtyTexture( );

		FontTexture* Texture = new FontTexture( mTextureBufferArray.size(), mFontFace.mDriver->GetTextureSize() );

		mTextureBufferArray.push_back( Texture );

		return Texture;
	}

	void FontObject::CommitFontTextureByString( const wchar* Text )
	{
		for ( dword i = 0; Text[i]; i++)
		{
			CacheSingleChar( Text[i] );			
		}

		FlushDirtyTexture();
	}

	bool FontObject::WriteCharToBuffer( dword CharIndex, FontGlyph& Glyph, FontTexture* FontTex )
	{		
		FT_Face& FF = mFontFace.GetFTFace();

		if (FT_Load_Glyph( FF, CharIndex, FT_LOAD_RENDER ))
			return true;

		dword TextureSize = mFontFace.mDriver->GetTextureSize();

		dword GlyphX = FF->glyph->bitmap.width + FontTexture::cGlyphMargin;
		dword GlyphY = FF->glyph->bitmap.rows + FontTexture::cGlyphMargin;

		// 字模本身的偏移，例如g要下偏移一点
		dword GlyphOffsetX = FF->glyph->bitmap_left;
		dword GlyphOffsetY = mFontSize - FF->glyph->bitmap_top;

		// 取行里最高的为整行高，避免截断
		FontTex->mMaxLineHeight = Math::Max( GlyphY + GlyphOffsetY , FontTex->mMaxLineHeight );

		// 计算字模右边界=当前打印位置+字模大小（带间隔）+字模偏移
		dword GlyphRight = FontTex->mPrintOffset.x + GlyphX + GlyphOffsetX;

		// 超过右边界情况
		if ( GlyphRight > TextureSize )
		{
			// 归位到左边
			FontTex->mPrintOffset.x = FontTexture::cGlyphMargin;

			// 重新计算新的右边界
			GlyphRight = FontTex->mPrintOffset.x + GlyphX;

			// 往下一行
			FontTex->mPrintOffset.y += FontTex->mMaxLineHeight;

			// 新行行高重新计算
			FontTex->mMaxLineHeight = FontTexture::cGlyphMargin;
		}

		// 计算字模下边界=当前打印位置+字模大小（带间隔）+字模偏移
		dword GlyphBottom = FontTex->mPrintOffset.y + GlyphY + GlyphOffsetY ;


		// 超过下边界，终止填充
		if ( GlyphBottom > TextureSize )
		{
			return false;
		}

		// 将FF的字模缓冲拷贝到指定缓冲之上
		CopyGlyph(	FontTex->mPrintOffset.x + GlyphOffsetX , 
					FontTex->mPrintOffset.y + GlyphOffsetY,
					GlyphOffsetX, 
					 GlyphOffsetY , 
					(word*)FontTex->mBufferData.mData,
					Glyph, 
					FontTex);

		// 答应位置转到下一个位置
		FontTex->mPrintOffset.x = GlyphRight;

		FontTex->mDirtyCharCount++;

		return true;
	}

	void FontObject::CopyGlyph( dword DestX, dword DestY, dword OffsetX, dword OffsetY, word* DestBuffer, FontGlyph& Glyph, FontTexture* FontTex   )
	{
		dword TextureSize = mFontFace.mDriver->GetTextureSize();

		FT_Bitmap& bitmap = mFontFace.GetFTFace()->glyph->bitmap;

		// 计算拷贝目标的偏移位置
		word* Dest = DestBuffer +  DestY  * TextureSize  + DestX ;

		for ( int y = 0; y < bitmap.rows ;y++ )
		{
			_byte* src = bitmap.buffer + ( y * bitmap.pitch );
			_byte* pDest = (_byte*)Dest;

			// 拷贝一行
			for ( int x = 0; x < bitmap.width; x ++ )
			{
				*pDest ++ = (*src++);
				*pDest ++ = 0xFF;
			}

			Dest += TextureSize;
		}

		Glyph.SetInfo( Point( DestX, DestY ),
					   Point( bitmap.width, bitmap.rows ),
					   Point( OffsetX, OffsetY ),
					   FontTex );
	}


	void FontObject::FlushDirtyTexture( )
	{
		if ( mTextureBufferArray.size() == 0 )
			return;

		FontTexture* DirtyTexture = mTextureBufferArray.back();

		// 没有脏的字符,无需提交
		if ( DirtyTexture->mDirtyCharCount == 0 )
			return;

		DirtyTexture->mDirtyCharCount = 0;

		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
			SyncFontData,
			RenderTexture2D*,Tex,DirtyTexture->mTexture,
		{
			Tex->SyncData();
		});

		if ( 0 )
		{
			DirtyTexture->mTexture->WaitSync();
			DirtyTexture->mTexture->mTextureRHI->Save(L"t.png");
		}
	}


	FontGlyph* FontObject::CacheSingleChar( wchar Char )
	{
		FT_Face& FF = mFontFace.GetFTFace();

		dword CharIndex = FT_Get_Char_Index( FF, Char );

		// 找不到这个字
		if ( CharIndex == 0 )
			return null;
		
		FontGlyph* GlyphRef = mGlphHashMap.find( CharIndex );

		if ( GlyphRef != null )
			return GlyphRef;


		FontTexture* Texture = GetCurrentFontTexture( );

		FontGlyph Glyph;
		dword Times = 0;

		while ( !WriteCharToBuffer( Char, Glyph, Texture ) )
		{
			// 不至于出现2张纹理都存不下的情况
			Assert( Times == 0 )
			if ( Times > 0 )
				break;

			// 本张纹理已经无法保存, 分配新的保存
			Texture = AllocFontTexture();
			Times++;
		}

		return &mGlphHashMap.set( CharIndex, Glyph);
	}





}