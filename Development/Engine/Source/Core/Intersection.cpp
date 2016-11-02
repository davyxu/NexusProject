#include "CorePCH.h"
#include "NekoCore_Math.h"


namespace NekoEngine
{
	bool Intersection::RayVSPlane( const Ray& ray, const Plane& plane, float& distance )
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

	bool Intersection::RayVSTriangle( const Ray& ray, const Triangle& triangle, float& distance )
	{
		Vector3 edge1 = triangle.mP2 - triangle.mP1;
		Vector3 edge2 = triangle.mP3 - triangle.mP1;

		Vector3 tvec, pvec = Vector3::CrossProduct( ray.GetDirection(), edge2 );

		float det = Vector3::DotProduct( edge1, pvec );

		if ( det >= 0.0f )
		{
			tvec = ray.GetOrigin() - triangle.mP1;
		}
		else
		{
			tvec = triangle.mP1 - ray.GetOrigin();
			det = -det;
		}

		if ( det < Math::cEpsilon )
			return false;

		float u = Vector3::DotProduct( tvec, pvec );
		if( u < 0.0f || u > det )
			return false;

		Vector3 qvec = Vector3::CrossProduct( tvec, edge1 );

		float v = Vector3::DotProduct( ray.GetDirection(), qvec );
		if( v < 0.0f || u + v > det )
			return false;

		distance = Vector3::DotProduct( edge2, qvec ) / det;

		return true;
	}

	bool Intersection::RayVSBox( const Ray& ray, const Box& box, float& distance )
	{
		if ( ray.GetOrigin().x >= box.mCornerPoint[0].x && ray.GetOrigin().x <= box.mCornerPoint[7].x &&
			ray.GetOrigin().y >= box.mCornerPoint[0].y && ray.GetOrigin().y <= box.mCornerPoint[7].y &&
			ray.GetOrigin().z >= box.mCornerPoint[0].z && ray.GetOrigin().z <= box.mCornerPoint[7].z )
		{
			distance = 0.0f;

			return true;
		}

		float tempdistance1 = 0.0f, tempdistance2 = -1.0f;

		if ( RayVSTriangle( ray, Triangle( box.mCornerPoint[0], box.mCornerPoint[1], box.mCornerPoint[2] ), tempdistance1 ) == true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
			tempdistance2 = tempdistance1;
		if ( RayVSTriangle( ray, Triangle( box.mCornerPoint[2], box.mCornerPoint[1], box.mCornerPoint[3] ), tempdistance1 ) == true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
			tempdistance2 = tempdistance1;

		if ( RayVSTriangle( ray, Triangle( box.mCornerPoint[4], box.mCornerPoint[6], box.mCornerPoint[5] ), tempdistance1 ) == true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
			tempdistance2 = tempdistance1;
		if ( RayVSTriangle( ray, Triangle( box.mCornerPoint[5], box.mCornerPoint[6], box.mCornerPoint[7] ), tempdistance1 ) == true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
			tempdistance2 = tempdistance1;

		if ( RayVSTriangle( ray, Triangle( box.mCornerPoint[4], box.mCornerPoint[5], box.mCornerPoint[0] ), tempdistance1 ) == true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
			tempdistance2 = tempdistance1;
		if ( RayVSTriangle( ray, Triangle( box.mCornerPoint[0], box.mCornerPoint[5], box.mCornerPoint[1] ), tempdistance1 ) == true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
			tempdistance2 = tempdistance1;

		if ( RayVSTriangle( ray, Triangle( box.mCornerPoint[1], box.mCornerPoint[5], box.mCornerPoint[4] ), tempdistance1 ) == true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
			tempdistance2 = tempdistance1;
		if ( RayVSTriangle( ray, Triangle( box.mCornerPoint[4], box.mCornerPoint[5], box.mCornerPoint[7] ), tempdistance1 ) == true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
			tempdistance2 = tempdistance1;

		if ( RayVSTriangle( ray, Triangle( box.mCornerPoint[2], box.mCornerPoint[3], box.mCornerPoint[6] ), tempdistance1 ) == true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
			tempdistance2 = tempdistance1;
		if ( RayVSTriangle( ray, Triangle( box.mCornerPoint[6], box.mCornerPoint[3], box.mCornerPoint[7] ), tempdistance1 ) == true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
			tempdistance2 = tempdistance1;

		if ( RayVSTriangle( ray, Triangle( box.mCornerPoint[4], box.mCornerPoint[0], box.mCornerPoint[6] ), tempdistance1 ) == true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
			tempdistance2 = tempdistance1;
		if ( RayVSTriangle( ray, Triangle( box.mCornerPoint[6], box.mCornerPoint[0], box.mCornerPoint[2] ), tempdistance1 ) == true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
			tempdistance2 = tempdistance1;

		if ( tempdistance2 < 0.0f )
			return false;

		distance = tempdistance2;

		return true;
	}

	bool Intersection::RayVSAABox( const Ray& ray, const AABB& box, float& distance )
	{
		return RayVSBox( ray, Box( box ), distance );
	}


	void Intersection::IterateMeshVertex( dword VertexSize, ePrimitiveType PrimitiveType, dword PrimitiveCount, const _byte* VertexBuffer, const _byte* IndexBuffer, IterateVertexCallback Func, void* UserData )
	{
		if ( VertexBuffer == null )
			return;

		for ( dword i = 0; i < PrimitiveCount; i ++ )
		{
			dword a = 0, b = 0, c = 0;

			switch ( PrimitiveType )
			{
			case TriangleList:
				{
					if ( IndexBuffer )
					{
						a = ( (word*) IndexBuffer )[ i * 3 + 0 ];
						b = ( (word*) IndexBuffer )[ i * 3 + 1 ];
						c = ( (word*) IndexBuffer )[ i * 3 + 2 ];
					}
					else
					{
						a = i * 3 + 0;
						b = i * 3 + 1;
						c = i * 3 + 2;
					}

					break;
				}

			case TriangleStrip:
				{
					if ( IndexBuffer )
					{
						a = ( (word*) IndexBuffer )[ i + 0 ];
						b = ( (word*) IndexBuffer )[ i + 1 + ( i % 2 ) ];
						c = ( (word*) IndexBuffer )[ i + 2 - ( i % 2 ) ];
					}
					else
					{
						a = i + 0;
						b = i + 1 + ( i % 2 );
						c = i + 2 - ( i % 2 );
					}
					
					break;
				}
			}

			const Vector3& vector1 = *( (Vector3*) ( (_byte*) VertexBuffer + a * VertexSize ) ); 
			const Vector3& vector2 = *( (Vector3*) ( (_byte*) VertexBuffer + b * VertexSize ) );
			const Vector3& vector3 = *( (Vector3*) ( (_byte*) VertexBuffer + c * VertexSize ) );

			if ( !Func( UserData, vector1, vector2, vector3 ) ) 
				break;
		}
	}

	struct RayVSMeshIterateData 
	{
		const Ray& mRay;
		float mTempDistance;
		float mNearest;
	};

	static bool RayVSMeshIterateVertexCallback( void* UserData, const Vector3& P1, const Vector3& P2, const Vector3& P3 )
	{
		RayVSMeshIterateData* TUserData = (RayVSMeshIterateData*)UserData;

		if ( Intersection::RayVSTriangle( TUserData->mRay, Triangle( P1, P2, P3 ), TUserData->mTempDistance ) == true )
		{
			if ( TUserData->mNearest < 0.0f || TUserData->mNearest > TUserData->mTempDistance )
				TUserData->mNearest = TUserData->mTempDistance;
		}

		return true;
	}



	bool Intersection::RayVSMesh( const Ray& TRay, float& Distance , dword VertexSize, ePrimitiveType PType, dword PrimitiveCount, const _byte* VertexBuffer, const _byte* Indexbuffer )
	{
		RayVSMeshIterateData UserData = { TRay, 0.0f, -1.0f };

		IterateMeshVertex( VertexSize, PType, PrimitiveCount, VertexBuffer, Indexbuffer, RayVSMeshIterateVertexCallback, &UserData );

		if ( UserData.mNearest >= 0.0f )
			Distance = UserData.mNearest;

		return UserData.mNearest >= 0.0f;
	}

	struct RectVSMeshIterateData 
	{
		Camera* mCamera;
		const FRect& mScreenSpaceRect;
		float mBackDistanceSquared;
		bool mAnyVertexInBound;
		Matrix4 mLocalToWorld;
	};

	static bool RectVSMeshIterateVertexCallback( void* UserData, const Vector3& P1, const Vector3& P2, const Vector3& P3 )
	{
		RectVSMeshIterateData* TUserData = (RectVSMeshIterateData*)UserData;

		const Vector3* PosArray[] = { &P1, &P2, &P3 };

		for ( dword i = 0;i< ArraySize(PosArray);i++ )
		{
			const Vector3& VertexPos = *PosArray[i];
			//if ( (VertexPos - TUserData->mCamera->GetPosition() ).MagnitudeSquared() > TUserData->mBackDistanceSquared  )
			//	continue;

			// 本地->世界->摄像机空间
			Vector3 EyeSpacePos = VertexPos * TUserData->mLocalToWorld * TUserData->mCamera->GetViewMatrix() ;

			// 在摄像机背后
			if ( EyeSpacePos.z < 0 )
				continue;

			// 摄像机->屏幕空间
			Vector3 ScreenSpacePos = EyeSpacePos * TUserData->mCamera->GetProjectMatrix() ;

			// 顶点投射到屏幕上,是否在给定范围内
			if ( ScreenSpacePos.x > TUserData->mScreenSpaceRect.left &&
				 ScreenSpacePos.x < TUserData->mScreenSpaceRect.right &&
				 ScreenSpacePos.y > TUserData->mScreenSpaceRect.top &&
				 ScreenSpacePos.y > TUserData->mScreenSpaceRect.bottom )
			{
				TUserData->mAnyVertexInBound = true;
				break;
			}


		}

		return !TUserData->mAnyVertexInBound;
	}


	bool Intersection::RectVSMesh( const FRect& ScreenArea, Camera* Cam, const Matrix4& LocalToWorld, float BackDistance, dword VertexSize, ePrimitiveType PType, dword PrimitiveCount, const _byte* VertexBuffer, const _byte* Indexbuffer )
	{
		// 从左上角原点转换为中心原点
		FRect ScreenSpaceRect;
		ScreenSpaceRect.left = 2.0f * ScreenArea.left - 1.0f;
		ScreenSpaceRect.right = 2.0f * ScreenArea.right - 1.0f;
		ScreenSpaceRect.top = 2.0f * (1 - ScreenArea.top) - 1.0f;
		ScreenSpaceRect.bottom = 2.0f * (1 - ScreenArea.bottom) - 1.0f;

		// 避免左右上下颠倒
		ScreenSpaceRect.left = Math::Min( ScreenSpaceRect.left, ScreenSpaceRect.right );
		ScreenSpaceRect.right = Math::Max( ScreenSpaceRect.left, ScreenSpaceRect.right );
		ScreenSpaceRect.top = Math::Min( ScreenSpaceRect.top, ScreenSpaceRect.bottom );
		ScreenSpaceRect.bottom = Math::Max( ScreenSpaceRect.top, ScreenSpaceRect.bottom );

		RectVSMeshIterateData UserData = { Cam, ScreenSpaceRect, Math::Square( BackDistance ), false, LocalToWorld };

		IterateMeshVertex( VertexSize, PType, PrimitiveCount, VertexBuffer, Indexbuffer, RectVSMeshIterateVertexCallback, &UserData );

		return UserData.mAnyVertexInBound;
	}



	bool Intersection::AABoxVSAABox( const AABB& box1, const AABB& box2 )
	{
		float x1 = Math::Max( box1.mMinVector.x, box2.mMinVector.x );
		if ( x1 >= box1.mMaxVector.x || x1 >= box2.mMaxVector.x )
			return false;

		float x2 = Math::Min( box1.mMaxVector.x, box2.mMaxVector.x );
		if ( x2 <= box1.mMinVector.x || x2 <= box2.mMinVector.x )
			return false;

		float y1 = Math::Max( box1.mMinVector.y, box2.mMinVector.y );
		if ( y1 >= box1.mMaxVector.y || y1 >= box2.mMaxVector.y )
			return false;

		float y2 = Math::Min( box1.mMaxVector.y, box2.mMaxVector.y );
		if ( y2 <= box1.mMinVector.y || y2 <= box2.mMinVector.y )
			return false;

		float z1 = Math::Max( box1.mMinVector.z, box2.mMinVector.z );
		if ( z1 >= box1.mMaxVector.z || z1 >= box2.mMaxVector.z )
			return false;

		float z2 = Math::Min( box1.mMaxVector.z, box2.mMaxVector.z );
		if ( z2 <= box1.mMinVector.z || z2 <= box2.mMinVector.z )
			return false;

		return true;
	}

	bool Intersection::SphereVSSphere( const Sphere& sphere1, const Sphere& sphere2 )
	{
		return Math::Abs( sphere1.mRadius ) + Math::Abs( sphere2.mRadius ) >
			( sphere1.mCenter - sphere2.mCenter ).Magnitude( );
	}

	bool Intersection::StaticAABoxVSPlane( const AABB& box, const Plane& plane )
	{
		float d1 = plane.Distance( Vector3( box.mMinVector.x, box.mMinVector.y, box.mMinVector.z ) );
		float d2 = plane.Distance( Vector3( box.mMaxVector.x, box.mMaxVector.y, box.mMaxVector.z ) );

		float t = d1 * d2;
		if ( t <= Math::cEpsilon )
			return true;

		t *= plane.Distance( Vector3( box.mMaxVector.x, box.mMinVector.y, box.mMinVector.z ) );
		if ( t <= Math::cEpsilon )
			return true;

		t *= plane.Distance( Vector3( box.mMinVector.x, box.mMaxVector.y, box.mMinVector.z ) );
		if ( t <= Math::cEpsilon )
			return true;

		t *= plane.Distance( Vector3( box.mMaxVector.x, box.mMaxVector.y, box.mMinVector.z ) );
		if ( t <= Math::cEpsilon )
			return true;

		t *= plane.Distance( Vector3( box.mMinVector.x, box.mMinVector.y, box.mMaxVector.z ) );
		if ( t <= Math::cEpsilon )
			return true;

		t *= plane.Distance( Vector3( box.mMaxVector.x, box.mMinVector.y, box.mMaxVector.z ) );
		if ( t <= Math::cEpsilon )
			return true;

		t *= plane.Distance( Vector3( box.mMinVector.x, box.mMaxVector.y, box.mMaxVector.z ) );
		if ( t <= Math::cEpsilon )
			return true;

		return false;
	}

	bool Intersection::DynamicAABoxVSPlane( const AABB& box, const Plane& plane, const Vector3& direction, float& distance )
	{
		distance = 0.0f;

		float t, d[8]; Vector3 vectors[8];
		d[0] = plane.Distance( vectors[0] = Vector3( box.mMinVector.x, box.mMinVector.y, box.mMinVector.z ) );
		d[1] = plane.Distance( vectors[1] = Vector3( box.mMaxVector.x, box.mMaxVector.y, box.mMaxVector.z ) );

		if ( ( t = d[0] * d[1] ) <= Math::cEpsilon )
			return true;

		d[2] = plane.Distance( vectors[2] = Vector3( box.mMaxVector.x, box.mMinVector.y, box.mMinVector.z ) );
		if ( ( t *= d[2] ) <= Math::cEpsilon )
			return true;

		d[3] = plane.Distance( vectors[3] = Vector3( box.mMinVector.x, box.mMaxVector.y, box.mMinVector.z ) );
		if ( ( t *= d[3] ) <= Math::cEpsilon )
			return true;

		d[4] = plane.Distance( vectors[4] = Vector3( box.mMaxVector.x, box.mMaxVector.y, box.mMinVector.z ) );
		if ( ( t *= d[4] ) <= Math::cEpsilon )
			return true;

		d[5] = plane.Distance( vectors[5] = Vector3( box.mMinVector.x, box.mMinVector.y, box.mMaxVector.z ) );
		if ( ( t *= d[5] ) <= Math::cEpsilon )
			return true;

		d[6] = plane.Distance( vectors[6] = Vector3( box.mMaxVector.x, box.mMinVector.y, box.mMaxVector.z ) );
		if ( ( t *= d[6] ) <= Math::cEpsilon )
			return true;

		d[7] = plane.Distance( vectors[7] = Vector3( box.mMinVector.x, box.mMaxVector.y, box.mMaxVector.z ) );
		if ( ( t *= d[7] ) <= Math::cEpsilon )
			return true;

		// Not intersecte, find the nearest point.
		dword n = 0; t = Math::Abs( d[0] );
		for ( dword i = 1; i < 8; i ++ )
		{
			if ( Math::Abs( d[i] ) < t )
			{
				t = Math::Abs( d[i] );
				n = i;
			}
		}

		// Find the distance from the point to the plane.
		return RayVSPlane( Ray( vectors[n], direction ), plane, distance );
	}

	bool Intersection::StaticSphereVSPlane( const Sphere& sphere, const Plane& plane )
	{
		return Math::Abs( plane.Distance( sphere.mCenter ) ) <= Math::Abs( sphere.mRadius );
	}

	bool Intersection::DynamicSphereVSPlane( const Sphere& sphere, const Plane& plane, const Vector3& direction, float& distance )
	{
		distance = 0.0f;

		if ( Math::Abs( plane.Distance( sphere.mCenter ) ) <= Math::Abs( sphere.mRadius ) )
			return true;

		return RayVSPlane( Ray( sphere.mCenter, direction ), Plane( plane.a, plane.b, plane.c, plane.d - Math::Abs( sphere.mRadius ) ), distance );
	}
}