#pragma once

namespace NekoEngine
{
	class FontFace;
	class FontGlyph;

	struct FontTexture 
	{
		enum{ cGlyphMargin = 2 };

		dword mIndex;

		RenderTexture2D*	mTexture;
		TextureMipMap		mBufferData;

		Point mPrintOffset;		// 当前打印头位置
		dword mMaxLineHeight;	// 本行的最大行高

		dword mDirtyCharCount;	// 本次有多少个字写入

		FontTexture( dword Index, dword TextureSize );

		~FontTexture( );
	};

	class FontObject : public IFontObject
	{
	public:
		FontObject( FontFace& Face, dword Size );

		virtual ~FontObject( );

		virtual IFontGlyph* GetGlyph( wchar C );

		virtual void CommitFontTextureByString( const wchar* Text );

	private:

		// 获取当前可用的纹理缓冲对象
		FontTexture* GetCurrentFontTexture( );

		// 提交之前的缓冲到纹理, 分配新的纹理缓冲对象
		FontTexture* AllocFontTexture( );

		FontGlyph* CacheSingleChar( wchar Char );

		// 写入一个字到缓冲
		bool WriteCharToBuffer( dword CharIndex, FontGlyph& Glyph, FontTexture* FontTex );
			
		// 拷贝字模	
		void CopyGlyph( dword DestX, dword DestY, dword OffsetX, dword OffsetY, word* DestBuffer, FontGlyph& Glyph, FontTexture* FontTex );

		// 刷新之前的脏纹理
		void FlushDirtyTexture( );
		

	private:
		

		FontFace& mFontFace;

		dword mFontSize;

		

		typedef std::vector<FontGlyph> GlyphsArray;
		GlyphsArray mGlyphsArray;


		typedef dynamic_hash<dword, FontGlyph, 2048 > GlphHash;
		GlphHash mGlphHashMap;



		typedef std::vector<FontTexture*> TextureBufferArray;
		TextureBufferArray mTextureBufferArray;
	};
}