#include "EnginePCH.h"
#include "NekoEngine_Render.h"
#include "NekoEngine_System.h"
#include "Core/Win32Header.h"

namespace NekoEngine
{
	PresentWindow::PresentWindow()
		: mRHIVideoDevice( null )
		
	{

	}

	PresentWindow::~PresentWindow()
	{
		if ( mRHIVideoDevice )
			mRHIVideoDevice->Release();
	}

	dword PresentWindow::GetWidth()
	{
		if ( mRHIVideoDevice == null )
			return 0;

		return mRHIVideoDevice->GetWidth();
	}

	dword PresentWindow::GetHeight()
	{
		if ( mRHIVideoDevice == null )
			return 0;

		return mRHIVideoDevice->GetHeight();
	}

	NekoEngine::handle PresentWindow::GetWindowHandle()
	{
		if ( mRHIVideoDevice == null )
			return null;

		return mRHIVideoDevice->GetWindowHandle();
	}

	void PresentWindow::Draw()
	{
		if ( !mEnableDraw )
			return;

		// BeginScene/EndScene只能有一对
		ENQUEUE_UNIQUE_RENDER_COMMAND(BeginDrawCommand, { 

			RHI->BeginDraw();
		} );

		// 设置BackBuffer并且清空
		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(ClearCommand,PresentWindow*,mRenderWindow, this, { 

			RHI->RestoreRenderTarget( 0 );

			RHI->Clear( true, mRenderWindow->GetClearColor() , true, 1.0f, false, 0 );
		} );
		
		// 绘制所有的SceneView
		DrawSceneView();


		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(DrawDebugStatus, PresentWindow*,mRenderWindow, this, { 

			DrawDebugStatus_RenderThread( mRenderWindow );
		} );


	}

	void PresentWindow::DrawDebugStatus_RenderThread( PresentWindow* RW )
	{
		// 恢复BackBuffer绘制，并恢复Viewport
		RHI->RestoreRenderTarget( 0 );

		Point MousePt = RW->GetInputDriver()->GetMousePoint();


		RHI->DrawDebugText(
			5,
			5,
			Color::cGreen,
			L"FPS: %d  mouse: %d,%d RenderThread:%d  Primitives:%d Vertices:%d", 
			GEngine->mFPSUnit->GetFramePerSecond(), 
			MousePt.x,
			MousePt.y,
			GRenderThread.mEnable,
			GRenderStatus.mNumPrimitives,
			GRenderStatus.mNumVertices
			);

		RHI->EndDraw();

		RW->GetRHIVideoDevice()->Present();
	}

	void PresentWindow::Resize( dword Width, dword Height )
	{
		mRHIVideoDevice->Resize( Width, Height, false );

		// 更新所有RT的及View的大小
		GEngine->UpdateRenderTarget();

		mRHIVideoDevice->UpdateDevice();
	}


	ExternWindow::ExternWindow( void* Window, bool IsFullScreen )
	{
		Rect rc;
		::GetClientRect((HWND)Window,(RECT*)&rc);

		mRHIVideoDevice = RHI->CreateVideoDevice( Window,rc.right - rc.left,rc.bottom - rc.top , IsFullScreen );

		mInputDriver = new MessageInputDriver( GetWindowHandle() );
	}

	ExternWindow::~ExternWindow()
	{
		delete mInputDriver;
	}

	GameWindow::GameWindow( const wchar* Title, dword Width, dword Height, bool IsFullScreen, dword IconID )
	{
		mForm = new WinForm;

		mForm->Create( Title, Point( Width, Height ), !IsFullScreen, NULL, NULL,  IconID );

		// 创建D3D设备
		mRHIVideoDevice = RHI->CreateVideoDevice( mForm->GetHandle(), Width, Height , IsFullScreen );

		Assert( mRHIVideoDevice != null );		

		mForm->SetEventHandle( this );

		mInputDriver = new MessageInputDriver( GetWindowHandle() );
	}

	GameWindow::~GameWindow( )
	{
		delete mForm;
		delete mInputDriver;
	}

	void GameWindow::Go( )
	{
		Assert( GEngine != null );


		// 主循环
		while( Application::DoEvents( ) )
		{
			if ( mInputDriver->IsKeyUp( KB_Escape ) )
			{
				mForm->Close();

				break;
			}

			GEngine->Tick();
		}
	}

	void GameWindow::OnWindowSizeChanged( Point beforeSize,Point afterSize )
	{
		Resize( afterSize.x, afterSize.y );
	}



	void GameWindow::OnWindowStateChanged()
	{

		switch( mForm->GetWindowState() )
		{
		case WS_Maxmized:
		case WS_Normal:
			{
				mRHIVideoDevice->Resize( mForm->GetSize().x ,mForm->GetSize().y, false );
				// 更新所有RT的及View的大小
				GEngine->UpdateRenderTarget();
				mRHIVideoDevice->UpdateDevice();
			}
			break;
		case WS_Minmized:
		case WS_Hide:
			{
				//mViewport->SetEnableDraw( false );
			}
			break;
		}

	}


	void GameWindow::OnKeyUp(eKeyButton Button)
	{

		mInputDriver->NotifyKeyState( Button, MKS_Up );
	}

	void GameWindow::OnMouseMove( int x,int y,eMouseButton button )
	{


		mInputDriver->NotifyMousePos( Point( x, y ) );
	}

	void GameWindow::OnMouseUp( int x,int y,eMouseButton button )
	{


		mInputDriver->NotifyMouseState( button, MKS_Up );
	}

	void GameWindow::OnMouseDown( int x,int y,eMouseButton button )
	{
		mInputDriver->NotifyMouseState( button, MKS_Down );
	}

	void GameWindow::OnKeyDown( eKeyButton button )
	{
		mInputDriver->NotifyKeyState( button, MKS_Down );
	}

	void GameWindow::OnMouseWheel( int z ,eMouseButton button )
	{

		mInputDriver->NotifyMouseScroll( z );
	}

	bool GameWindow::OnFormClose()
	{
		return true;
	}











}