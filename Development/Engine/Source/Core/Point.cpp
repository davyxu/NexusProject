#include "CorePCH.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{
	const Point Point::cZero( 0, 0 );
	const Point Point::cMaxPoint( 0x7FFFFFFF, 0x7FFFFFFF );
	const Point Point::cMinPoint( 0x80000001, 0x80000001 );

	int Point::DistanceTo(const Point& v1, const Point& v2)
	{
		int dx = v1.x - v2.x;
		int dy = v1.y - v2.y;
		return (int)Math::Sqrt( float(dx * dx + dy * dy) );
	}


	int Point::DistanceToSqare(const Point& v1, const Point& v2)
	{
		int dx = v1.x - v2.x;
		int dy = v1.y - v2.y;
		return dx * dx + dy * dy ;
	}
}
