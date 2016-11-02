#include "CorePCH.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{
	//=====================================================
	// Ray
	//=====================================================

	Ray::Ray( )
	{
	}

	Ray::Ray( const Vector3& origin, const Vector3& direction )
		: mOrigin( origin ), mDirection( direction )
	{
	}

	Ray::Ray( float x, float y, const Matrix4& viewtransform, const Matrix4& projectiontransform )
	{
		Matrix4 viewinverse = viewtransform.Inverse();

		Vector3 temp( x / projectiontransform( 0, 0 ), - y / projectiontransform( 1, 1 ), 1.0f );

		mOrigin.x		= viewinverse( 3, 0 );
		mOrigin.y		= viewinverse( 3, 1 );
		mOrigin.z		= viewinverse( 3, 2 );
		mDirection.x	= temp.x * viewinverse( 0, 0 ) + temp.y * viewinverse( 1, 0 ) + temp.z * viewinverse( 2, 0 );
		mDirection.y	= temp.x * viewinverse( 0, 1 ) + temp.y * viewinverse( 1, 1 ) + temp.z * viewinverse( 2, 1 );
		mDirection.z	= temp.x * viewinverse( 0, 2 ) + temp.y * viewinverse( 1, 2 ) + temp.z * viewinverse( 2, 2 );
	}

	Ray& Ray::operator *= ( const Matrix4& matrix )
	{
		mOrigin *= matrix;
		mDirection = Vector4( mDirection, 0.0f ) * matrix;

		return *this;
	}

	Ray Ray::operator * ( const Matrix4& matrix ) const
	{
		return Ray( mOrigin * matrix, Vector4( mDirection, 0.0f ) * matrix );
	}

	Vector3 Ray::DistanceVector( float distance ) const
	{
		return mOrigin + mDirection * distance;
	}

	//----------------------------------------------------------------------------
	// AABB Implementation
	//----------------------------------------------------------------------------

	const AABB AABB::cNullBox( Vector3::cZero, Vector3::cZero );
	const AABB AABB::cInitBox( Vector3( Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat ), Vector3( Math::cMinFloat, Math::cMinFloat, Math::cMinFloat ) );
	const AABB AABB::cMaxBox( Vector3( Math::cMinFloat, Math::cMinFloat, Math::cMinFloat ), Vector3( Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat ) );

	AABB::AABB( )
	{
	}

	AABB::AABB( const Vector3& minvector, const Vector3& maxvector )
		: mMinVector( minvector ), mMaxVector( maxvector )
	{
	}

	AABB& AABB::operator += ( const AABB& box )
	{
		mMinVector.x = Math::Min( mMinVector.x, box.mMinVector.x );
		mMinVector.y = Math::Min( mMinVector.y, box.mMinVector.y );
		mMinVector.z = Math::Min( mMinVector.z, box.mMinVector.z );
		mMaxVector.x = Math::Max( mMaxVector.x, box.mMaxVector.x );
		mMaxVector.y = Math::Max( mMaxVector.y, box.mMaxVector.y );
		mMaxVector.z = Math::Max( mMaxVector.z, box.mMaxVector.z );

		return *this;
	}

	AABB& AABB::operator += ( const Vector3& vector )
	{
		mMinVector.x = Math::Min( mMinVector.x, vector.x );
		mMinVector.y = Math::Min( mMinVector.y, vector.y );
		mMinVector.z = Math::Min( mMinVector.z, vector.z );
		mMaxVector.x = Math::Max( mMaxVector.x, vector.x );
		mMaxVector.y = Math::Max( mMaxVector.y, vector.y );
		mMaxVector.z = Math::Max( mMaxVector.z, vector.z );

		return *this;
	}

	AABB& AABB::operator *= ( const Matrix4& matrix )
	{
		Box box( *this );
		box *= matrix;

		mMinVector = mMaxVector = box.mCornerPoint[0];

		for ( dword i = 1; i < 8; i ++ )
		{
			mMinVector.x = Math::Min( mMinVector.x, box.mCornerPoint[i].x );
			mMinVector.y = Math::Min( mMinVector.y, box.mCornerPoint[i].y );
			mMinVector.z = Math::Min( mMinVector.z, box.mCornerPoint[i].z );
			mMaxVector.x = Math::Max( mMaxVector.x, box.mCornerPoint[i].x );
			mMaxVector.y = Math::Max( mMaxVector.y, box.mCornerPoint[i].y );
			mMaxVector.z = Math::Max( mMaxVector.z, box.mCornerPoint[i].z );
		}

		return *this;
	}

	AABB AABB::operator + ( const AABB& box ) const
	{
		AABB returnbox( *this );

		return returnbox += box;
	}

	AABB AABB::operator + ( const Vector3& vector ) const
	{
		AABB returnbox( *this );

		return returnbox += vector;
	}

	AABB AABB::operator * ( const Matrix4& matrix ) const
	{
		AABB returnbox( *this );

		return returnbox *= matrix;
	}

	bool AABB::VectorInBox( const Vector3& vector ) const
	{
		return	vector.x >= mMinVector.x && vector.x <= mMaxVector.x &&
			vector.y >= mMinVector.y && vector.y <= mMaxVector.y &&
			vector.z >= mMinVector.z && vector.z <= mMaxVector.z;
	}

	Vector3 AABB::GetCenter( ) const
	{
		return ( mMinVector + mMaxVector ) * 0.5f;
	}

	Vector3 AABB::GetSize( ) const
	{
		if ( *this == AABB::cNullBox )
			return Vector3::cZero;

		if ( *this == AABB::cMaxBox )
			return Vector3( Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat );

		return mMaxVector - mMinVector;
	}

	Vector3 AABB::GetHalfSize( ) const
	{
		if ( *this == AABB::cNullBox )
			return Vector3::cZero;

		if ( *this == AABB::cMaxBox )
			return Vector3( Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat );

		return ( mMaxVector - mMinVector ) * 0.5f;
	}

	bool AABB::operator==( const AABB& box ) const
	{
		return mMinVector == box.mMinVector && mMaxVector == box.mMaxVector;
	}

	bool AABB::operator!=( const AABB& box ) const
	{
		return mMinVector != box.mMinVector || mMaxVector != box.mMaxVector;
	}

	//----------------------------------------------------------------------------
	// nBox Implementation
	//----------------------------------------------------------------------------

	Box::Box( )
	{
	}
	/*     Z
	       |
		   1-----2
		  /|    /|
		 / |   / |
		5-----4  |
		|  0--|--3 -->X
		| /   | /
		|/    |/
		6-----7
	   /
      Y

	*/

	Box::Box( const AABB& box )
	{
		mCornerPoint[0] = Vector3( box.mMinVector.x, box.mMinVector.y, box.mMinVector.z );

		mCornerPoint[1] = Vector3( box.mMinVector.x, box.mMinVector.y, box.mMaxVector.z );
		mCornerPoint[2] = Vector3( box.mMaxVector.x, box.mMinVector.y, box.mMaxVector.z );
		mCornerPoint[3] = Vector3( box.mMaxVector.x, box.mMinVector.y, box.mMinVector.z );

		mCornerPoint[4] = Vector3( box.mMaxVector.x, box.mMaxVector.y, box.mMaxVector.z );

		mCornerPoint[5] = Vector3( box.mMinVector.x, box.mMaxVector.y, box.mMaxVector.z );
		mCornerPoint[6] = Vector3( box.mMinVector.x, box.mMaxVector.y, box.mMinVector.z );
		mCornerPoint[7] = Vector3( box.mMaxVector.x, box.mMaxVector.y, box.mMinVector.z );
	}

	const Box::NeighbourIndex& Box::GetNeighbourIndex( dword Index )
	{
		static Box::NeighbourIndex cNeighboarBuffer[] =
		{
			{
				3, 6, 1
			},
			{
				2, 5, 0
			},
			{
				1, 4, 3
			},
			{
				0, 7, 2
			},
			{
				5, 2, 7
			},
			{
				4, 1, 6
			},
			{
				7, 0, 5
			},
			{
				6, 3, 4
			},
		};

		Assert( Index < ArraySize( cNeighboarBuffer) );
		return cNeighboarBuffer[ Index ];
	}
	


	dword Box::GetSideIndex( dword Index )
	{
		static dword cIndexBuffer[] = {
			1,0,
			2,3,
			4,7,
			5,6,

			1,2,
			2,4,
			4,5,
			5,1,

			0,3,
			3,7,
			6,7,
			0,6
		};
		
		Assert( Index < ArraySize( cIndexBuffer ) );
		return cIndexBuffer[ Index ];
	}

	Box& Box::operator *= ( const Matrix4& matrix )
	{
		mCornerPoint[0] *= matrix;
		mCornerPoint[1] *= matrix;
		mCornerPoint[2] *= matrix;
		mCornerPoint[3] *= matrix;
		mCornerPoint[4] *= matrix;
		mCornerPoint[5] *= matrix;
		mCornerPoint[6] *= matrix;
		mCornerPoint[7] *= matrix;

		return *this;
	}

	Box Box::operator * ( const Matrix4& matrix ) const
	{
		Box returnbox( *this );

		return returnbox *= matrix;
	}

	//----------------------------------------------------------------------------
	// nSphere Implementation
	//----------------------------------------------------------------------------

	Sphere::Sphere( )
	{
	}

	Sphere::Sphere( const Vector3& center, float radius )
	{
		mCenter = center;
		mRadius	= radius;
	}

	Sphere& Sphere::operator *= ( const Matrix4& matrix )
	{
		mCenter *= matrix;

		return *this;
	}

	Sphere Sphere::operator * ( const Matrix4& matrix ) const
	{
		Sphere returnsphere( *this );

		return returnsphere *= matrix;
	}

	//=====================================================
	// nTriangle
	//=====================================================


	Triangle::Triangle()
	{

	}

	bool Triangle::Inside(float x,float y)
	{
		Vector3 point(x,y,0);

		for (int i = 0;i<3;i++)
		{
			if ( CrossProduct3(point,mPoint[i],mPoint[(i+1)%3]) * 
				 CrossProduct3(point,mPoint[(i+2)%3],mPoint[(i+1)%3]) >0 )   
				return false;
		}

		return true;
	}

	float Triangle::CrossProduct3(const Vector3& p1,const Vector3& p2, const Vector3& p0 )
	{
		return (p1.x-p0.x)*(p2.y-p0.y)-(p2.x-p0.x)*(p1.y-p0.y);
	}

	float Triangle::Area( )
	{
		return Math::Abs((mPoint[0].x-mPoint[2].x)*(mPoint[1].y-mPoint[2].y)-(mPoint[1].x-mPoint[2].x)*(mPoint[0].y-mPoint[2].y)); 
	}

	bool Triangle::Intersect( const Ray& ray,float& distance,float* tU,float* tV)
	{
		Vector3 edge1 = mP2 - mP1;
		Vector3 edge2 = mP3 - mP1;

		Vector3 tvec, pvec = Vector3::CrossProduct( ray.GetDirection(), edge2 );

		float det = Vector3::DotProduct( edge1, pvec );

		if ( det >= 0.0f )
		{
			tvec = ray.GetOrigin() - mP1;
		}
		else
		{
			tvec = mP1 - ray.GetOrigin();
			det = -det;
		}

		if ( det < 0.00000001f )
			return false;

		float u = Vector3::DotProduct( tvec, pvec );
		if( u < 0.0f || u > det )
			return false;

		Vector3 qvec = Vector3::CrossProduct( tvec, edge1 );

		float v = Vector3::DotProduct( ray.GetDirection(), qvec );
		if( v < 0.0f || u + v > det )
			return false;

		distance = Vector3::DotProduct( edge2, qvec ) / det;

		float fInvDet = 1.0f / det;

		u *= fInvDet;
		v *= fInvDet;

		if (tU)
			*tU = u;

		if (tV)
			*tV = v;

		return true;
	}

	Triangle::Triangle( const Vector3& p1,const Vector3& p2,const Vector3& p3 )
		: mP1( p1 )
		, mP2( p2 )
		, mP3( p3 )
	{

	}


	///////////////////////////////////////////////////////////////////////////
	// nPlane
	//////////////////////////////////////////////////////////////////////////

	const Plane Plane::cXYPlane = Plane(0.0f, 0.0f, 1.0f, 0.0f);
	const Plane Plane::cXZPlane = Plane(0.0f, 1.0f, 0.0f, 0.0f);
	const Plane Plane::cYZPlane = Plane(1.0f, 0.0f, 0.0f, 0.0f);

	bool Plane::Intersect(const Ray& ray, const Plane& plane, float& distance)
	{
		float dotvalue = Vector3::DotProduct( plane.Normal( ), ray.GetDirection() );

		if ( Math::Abs( dotvalue ) < Math::cEpsilon )
			return false;

		float tempdistance = plane.Distance( ray.GetOrigin() ) / -dotvalue;

		if ( tempdistance < 0.0f )
			return false;

		distance = tempdistance;

		return true;
	}


	bool Plane::IntersectResult(const Ray& ray, const Plane& plane,Vector3& out)
	{
		float dotValue = Vector3::DotProduct(ray.GetDirection(), plane.n);

		if ( Math::Abs( dotValue ) < Math::cEpsilon )
			return false;

		out = ray.GetOrigin() - ray.GetDirection() *(plane.d + Vector3::DotProduct(plane.n,ray.GetOrigin())) / dotValue;

		return true;
	}






	Plane::Plane( const Vector3& position, const Vector3& normal )
	{
		a = normal.x;
		b = normal.y;
		c = normal.z;
		d = - Vector3::DotProduct( position, normal );
	}

	Plane::Plane( const Vector3& vector1, const Vector3& vector2, const Vector3& vector3 )
	{
		Vector3 edge1 = vector2 - vector1;
		Vector3 edge2 = vector3 - vector1;
		Vector3 normal = Vector3::CrossProduct( edge1, edge2 ).Normalize( );

		a = normal.x;
		b = normal.y;
		c = normal.z;
		d = - Vector3::DotProduct( vector1, normal );
	}

	Plane Plane::operator + ( ) const
	{
		return Plane( a, b, c, d );
	}

	Plane Plane::operator - ( ) const
	{
		return Plane( -a, -b, -c, -d );
	}

	Vector3 Plane::Normal( ) const
	{
		return Vector3( a, b, c );
	}

	float Plane::Distance( const Vector3& vector ) const
	{
		return a * vector.x + b * vector.y + c * vector.z + d;
	}

	Vector3 Plane::ProjectVector(const Vector3& p) const
	{
		// We know plane normal is unit length, so use simple method
		Matrix3 xform;
		xform.m11 = 1.0f - n.x * n.x;
		xform.m12 = -n.x * n.y;
		xform.m13 = -n.x * n.z;
		xform.m21 = -n.y * n.x;
		xform.m22 = 1.0f - n.y * n.y;
		xform.m23 = -n.y * n.z;
		xform.m31 = -n.z * n.x;
		xform.m32 = -n.z * n.y;
		xform.m33 = 1.0f - n.z * n.z;
		return xform * p;

	}

	Plane::eSide Plane::GetSide( const Vector3& Center, const Vector3& HalfSize ) const
	{
		float Dis = Distance( Center );

		if ( Dis < 0.0 )
			return Plane::NegativeSide;

		if ( Dis > 0.0 )
			return Plane::PositiveSide;

		return Plane::NoSide;
	}

	//----------------------------------------------------------------------------
	// Frustum Implementation
	//----------------------------------------------------------------------------

	Frustum::Frustum( )
	{
	}

	/*     Z
	       | Top
              |   

		   6-----7  Back
		  /|    /|
		 / |   / |
		2-----3  |
		|  4--|--5     -->X
Left	| /   | /
		|/    |/  Right
		0-----1
            |
          Bottom
	   /
      Y
       Front
	*/

	Frustum::Frustum( const Matrix4& viewtransform, const Matrix4& projectiontransform )
	{
		Matrix4 inversematrix = ( viewtransform * projectiontransform ).Inverse( );

		mVectors[0] = Vector3( -1.0f, -1.0f, 0.0f ) * inversematrix;
		mVectors[1] = Vector3(  1.0f, -1.0f, 0.0f ) * inversematrix;
		mVectors[2] = Vector3( -1.0f,  1.0f, 0.0f ) * inversematrix;
		mVectors[3] = Vector3(  1.0f,  1.0f, 0.0f ) * inversematrix;
		mVectors[4] = Vector3( -1.0f, -1.0f, 1.0f ) * inversematrix;
		mVectors[5] = Vector3(  1.0f, -1.0f, 1.0f ) * inversematrix;
		mVectors[6] = Vector3( -1.0f,  1.0f, 1.0f ) * inversematrix;
		mVectors[7] = Vector3(  1.0f,  1.0f, 1.0f ) * inversematrix;

		mPlanes[FrustumPlane_Front]		= Plane( mVectors[0], mVectors[1], mVectors[2] ); // Front
		mPlanes[FrustumPlane_Back]		= Plane( mVectors[6], mVectors[7], mVectors[5] ); // Back
		mPlanes[FrustumPlane_Left]		= Plane( mVectors[2], mVectors[6], mVectors[4] ); // Left
		mPlanes[FrustumPlane_Right]		= Plane( mVectors[7], mVectors[3], mVectors[5] ); // Right
		mPlanes[FrustumPlane_Top]		= Plane( mVectors[2], mVectors[3], mVectors[6] ); // Top
		mPlanes[FrustumPlane_Bottom]	= Plane( mVectors[1], mVectors[0], mVectors[4] ); // Bottom
	}


	float Frustum::Depth( ) const
	{
		return Math::Abs( mPlanes[1].Distance( mVectors[0] ) );
	}


	//----------------------------------------------------------------------------
	// PlaneBoundedVolume Implementation
	//----------------------------------------------------------------------------
	PlaneBoundedVolume::PlaneBoundedVolume( const Ray& RayTopLeft, const Ray& RayTopRight, const Ray& RayBottomLeft, const Ray& RayBottomRight, float FrontDistance, float BackDistance )
	{

		mVectors[0] = RayBottomLeft.DistanceVector( FrontDistance );
		mVectors[1] = RayBottomRight.DistanceVector( FrontDistance );
		mVectors[2] = RayTopLeft.DistanceVector( FrontDistance );
		mVectors[3] = RayBottomRight.DistanceVector( FrontDistance );
		mVectors[4] = RayBottomLeft.DistanceVector( BackDistance );
		mVectors[5] = RayBottomRight.DistanceVector( BackDistance);
		mVectors[6] = RayTopLeft.DistanceVector( BackDistance );
		mVectors[7] = RayTopRight.DistanceVector( BackDistance );

		mPlanes[FrustumPlane_Front]		= Plane( mVectors[2], mVectors[0], mVectors[3] );	// 2, 0, 3
		mPlanes[FrustumPlane_Back]		= Plane( mVectors[5], mVectors[7], mVectors[6] );	// 5, 7, 6
		mPlanes[FrustumPlane_Left]		= Plane( mVectors[2], mVectors[6], mVectors[4] );	// 2, 6, 4
		mPlanes[FrustumPlane_Right]		= Plane( mVectors[3], mVectors[1], mVectors[5] );	// 3, 1, 5
		mPlanes[FrustumPlane_Top]		= Plane( mVectors[2], mVectors[3], mVectors[7] );	// 2, 3, 7
		mPlanes[FrustumPlane_Bottom]	= Plane( mVectors[0], mVectors[4], mVectors[5] );	// 0, 4, 5

	}

	bool PlaneBoundedVolume::Inside( const AABB& box ) const
	{
		if ( box == AABB::cNullBox )
			return false;

		if ( box == AABB::cMaxBox )
			return true;

		Vector3 Center = box.GetCenter();
		Vector3 HalfSize = box.GetHalfSize( );

		// 此判定适用于选择,必须所有边都在内部
		for ( dword i = 0;i < 6;i ++)
		{
			Plane::eSide Side = mPlanes[i].GetSide( Center, HalfSize );
			if ( Side == Plane::NegativeSide )
				return false;
		}

		return true;
	}

	bool PlaneBoundedVolume::Inside( const Box& box ) const
	{
		// 这种绑定盒判定适用于视锥,有一个即在内部
		_byte outside[8] = { 0 };

		for ( int i = 0; i < 8; i ++ )
		{
			for ( int j = 0; j < 6; j ++ )
			{
				if ( mPlanes[j].Distance( box.mCornerPoint[i] ) < 0.0f )
				{
					outside[i] |= 1 << j;
				}
			}

			if ( outside[i] == 0 )
				return true;
		}

		return ( outside[0] & outside[1] & outside[2] & outside[3] & outside[4] & outside[5] & outside[6] & outside[7] ) == 0;
	}



	Triangle2D::Triangle2D()
	{

	}

	Triangle2D::Triangle2D( const Vector2& p1,const Vector2& p2,const Vector2& p3 )
		: mP1( p1 )
		, mP2( p2 )
		, mP3( p3 )
	{

	}

	bool Triangle2D::Inside( const Vector2& p )
	{
		return (CrossProduct3(mP1,p,mP2)*CrossProduct3(mP3,p,mP2)<0) &&
			   (CrossProduct3(mP2,p,mP1)*CrossProduct3(mP3,p,mP1)<0) &&
			   (CrossProduct3(mP1,p,mP3)*CrossProduct3(mP2,p,mP3)<0);
	}

	// Copy from IndieLib
	bool Triangle2D::Inside2( const Vector2& p )
	{
		
		Vector2 v0 = mP3 - mP1;
		Vector2 v1 = mP2 - mP1;
		Vector2 v2 = p - mP1;

		// Compute dot products
		float dot00 =  Vector2::DotProduct( v0, v0 );
		float dot01 =  Vector2::DotProduct( v0, v1 );
		float dot02 =  Vector2::DotProduct( v0, v2 );
		float dot11 =  Vector2::DotProduct( v1, v1 );
		float dot12 =  Vector2::DotProduct( v1, v2 );

		// Compute barycentric coordinates
		float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is in triangle
		return (u > 0) && (v > 0) && (u + v < 1);
	}

	float Triangle2D::Area()
	{
		float area = ( mP1.x * mP2.y + mP2.x * mP3.y + mP3.x * mP1.y + mP2.x * mP1.y + mP3.x * mP2.y + mP1.x * mP3.y ) * 0.5f;
		return area > 0 ? area :  -area;
	}

	float Triangle2D::CrossProduct3(const Vector2& p1,const Vector2& p2, const Vector2& p0 )
	{
		return (p1.x-p0.x)*(p2.y-p0.y)-(p2.x-p0.x)*(p1.y-p0.y);
	}



	Quadrangle::Quadrangle( )
	{

	}

	Quadrangle::Quadrangle( const Vector2& p1, const Vector2& p2, const Vector2& p3,const Vector2& p4 )
		: mP1( p1 )
		, mP2( p2 )
		, mP3( p3 )
		, mP4( p4 )
	{

	}

	Quadrangle::Quadrangle( Vector2* pointlist )
	{
		::memcpy( mPoint, pointlist, sizeof mPoint );
	}

	bool Quadrangle::Inside( const Vector2& p )
	{
		bool tri1 = Triangle2D( mP1,mP2,mP3).Inside( p );
		bool tri2 = Triangle2D( mP1,mP3,mP4).Inside( p );
		return	 tri1 || tri2;
	}


	bool Segement2D::Intersect( const Vector2& p1, const Vector2& p2,const Vector2& p3, const Vector2& p4 )
	{
		float gradab, gradcd, ycptab, ycptcd, interceptX, intercepty;

		// In order to avoid divisions by zero
		//if (mP1.y == mP2.y)
		//	mP2.y += 0.0001f;

		//if (mP1.x == mP2.x)
		//	mP2.x += 0.0001f;

		//if (seg.mP1.y == seg.mP2.y)
		//	seg.mP2.y += 0.0001f;

		//if (seg.mP1.x == seg.mP2.x)
		//	seg.mP2.x += 0.0001f;

		// Calculates the intersection between the two lines
		gradab = (p1.y - p2.y) / (p1.x - p2.x);
		gradcd = (p3.y - p4.y) / (p3.x - p4.x);

		ycptab = p1.y - p1.x * gradab;
		ycptcd = p3.y - p3.x * gradcd;
		interceptX = (ycptab - ycptcd) / (gradcd - gradab);
		intercepty = (ycptab - (gradab * ycptcd) / gradcd) / (1 - gradab / gradcd);

		// Checking in the intersection is inside the segment
		if (!((interceptX >= p1.x && interceptX <= p2.x) || (interceptX >= p2.x && interceptX <= p1.x)))
			return 0;

		if (!((intercepty >= p1.y && intercepty <= p2.y) || (intercepty >= p2.y && intercepty <= p1.y)))
			return 0;

		if (!((interceptX >= p3.x && interceptX <= p4.x) || (interceptX >= p4.x && interceptX <= p3.x)))
			return 0;

		if (!((intercepty >= p3.y && intercepty <= p4.y) || (intercepty >= p4.y && intercepty <= p3.y)))
			return 0;

		return 1;
	}

	Vector2 Segement2D::GetCrossPoint(const Vector2& p1, const Vector2& p2, const Vector2& q1, const Vector2& q2)
	{
		//必须相交求出的才是线段的交点，但是下面的程序段是通用的

		/*根据两点式化为标准式，进而求线性方程组*/
		Vector2 crossPoint;
		//求x坐标
		float tempLeft = (q2.x - q1.x) * (p1.y - p2.y) - (p2.x - p1.x) * (q1.y - q2.y);
		float tempRight = (p1.y - q1.y) * (p2.x - p1.x) * (q2.x - q1.x) + q1.x * (q2.y - q1.y) * (p2.x - p1.x) - p1.x * (p2.y - p1.y) * (q2.x - q1.x);
		crossPoint.x = tempRight / tempLeft;
		//求y坐标
		tempLeft = (p1.x - p2.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q1.x - q2.x);
		tempRight = p2.y * (p1.x - p2.x) * (q2.y - q1.y) + (q2.x- p2.x) * (q2.y - q1.y) * (p1.y - p2.y) - q2.y * (q1.x - q2.x) * (p2.y - p1.y);
		crossPoint.y = tempRight / tempLeft;

		return crossPoint;
	}
}


