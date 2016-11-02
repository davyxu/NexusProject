#include "EnginePCH.h"
#include "NekoInput.h"
#include "NekoEngine_Render.h"



namespace NekoEngine
{

	CameraController::CameraController( )
		: mInputDev( null )
		, mCamera( null )
	{

	}

	void CameraController::AttachInputDevice( IInputDriver* driver )
	{
		mInputDev = driver;
	}

	void CameraController::AttachCamera( Camera* camera )
	{
		mCamera = camera;
	}


	void ArcBallCameraController::Tick(float elapse )
	{
		if ( mCamera == null )
			return;

		const float ScrollSpd = 0.0025f;

		float distance = mCamera->GetViewDir().Magnitude();

		Point mf = mInputDev->GetMouseOffset();

		if (mInputDev->IsMouseDown(MB_Middle))
		{
			if (mInputDev->IsKeyDown(KB_LeftAtl))
			{
				mCamera->Phi(mf.x * ScrollSpd );
				mCamera->Theta(mf.y * ScrollSpd );
			}
			else
			{
				// ¾Û½¹µã

				const float PanSpd = 0.0005f * distance;

				mCamera->Strafe(mf.x * PanSpd );
				mCamera->Fly(mf.y * PanSpd );
			}

		}


		const float MouseScrollerMoveSpeed = 0.1f ;

		int data = mInputDev->GetMouseScroller()/120;
		if ( data != 0)
		{
			mCamera->Radius(-data * MouseScrollerMoveSpeed * distance );
		}
	}

	void FPSCameraController::Tick( float elapse )
	{
		Assert( mCamera != null );
		Assert( mInputDev != null );


		const float MouseScrollerMoveSpeed = 5.0f ;
		const float MoveSpd = 0.03f;

		Point mf = mInputDev->GetMouseOffset();

		if (mInputDev->IsMouseDown(MB_Right) )
		{
			mCamera->Yaw(-mf.x / 360.0f);
			mCamera->Pitch(-mf.y / 360.0f);
			mCamera->SetUp( Vector3::cZAxis );
		}


		int data = mInputDev->GetMouseScroller();
		if ( data != 0)
		{
			float distance = mCamera->GetViewDir().Magnitude();
			mCamera->Zoom(-data/MouseScrollerMoveSpeed * distance * 0.003f);
		}


		// mCamera position

		float spdAdj = TimeSource::GetElapsedTime() * MoveSpd;

		if (mInputDev->IsKeyDown(KB_S))
		{
			mCamera->Zoom(spdAdj);
		}

		if (mInputDev->IsKeyDown(KB_W))
		{
			mCamera->Zoom(-spdAdj);
		}

		if (mInputDev->IsKeyDown(KB_A))
		{
			mCamera->Strafe(spdAdj);
		}

		if (mInputDev->IsKeyDown(KB_D))
		{
			mCamera->Strafe(-spdAdj);
		}

		if (mInputDev->IsKeyDown(KB_Space))
		{
			mCamera->FlyStaticUp(spdAdj);
		}

		if (mInputDev->IsKeyDown(KB_LeftShift))
		{
			mCamera->FlyStaticUp(-spdAdj);
		}

		if (mInputDev->IsMouseDown(MB_Middle))
		{
			mCamera->SetPosition( CameraStartPos );
			mCamera->SetLook( Vector3::cZero );
		}
	}

	FocusCameraController::FocusCameraController()
		: mZooming( false )
	{

	}


	void FocusCameraController::Tick( float elapse )
	{
		const float ScrollSpd = 0.0025f;

		Point mf = mInputDev->GetMouseOffset();

		if (mInputDev->IsMouseDown(MB_Left))
		{
			mCamera->Phi(mf.x * ScrollSpd );
			mCamera->Theta(mf.y * ScrollSpd );			
		}

		if ( mZooming )
		{
			if ( !mInputDev->IsMouseDown(MB_Right) )
			{
				mZooming = false;
			}

			const float ZoomSpd = 0.1f;
			mCamera->SetPosition( mSavedPos );
			mCamera->Radius( (mInputDev->GetMousePoint().y - mSavedMouseY) * ZoomSpd );
		}
		else
		{
			if ( mInputDev->IsMouseDown(MB_Right) )
			{
				mZooming = true;
				mSavedMouseY = (float)mInputDev->GetMousePoint().y;
				mSavedPos = mCamera->GetPosition();
			}
		}



	}


}