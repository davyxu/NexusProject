#include "EngineDemoPCH.h"


//IRenderDriver* GRenderDriver = null;
IInputDriver* GInputDriver = null;
//ModelMaster GModelMaster;
//ArchiveMaster GArchiveMaster;

DebugFrame::DebugFrame( )
:mShowGrid(true)
{
	this->SetEventHandle(this);
	mCameraController = new FPSCameraController;
	//mCameraController = new ArcBallCameraController;
}

DebugFrame::~DebugFrame( )
{
	SafeDelete( mCameraController );
}

bool DebugFrame::OnFormCreate( )
{	
	GInputDriver = CreateDInput();
	if ( !GInputDriver->Create( this->GetHandle() ) )
		return false;

	return true;
}



bool DebugFrame::OnFormLoop( )
{
	TimeSource::Tick();
	mFPSUnit.Tick();

	float elapse = TimeSource::GetElapsedTime();

	GInputDriver->Tick( elapse );

	mCameraController->Tick( elapse );

	Update( elapse );

	return true;
}

void DebugFrame::OnFormDestroy(void)
{
	SafeRelease( GInputDriver );
}


void DebugFrame::OnKeyUp(eKeyButton button)
{
	switch ( button )
	{
	case FKey_P:
		{
			Application::SetAutoSleep( !Application::GetAutoSleep() );
		}
		break;
	case FKey_G:
		{
			mShowGrid = !mShowGrid;
		}
		break;
	}

}


