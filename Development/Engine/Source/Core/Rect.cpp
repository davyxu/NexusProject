#include "CorePCH.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{
	const Rect Rect::cNullRect( 0, 0, 0, 0 );

	Rect Rect::Intersect( const Rect& rect1, const Rect& rect2 )
	{
		if ( rect1.Width( ) == 0 || rect1.Height( ) == 0 )
			return rect2;

		if ( rect2.Width( ) == 0 || rect2.Height( ) == 0 )
			return rect1;

		int left = Math::Max( rect1.left, rect2.left );

		if ( left >= rect1.right || left >= rect2.right )
			return cNullRect;

		int right = Math::Min( rect1.right, rect2.right );

		if ( right <= rect1.left || right <= rect2.left )
			return cNullRect;

		int top = Math::Max( rect1.top, rect2.top );

		if ( top >= rect1.bottom || top >= rect2.bottom )
			return cNullRect;

		int bottom = Math::Min( rect1.bottom, rect2.bottom );

		if ( bottom <= rect1.top || bottom <= rect2. top )
			return cNullRect;

		return Rect( left, top, right, bottom );
	}

	Rect Rect::Unite( const Rect& rect1, const Rect& rect2 )
	{
		if ( rect1.Width( ) == 0 || rect1.Height( ) == 0 )
			return rect2;

		if ( rect2.Width( ) == 0 || rect2.Height( ) == 0 )
			return rect1;

		int left = Math::Min( rect1.left, rect2.left );
		int right = Math::Max( rect1.right, rect2.right );
		int top = Math::Min( rect1.top, rect2.top );
		int bottom = Math::Max( rect1.bottom, rect2.bottom );

		return Rect( left, top, right, bottom );
	}

	Rect Rect::Clip( const Rect& source, const Rect& cliprc )
	{
		int left = Math::Max( source.left, cliprc.left );

		if ( left >= cliprc.right || left >= source.right )
			return Rect::cNullRect;


		int right = Math::Min( source.right, cliprc.right );

		if ( right <= cliprc.left || right <= source.left )
			return Rect::cNullRect;

		int top = Math::Max( source.top, cliprc.top );

		if ( top >= cliprc.bottom || top >= source.bottom )
			return Rect::cNullRect;


		int bottom = Math::Min( source.bottom, cliprc.bottom );

		if ( bottom <= cliprc.top || bottom <= source.top )
			return Rect::cNullRect;

		return Rect( left, top, right, bottom );
	}
}
