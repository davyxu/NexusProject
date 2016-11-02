#include "CorePCH.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{
	const FRect FRect::cNullRect( 0, 0, 0, 0 );

	FRect FRect::Intersect( const FRect& rect1, const FRect& rect2 )
	{
		if ( rect1.Width( ) == 0 || rect1.Height( ) == 0 )
			return rect2;

		if ( rect2.Width( ) == 0 || rect2.Height( ) == 0 )
			return rect1;

		float left = Math::Max( rect1.left, rect2.left );

		if ( left >= rect1.right || left >= rect2.right )
			return cNullRect;

		float right = Math::Min( rect1.right, rect2.right );

		if ( right <= rect1.left || right <= rect2.left )
			return cNullRect;

		float top = Math::Max( rect1.top, rect2.top );

		if ( top >= rect1.bottom || top >= rect2.bottom )
			return cNullRect;

		float bottom = Math::Min( rect1.bottom, rect2.bottom );

		if ( bottom <= rect1.top || bottom <= rect2. top )
			return cNullRect;

		return FRect( left, top, right, bottom );
	}

	FRect FRect::Unite( const FRect& rect1, const FRect& rect2 )
	{
		if ( rect1.Width( ) == 0 || rect1.Height( ) == 0 )
			return rect2;

		if ( rect2.Width( ) == 0 || rect2.Height( ) == 0 )
			return rect1;

		float left = Math::Min( rect1.left, rect2.left );
		float right = Math::Max( rect1.right, rect2.right );
		float top = Math::Min( rect1.top, rect2.top );
		float bottom = Math::Max( rect1.bottom, rect2.bottom );

		return FRect( left, top, right, bottom );
	}
}
