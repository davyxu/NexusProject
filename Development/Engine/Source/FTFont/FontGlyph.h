#pragma once

namespace NekoEngine
{
	// 字模，保存纹理坐标
	class FontGlyph : public IFontGlyph
	{
	public:

		virtual const Point& GetUV( ){ return mUV; }

		virtual const Point& GetSize( ){ return mSize; }

		virtual const Point& GetOffset( ){ return mOffset; }

		virtual RenderTexture* GetTexture() { return mTexture->mTexture;}

		virtual dword GetTextureWidth();

		virtual dword GetTextureHeight();

		//bool IsCached( ){ return mIsCached; }

		void SetInfo( const Point& UV, const Point& Size, const Point& Offset, FontTexture* InTexture );

	private:
		//bool mIsCached;
		Point mUV;
		Point mSize;
		Point mOffset;
		FontTexture* mTexture;

	};
}