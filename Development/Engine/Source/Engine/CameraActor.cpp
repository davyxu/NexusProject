#include "EnginePCH.h"
#include "NekoEngine_System.h"

namespace NekoEngine
{


	IMPLEMENT_RTTI_CLASS( CameraActor );

	CameraActor::CameraActor( )
	: mUpFixed( false )
	{		
		SetPosition( Vector3(10,-50,50) );
		SetViewTarget( Vector3::cZero );

		//OnObjectTransformed( TT_All );
	}

	void CameraActor::Serialize( BinarySerializer& Ser )
	{
		__super::Serialize( Ser );

		Ser << mCamera;
	}

	const Vector3 CameraActor::cCameraFront = Vector3::cXAxis;

	void CameraActor::OnObjectTransformed( dword TransformType )
	{
		mCamera.SetPosition( GetDerivedPosition() );
		mCamera.SetLook(  GetDerivedRotation() * cCameraFront + GetDerivedPosition()  );

		if ( mUpFixed )
			mCamera.SetUp( Vector3::cZAxis );
		else
			mCamera.SetUp( GetDerivedRotation() * Vector3::cZAxis );
	}


	//////////////////////////////////////////////////////////////////////////
	// ActorControllerComponent
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( ActorControllerComponent )

	ActorControllerComponent::ActorControllerComponent( )
		: mInputDevice( null )
	{

	}

	void ActorControllerComponent::Draw( SceneView* RV )
	{
		//Canvas* CV = RV->GetCanvas();

		//CV->PushTransform( RV->GetCamera()->GetViewMatrix() * RV->GetCamera()->GetProjectMatrix() );


		//CV->DrawLine( mCamera.GetPosition(), mCamera.GetLook() , Color::cGreen );
		//CV->DrawLine( mCamera.GetPosition(), mCamera.GetPosition() + Vector3(mCamera.GetUp()).Normalize() * 50.0f , Color::cGreen );

		//CV->PopTransform();
	}

	//////////////////////////////////////////////////////////////////////////
	// FPSActorControllerComponent
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( FPSActorControllerComponent )



	//@TODO 要调整此方向与模型对应
	const Vector3 CameraFront = Vector3::cXAxis;

	void FPSActorControllerComponent::Tick( float Elapse )
	{
		Point mf = mInputDevice->GetMouseOffset();

		if (mInputDevice->IsMouseDown(MB_Right) )
		{
			if ( mf.x != 0  )
			{				
				Yaw(-mf.x / 360.0f);			
			}

			if (  mf.y != 0 )
			{				
				Pitch( -mf.y / 360.0f );			
			}
		}

		const float MoveSpd = 0.03f;

		if (mInputDevice->IsKeyDown(KB_S))
		{
			Move( Vector3::cXAxis * -Elapse * MoveSpd );
		}

		if (mInputDevice->IsKeyDown(KB_W))
		{			
			Move( Vector3::cXAxis *  Elapse * MoveSpd );
		}

		if (mInputDevice->IsKeyDown(KB_A))
		{
			Move( Vector3::cYAxis * -Elapse * MoveSpd );
		}

		if (mInputDevice->IsKeyDown(KB_D))
		{
			Move( Vector3::cYAxis *  Elapse * MoveSpd );
		}

		if( mInputDevice->IsKeyDown( KB_O ) )
		{
			mOwner->SetViewTarget( Vector3( 0,0,0 ));
		}

		if (mInputDevice->IsKeyDown(KB_Space))
		{
			Move( VectorMapIntoObjectSpace( Vector3::cZAxis  ) * Elapse * MoveSpd );
		}

		if (mInputDevice->IsKeyDown(KB_LeftShift))
		{
			Move( VectorMapIntoObjectSpace( Vector3::cZAxis ) *  -Elapse * MoveSpd );
		}
	}

		/*

		Z
		|  Up
		| /
		|/
        Obj

		*/
	void FPSActorControllerComponent::Yaw( float XDelta )
	{
		// 先将世界Z轴转换到对象变换空间内
		Vector3 AxisZ = VectorMapIntoObjectSpace( Vector3::cZAxis );


		// 再绕这个Z轴转
		mOwner->SetDerivedRotation( mOwner->GetDerivedRotation() * Quaternion().RotationAxis( AxisZ, -XDelta )  );		
	}	

	Vector3 FPSActorControllerComponent::VectorMapIntoObjectSpace( const Vector3& V )
	{
		return V * (-mOwner->GetDerivedRotation() );
	}

	void FPSActorControllerComponent::Pitch(  float YDelta )
	{
		Quaternion RotY;
		RotY.RotationY( -YDelta );

		mOwner->SetDerivedRotation( mOwner->GetDerivedRotation() * RotY  );				
	}	



	void FPSActorControllerComponent::Move( const Vector3& Delta )
	{
		Vector3 Pos = mOwner->GetDerivedRotation() *  Delta;

		mOwner->SetDerivedPosition( mOwner->GetDerivedPosition() + Pos );
	}

	//////////////////////////////////////////////////////////////////////////
	// FocusActorControllerComponent
	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_RTTI_CLASS( FocusActorControllerComponent )
	FocusActorControllerComponent::FocusActorControllerComponent()
		: mFocusTarget( Vector3::cZero )
		, mPhi(  Math::ToRadian( -135.0f ) )
		, mTheta( Math::ToRadian( 30.0f ) )
		, mRadius( 50.0f )
	{
		
	}

	void FocusActorControllerComponent::OnAttach()
	{
		UpdateObjectTransform();
	}

	void FocusActorControllerComponent::Tick( float Elapse )
	{
		Point mf = mInputDevice->GetMouseOffset();

		bool NeedUpdate = false;

		if (mInputDevice->IsMouseDown(MB_Right) )
		{
			const float ScrollSpd = 0.0025f;


			if ( mf.y != 0 )
			{				
				mTheta += mf.y * ScrollSpd;

				NeedUpdate = true;
			}


			if ( mf.x != 0 )
			{				
				mPhi += mf.x * ScrollSpd;

				NeedUpdate = true;
			}

		}

		if ( mInputDevice->IsMouseDown(MB_Left))
		{
			if (  mf.y != 0 )
			{
				mRadius += mf.y* 0.3f;

				if ( mRadius < 0.0f )
					mRadius = 0.0f;

				NeedUpdate = true;
			}
		}


		if ( NeedUpdate )
			UpdateObjectTransform();
	}

	void FocusActorControllerComponent::Phi( float Delta )
	{
		Vector3 ViewDir = mFocusTarget - mOwner->GetDerivedPosition();

		// 先将世界Z轴转换到对象变换空间内
		Vector3 AxisZ = Vector3::cZAxis * (-mOwner->GetDerivedRotation() );

		Quaternion Rot = mOwner->GetDerivedRotation() * Quaternion().RotationAxis( AxisZ, -Delta );

		Vector3 RotHandle = Vector3::cZAxis * Rot * ViewDir.Magnitude();

		mOwner->SetDerivedRotation( Rot );
		mOwner->SetDerivedPosition( mFocusTarget + Vector3( -RotHandle.x, -RotHandle.y, RotHandle.z ) );
		
	}

	void FocusActorControllerComponent::Theta( float Delta )
	{
		Vector3 ViewDir = mFocusTarget - mOwner->GetDerivedPosition();

		Quaternion Rot = mOwner->GetDerivedRotation() * Quaternion().RotationY( -Delta );

		Vector3 RotHandle = Rot.GetXAxis() * ViewDir.Magnitude();

		mOwner->SetDerivedRotation( Rot );
		mOwner->SetDerivedPosition( mFocusTarget - RotHandle );
		
	}

	void FocusActorControllerComponent::UpdateObjectTransform( )
	{
		Quaternion Rot = Quaternion().RotationZ( mPhi ) * Quaternion().RotationY( mTheta );

		mOwner->SetDerivedRotation( Rot );
		mOwner->SetDerivedPosition( mFocusTarget - Vector3::cXAxis * Rot * mRadius );
	}

	

	void FocusActorControllerComponent::SetFocusPoint( const Vector3& Value )
	{
		mFocusTarget = Value;
	}


}