#include "FontDrvPCH.h"
#include "FontFiles.h"

namespace NekoEngine
{
	/*FontGlyph::FontGlyph()
		: mIsCached( false )
	{

	}*/

	void FontGlyph::SetInfo( const Point& UV, const Point& Size, const Point& Offset, FontTexture* InTexture )
	{
		mUV = UV;
		mSize = Size;
		mOffset = Offset;
		mTexture = InTexture;

		//mIsCached = true;
	}

	dword FontGlyph::GetTextureWidth()
	{
		return mTexture->mTexture->GetWidth();		
	}

	dword FontGlyph::GetTextureHeight()
	{
		return mTexture->mTexture->GetHeight();		
	}


}