#include "D3D9PCH.h"
#include "Engine/RenderStateWrapper.h"

namespace NekoEngine
{
	Logger* D3D9Logger;

	bool GIsRHIInitialized = false;

	/** The global D3D interface. */
	IDirect3D9* GDirect3D;

	/** The global D3D device. */
	IDirect3DDevice9* GDirect3DDevice;

	bool GD3DDeviceLost = false;

	bool GD3DIsFullscreenDevice = false;

	Point GD3DDeviceSize = Point::cZero;

	D3D9VideoDevice* GD3DFullscreenViewport = NULL; 


	extern ID3DXFont* GDebugFont;

	//FSurfaceRHIRef GD3DBackBuffer;
	HWND GD3DDeviceWindow = NULL;

	IRenderHardware* RHI = null;

	IDirect3DSurface9* GD3DBackBuffer = null;


	void OnInitDeviceState( )
	{
		TStaticBlendState<>::Commit();
		TStaticDepthState<>::Commit();
		TStaticRasterizerState<>::Commit();
		TStaticStencilState<>::Commit();
	}

	void CheckD3D( )
	{
		if ( !D3D9Logger )
		{
			D3D9Logger = new SimpleLog;
			D3D9Logger->SetPrefix(L"D3DVideoDriver");
		}

		if ( !GDirect3D )
		{
			GDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
		}

		if ( !GDirect3D )
		{
			D3D9Logger->Debug( L"Please install DirectX 9.0c or later" );
		}

	}

	extern void RHIUnloadVertexFormat( );

	extern void RHIReleaseShader( );

	void RHIInitialize( )
	{
		RHI = new D3D9RHI;
	}

	void RHIFinalize( )
	{
		SafeDelete( RHI );
	}

	void CleanupD3DDevice( )
	{
		if ( GIsRHIInitialized )
		{
			TLinkedList<RenderResource*> *&ResourceList = RenderResource::GetResourceList();

			D3D9Logger->Info(L"Release RHI Render Resource...");

			for(TLinkedList<RenderResource*>::TIterator ResourceIt(ResourceList);ResourceIt;ResourceIt.Next())
			{
				ResourceIt->ReleaseRHI();
			}

			for(TLinkedList<RenderResource*>::TIterator ResourceIt(ResourceList);ResourceIt;ResourceIt.Next())
			{
				ResourceIt->ReleaseDynamicRHI();
			}

			RHIUnloadVertexFormat( );

			RHIReleaseShader();
			
			SafeRelease( GDebugFont );
			SafeRelease( GDirect3D );
			SafeRelease( GD3DBackBuffer );
			SafeRelease( GRTSurfaceCopy );

			D3D9Logger->Info(L"Release Device...");

			dword ref =  GDirect3DDevice->Release();
			
			if ( ref > 0 )
				D3D9Logger->Debug( L"!!!!!!!!!!!!!!!!!%d D3DObject not release!!!!!!!!!!!!!!!!!!!!", ref );

			GIsRHIInitialized = false;
			SafeDelete(D3D9Logger);
		}
	}

	void OnDeviceLost( )
	{
		if ( GDebugFont != null )
			GDebugFont->OnLostDevice();

		SafeRelease( GRTSurfaceCopy );

		TLinkedList<RenderResource*> *&ResourceList = RenderResource::GetResourceList();

		// Release dynamic resources and render targets.
		for(TLinkedList<RenderResource*>::TIterator ResourceIt(ResourceList);ResourceIt;ResourceIt.Next())
		{
			ResourceIt->ReleaseDynamicRHI();
		}

	}

	void OnDeviceReset( )
	{
		if ( GDebugFont != null )
			GDebugFont->OnResetDevice();

		TLinkedList<RenderResource*> *&ResourceList = RenderResource::GetResourceList();

		for(TLinkedList<RenderResource*>::TIterator ResourceIt(ResourceList);ResourceIt;ResourceIt.Next())
		{
			ResourceIt->InitDynamicRHI();
		}

	}



	void ResetDevice( HWND window, const Point& newdevicesize, bool isfullscreen )
	{
		D3DDISPLAYMODE d3ddm;
		GDirect3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

		D3DPRESENT_PARAMETERS PresentParameters;
		memset(&PresentParameters,0,sizeof(PresentParameters));
		PresentParameters.BackBufferCount			= 1;
		PresentParameters.BackBufferFormat			= d3ddm.Format;
		PresentParameters.BackBufferWidth			= newdevicesize.x;
		PresentParameters.BackBufferHeight			= newdevicesize.y;
		PresentParameters.SwapEffect				= isfullscreen ? D3DSWAPEFFECT_DISCARD : D3DSWAPEFFECT_COPY;
		PresentParameters.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		PresentParameters.EnableAutoDepthStencil	= TRUE;
		PresentParameters.AutoDepthStencilFormat	= D3DFMT_D16;
		PresentParameters.Windowed					= !isfullscreen;
		PresentParameters.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
		PresentParameters.hDeviceWindow				= window;

		HRESULT Result;
		if ( GDirect3DDevice )
		{
			// 后备缓冲已经失效，释放
			SafeRelease( GD3DBackBuffer );

			OnDeviceLost( );

			// Simple reset the device with the new present parameters.
			do 
			{ 
				//GLogger->Debug(L"Device Reset.." );

				if( FAILED(Result=GDirect3DDevice->Reset(&PresentParameters) ) )
				{
					// Sleep for a second before trying again if we couldn't reset the device as the most likely
					// cause is the device not being ready/lost which can e.g. occur if a screen saver with "lock"
					// kicks in.
					::Sleep(1000);
				}
			} 
			while( FAILED(Result) );


			 OnDeviceReset( );
		}
		else
		{
			while ( 1 )
			{
				Result = GDirect3D->CreateDevice(
					D3DADAPTER_DEFAULT,
					D3DDEVTYPE_HAL,
					window,
					D3DCREATE_HARDWARE_VERTEXPROCESSING,
					&PresentParameters,
					&GDirect3DDevice
					);

				if( Result != D3DERR_DEVICELOST )
					break;

				::Sleep( 15 );
			}

			GDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0XFF000000, 1.0f, 0);

			VD3DHR( Result );

			GIsRHIInitialized = true;

			
		}

		// 获取/更新后备缓冲
		VD3DHR( GDirect3DDevice->GetRenderTarget( 0 , &GD3DBackBuffer ) );

		OnInitDeviceState( );


		GD3DDeviceLost = false;
	}


	void UpdateDevice_RenderThread()
	{
		Assert( IsInRenderingThread() );

		if ( GD3DViewportArray.size() > 0 )
		{
			CheckD3D();

			// Find the maximum viewport dimensions and any fullscreen viewports.
			D3D9VideoDevice*	NewFullscreenViewport = NULL;
			Point			MaxViewportSize = Point::cZero;


			for(dword ViewportIndex = 0;ViewportIndex < GD3DViewportArray.size();ViewportIndex++)
			{
				D3D9VideoDevice* D3DViewport = GD3DViewportArray.at(ViewportIndex);

				MaxViewportSize.x = Math::Max((dword)MaxViewportSize.x,D3DViewport->GetWidth());
				MaxViewportSize.y = Math::Max((dword)MaxViewportSize.y,D3DViewport->GetHeight());

				if(D3DViewport->IsFullscreen())
				{
					Assert(!NewFullscreenViewport);
					NewFullscreenViewport = D3DViewport;

					// Check that all fullscreen viewports use supported resolutions.
					UINT Width = D3DViewport->GetWidth();
					UINT Height = D3DViewport->GetHeight();
					/*GetSupportedResolution( Width, Height );
					Assert( Width == D3DViewport->GetSizeX() && Height == D3DViewport->GetSizeY() );*/
				}
			}

			Point NewDeviceSize = MaxViewportSize;
			// Setup the needed device parameters.
			
			D3D9VideoDevice* VP = GD3DViewportArray.at(0);
			HWND NewDeviceWindow = NewFullscreenViewport ? (HWND)NewFullscreenViewport->GetWindowHandle() : (HWND)VP->GetWindowHandle();
			bool NewDeviceFullscreen = NewFullscreenViewport ? true : false;

			// Check to see if GDirect3DDevice needs to be recreated.

			if(GDirect3DDevice)
			{
				if(GD3DDeviceLost)
				{
					// NOTE: We can't use GetFocus() or GetActiveWindow() since NewDeviceWindow
					// may be owned by the game thread and not by us.
					HWND FocusWindow = ::GetForegroundWindow();
					BOOL bIsMinimized = FocusWindow ? ::IsIconic( FocusWindow ) : TRUE;
					if ( FocusWindow != NewDeviceWindow || bIsMinimized )
					{
						// Abort and try again next time Present() is called again from RHIEndDrawingViewport().
						return;
					}
					FocusWindow = ::GetFocus();
					//bRecreateDevice = true;
				}

	/*			if(NewDeviceFullscreen != GD3DIsFullscreenDevice)
				{
					bRecreateDevice = true;
				}

				if(NewDeviceFullscreen)
				{
					if(GD3DDeviceSize.x != NewDeviceSize.x || GD3DDeviceSize.y != NewDeviceSize.y)
					{
						bRecreateDevice = true;
					}
				}
				else
				{
					if(GD3DDeviceSize.x < NewDeviceSize.x || GD3DDeviceSize.y < NewDeviceSize.y )
					{
						bRecreateDevice = true;
					}
				}

				if(GD3DDeviceWindow != NewDeviceWindow)
				{
					bRecreateDevice	= true;
				}*/
			}

			ResetDevice( NewDeviceWindow, NewDeviceSize, NewDeviceFullscreen );

			GD3DFullscreenViewport = NewFullscreenViewport;
			GD3DDeviceSize = NewDeviceSize;
			GD3DDeviceWindow = NewDeviceWindow;
			GD3DIsFullscreenDevice = NewDeviceFullscreen;
		}
		else
		{
			CleanupD3DDevice( );
			GDirect3DDevice = NULL;
		}
	}
}