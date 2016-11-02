#include "CorePCH.h"
#include "NekoCore_Math.h"
#include "NekoCore_Stream.h"
#include "NekoCore_Reflection.h"


namespace NekoEngine
{

	const Vector3 StartPos = Vector3(10,-50,50);

	const Vector3 UpVector = Vector3::cZAxis;
	

	Camera::Camera( ) 
		: mPosition(StartPos)
		, mLook(Vector3::cZero)
		, mUp(Vector3::cZAxis)
		, mViewMatrixDirty( true )
	{

	}


	Vector3 Camera::GetViewDir( )
	{
		return  mPosition - mLook;
	}

	Frustum Camera::CreateFrustum( ) const
	{
		return Frustum(mViewMatrix,mProjectMatrix);
	}



	void Camera::Yaw(float angle)
	{
		Vector3 vDir  = GetViewDir();

		float len = vDir.Magnitude();
		vDir.Normalize();

		Matrix4 matRot;
		matRot.RotationAxis(mUp, angle*CameraCoordFix);

		Vector3 newDir = vDir * matRot;

		newDir = newDir * len;
		mLook = mPosition + newDir;

		mViewMatrixDirty = true;
	}

	void Camera::Pitch(float radian)
	{
		Vector3 vDir  = GetViewDir();

		float len = vDir.Magnitude();
		vDir.Normalize();
		Vector3 vRight = Vector3::CrossProduct(vDir,mUp);

		Vector3 newDir;
		Vector3 newUp;

		Matrix4 matRot;
		matRot.RotationAxis(vRight, radian);

		newDir = vDir * matRot;
		newUp =  mUp * matRot;
		mUp = newUp;
		newDir = newDir * len;
		mLook = mPosition + newDir;

		mViewMatrixDirty = true;
	}

	void Camera::Roll(float radian)
	{
		Vector3 vDir  = GetViewDir();
		vDir.Normalize();

		Vector3 newUp;

		Matrix4 matRot;
		matRot.RotationAxis(vDir, radian);

		newUp = mUp * matRot;
		mUp = newUp;

		mViewMatrixDirty = true;
	}

	void Camera::Strafe(float units)
	{
		Vector3 vDir  = GetViewDir();
		Vector3 vRight = Vector3::CrossProduct(vDir.Normalize(),mUp);
		vRight.Normalize();

#ifdef USE_R_HAND_COORDSYS
		mLook	+= vRight * units;
		mPosition	+= vRight * units;
#else
		mLook	-= vRight * units;
		mPosition	-= vRight * units;
#endif

		mViewMatrixDirty = true;
	}

	void Camera::FlyStaticUp(float units)
	{
		mLook	+= mUp * units;
		mPosition	+= mUp * units;

		mViewMatrixDirty = true;
	}

	void Camera::Fly(float units)
	{
		Vector3 vDir  = GetViewDir();
		Vector3 vRight = Vector3::CrossProduct(vDir.Normalize(),mUp);
		vRight.Normalize();

		Vector3 vUp = -Vector3::CrossProduct(vDir,vRight);
		vUp.Normalize();

		mLook	+= vUp * units;
		mPosition	+= vUp * units;

		mViewMatrixDirty = true;
	}

	void Camera::LookAt( const Vector3& look )
	{
		Vector3 temp = mLook;

		mLook		= look;
		mPosition	= mLook + ( mPosition - temp );

		mViewMatrixDirty = true;
	}
	

	void Camera::Zoom(float units)
	{
		Vector3 viewdir = GetViewDir();
		Vector3 offset =  Vector3(viewdir).Normalize() * units;

		mPosition += offset;
		mLook +=  offset;

		mViewMatrixDirty = true;
	}

	void Camera::Radius( float units )
	{
		Vector3 position = mPosition + ( mPosition - mLook ).Normalize( ) * units;

		if ( Vector3::DotProduct( mPosition - mLook, position - mLook ) > 0.0f )
		{
			mPosition = position;
			mViewMatrixDirty = true;
		}
	}

	void Camera::Phi( float radian )
	{
		mPosition *=	Matrix4( ).Translation( - mLook ) *
					Matrix4( ).RotationAxis( mUp, radian ) * 
					Matrix4( ).Translation( mLook );

		mViewMatrixDirty = true;
	}

	void Camera::Theta( float radian )
	{
		Vector3 vDir  = GetViewDir();
		Vector3 vRight = Vector3::CrossProduct(vDir.Normalize(),mUp);
		vRight.Normalize();

		Vector3 vector1 = Vector3::CrossProduct( mPosition - mLook, vRight );

		Matrix4 matRot;
		matRot.RotationAxis(vRight, radian);

		mPosition *= Matrix4( ).Translation( - mLook ) * matRot * Matrix4( ).Translation( mLook );

		Vector3 vector2 = Vector3::CrossProduct( mPosition - mLook, vRight );

		// µ¹ÖÃºó·­×ªÒ»È¦
		if ( Vector3::DotProduct( vector1, mUp ) * Vector3::DotProduct( vector2, mUp ) < 0.0f )
			mUp = - mUp;

		mViewMatrixDirty = true;
	}

	void Camera::SetUp( const Vector3& Value )
	{
		mUp = Value;
		mViewMatrixDirty = true;
	}

	void Camera::SetLook( const Vector3& Value )
	{
		mLook = Value;
		mViewMatrixDirty = true;
	}

	void Camera::SetPosition( const Vector3& Value )
	{
		mPosition = Value;
		mViewMatrixDirty = true;
	}

	const Matrix4& Camera::GetViewMatrix() const
	{
		UpdateViewMatrix();

		return mViewMatrix;
	}

	void Camera::Update( const Viewport& vp )
	{
		UpdateViewMatrix();

		mProjectMatrix = Matrix4::PerspectiveFovLH( Math::cPi/4, (float)vp.Width / vp.Height , 0.1f, vp.FarZ );
	}

	void Camera::UpdateViewMatrix( ) const
	{
		if ( mViewMatrixDirty )
		{
			mViewMatrix = Matrix4::LookAtLH( mPosition, mLook, mUp );

			mViewMatrixDirty = false;
		}
	}

	void Camera::Serialize( BinarySerializer& Ser )
	{
		Ser << mPosition 
			<< mLook 
			<< mUp;
	}
}