#pragma once


// 调试用的基础框架
class DebugFrame :	public WinForm,	
					public WinFormListener
{
public:
	DebugFrame( );

	virtual ~DebugFrame( );

	virtual bool OnFormCreate( );

	virtual void Update( float elapse ) = 0;

	virtual void Render( ) = 0;

	virtual bool OnFormLoop(void);

	virtual void OnFormDestroy(void);

	virtual void OnKeyUp(eKeyButton button);	

public:
	FrameSpeedUnit	mFPSUnit;
	CameraController* mCameraController;
	bool mShowGrid;	
};


extern IInputDriver* GInputDriver;
//extern ModelMaster GModelMaster;
//extern ArchiveMaster GArchiveMaster;