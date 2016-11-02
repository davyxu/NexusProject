#include "EnginePCH.h"
#include "NekoEngine_Render.h"
#include "NekoEngine_Extend.h"

namespace NekoEngine
{


	//////////////////////////////////////////////////////////////////////////
	// CameraControllerComponent
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( CameraControllerComponent)
		CameraControllerComponent::CameraControllerComponent()
		: mController( null )
	{

	}

	CameraControllerComponent::~CameraControllerComponent()
	{
		delete mController;
	}

	void CameraControllerComponent::Draw( SceneView* RV )
	{

	}

	void CameraControllerComponent::SetControllerType( CameraController* Controller, IInputDriver* InputDrv )
	{
		if ( mController )
			delete mController;

		mController = Controller;

		CameraActor* ACamera = mOwner->Cast<CameraActor>();

		Assert( ACamera != null );

		Assert( mController != null );
		Assert( InputDrv != null );

		mController->AttachCamera( &ACamera->mCamera );
		mController->AttachInputDevice( InputDrv );
	}


	void CameraControllerComponent::Tick( float Elapse )
	{
		Assert( mController != null );

		mController->Tick( Elapse );
	}





}