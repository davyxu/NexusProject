#include "D3D9PCH.h"


namespace NekoEngine
{
	/** A list of all viewport RHIs that have been created. */
	
	D3DViewportArray GD3DViewportArray;

	RHIVideoDevice* D3D9RHI::CreateVideoDevice( void* window, dword Width, dword Height,bool isfullscn )
	{
		Assert( window != null );
		Assert( Width > 0 );
		Assert( Height > 0 );

		return new D3D9VideoDevice( window, Width, Height , isfullscn );
	}

	void D3D9VideoDevice::UpdateDevice( )
	{
		ENQUEUE_UNIQUE_RENDER_COMMAND(EndDrawCommand, { 

			UpdateDevice_RenderThread();

		} );

		GRenderThread.Flush();
	}

	D3D9VideoDevice::D3D9VideoDevice(void* InWindowHandle, dword Width, dword Height,bool bInIsFullscreen)
		: mWindowHandle(InWindowHandle)
		, mWidth( Width )
		, mHeight( Height )
		, mIsFullscreen(bInIsFullscreen)
	{
		GD3DViewportArray.push_back(this);


		UpdateDevice( );
	}




	D3D9VideoDevice::~D3D9VideoDevice()
	{
		GD3DViewportArray.erase( find(GD3DViewportArray.begin(), GD3DViewportArray.end(), this ) );

		UpdateDevice( );
	}

	void D3D9VideoDevice::Resize( dword Width, dword Height, bool bInIsFullscreen, void* NewWindowHandle )
	{
		if ( mWidth == Width && mHeight == Height )
			return;

		mWidth = Width;
		mHeight = Height;
		mIsFullscreen = bInIsFullscreen;

		if ( NewWindowHandle )
			mWindowHandle = NewWindowHandle;
	}




	void D3D9VideoDevice::Present( )
	{
		// Clear references the device might have to resources.
		//GDirect3DDevice->SetRenderTarget(0,GD3DBackBuffer);
		//GDirect3DDevice->SetDepthStencilSurface(NULL);

		// Present the back buffer to the viewport window.
		HRESULT Result;
		if(mIsFullscreen)
		{
			Result = GDirect3DDevice->Present(NULL,NULL,NULL,NULL);
		}
		else
		{
			RECT SourceRect;
			SourceRect.left		= SourceRect.top = 0;
			SourceRect.right	= mWidth;
			SourceRect.bottom	= mHeight;
			Result				= GDirect3DDevice->Present(&SourceRect,NULL,(HWND)mWindowHandle,NULL);
		}

		// Detect a lost device.
		if(Result == D3DERR_DEVICELOST)
		{
			// This variable is checked periodically by the main thread.
			GD3DDeviceLost = true;
		}
		else
		{
			//VD3DHR(Result);
		}
	}

	void D3D9VideoDevice::Release()
	{
		delete this;
	}

	extern UINT BitsPerPixel( D3DFORMAT fmt );

	dword D3D9RHI::GetBitsPerPixel( ePixelFormat Format )
	{
		return BitsPerPixel( D3DFORMAT(Format) );
	}
}