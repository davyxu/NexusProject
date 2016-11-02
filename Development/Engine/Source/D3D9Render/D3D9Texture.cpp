#include "D3D9PCH.h"


namespace NekoEngine
{
	class D3D9Texture : public RHITexture
	{
	public:
		D3D9Texture( dword Width, dword Height )
			: mDirect3DTexture9( null )
			, mWidth( Width )
			, mHeight( Height )
		{
			
		}

		virtual dword GetWidth( )
		{
			return mWidth;
		}

		virtual dword GetHeight( )
		{
			return mHeight;
		}

		virtual ~D3D9Texture( )
		{
			SafeRelease( mDirect3DTexture9 );
		}

		virtual void CommitAsRenderTarget( dword Index )
		{
			Assert( "Only TargetableTexture can commit as render target" );
		}

		virtual void Commit( dword SamplerIndex )
		{
			Assert( IsInRenderingThread() );

			VD3DHR(GDirect3DDevice->SetTexture( SamplerIndex, mDirect3DTexture9 ));
		}

		virtual void* Lock( dword MipIndex,bool NeedWriteData,dword& DestStride, const Rect* RectToLock )
		{
			Assert( IsInRenderingThread() );

			D3DLOCKED_RECT	LockedRect;
			DWORD			LockFlags = D3DLOCK_NOSYSLOCK;

			if( !NeedWriteData )
			{
				LockFlags |= D3DLOCK_READONLY;
			}

			VD3DHR(mDirect3DTexture9->LockRect(MipIndex,&LockedRect,(RECT*)RectToLock,LockFlags));

			DestStride = LockedRect.Pitch;

			return LockedRect.pBits;
		}

		virtual void Unlock( dword MipIndex )
		{
			Assert( IsInRenderingThread() );

			VD3DHR(mDirect3DTexture9->UnlockRect(MipIndex));
		}
		

		virtual void Save( const wchar* FileName )
		{
			D3DXSaveTextureToFile( FileName, D3DXIFF_PNG, mDirect3DTexture9, NULL );
		}

	public:
		IDirect3DTexture9*	mDirect3DTexture9;
		dword mWidth, mHeight;
	};




	class D3D9TargetableTexture : public D3D9Texture
	{
	public:
		D3D9TargetableTexture( dword Width, dword Height )
			: D3D9Texture( Width, Height ) 
			, mRenderTargetSurface( null )
			, mDepthStencilSurface( null )
		{

		}

		virtual void CommitAsRenderTarget( dword Index )
		{
			Assert( IsInRenderingThread() );
			GDirect3DDevice->SetRenderTarget( Index, mRenderTargetSurface );
		}

		virtual void* Lock( dword MipIndex,bool NeedWriteData,dword& DestStride, Rect* RectToLock )
		{
			// 如果确实需要，请参阅http://www.opengpu.org/bbs/viewthread.php?tid=445
			Assert("Use RHI->LockRenderTargetSurface() Instead ");

			return null;
		}

		virtual void Unlock( dword MipIndex )
		{
			Assert("Use RHI->UnlockRenderTargetSurface() Instead ");
		}

		virtual ~D3D9TargetableTexture( )
		{
			SafeRelease( mRenderTargetSurface );
			SafeRelease( mDepthStencilSurface );
		}

	public:
		IDirect3DSurface9* mRenderTargetSurface;
		IDirect3DSurface9* mDepthStencilSurface;
	};


	class D3D9TargetableSurface : public RHISurface
	{
	public:
		D3D9TargetableSurface( dword Width, dword Height )
			: mWidth( Width)
			, mHeight( Height ) 
			, mRenderTargetSurface( null )
			, mDepthStencilSurface( null )
		{

		}

		virtual void Save( const wchar* FileName )
		{
			D3DXSaveSurfaceToFile( FileName, D3DXIFF_PNG, mRenderTargetSurface, null, null);
		}

		virtual dword GetWidth( )
		{
			return mWidth;
		}

		virtual dword GetHeight( )
		{
			return mHeight;
		}

		virtual void CommitAsRenderTarget( dword Index )
		{
			Assert( IsInRenderingThread() );
			VD3DHR( GDirect3DDevice->SetRenderTarget( Index, mRenderTargetSurface ) );
		}

		virtual void* Lock( dword& DestStride, Rect* RectToLock )
		{
			Assert( IsInRenderingThread() );

			D3DLOCKED_RECT	LockedRect;

			VD3DHR(mRenderTargetSurface->LockRect( &LockedRect,(RECT*)RectToLock,D3DLOCK_READONLY));

			DestStride = LockedRect.Pitch;

			return LockedRect.pBits;
		}

		virtual void Unlock( )
		{
			Assert( IsInRenderingThread() );
			mRenderTargetSurface->UnlockRect( );
		}

		virtual ~D3D9TargetableSurface( )
		{
			SafeRelease( mRenderTargetSurface );
			SafeRelease( mDepthStencilSurface );
		}

	public:
		IDirect3DSurface9* mRenderTargetSurface;
		IDirect3DSurface9* mDepthStencilSurface;
		dword mWidth, mHeight;
	};



	IDirect3DSurface9* GRTSurfaceCopy = null;


	void UpdateRTSurfaceCopy_RenderThread(  )
	{
		LPDIRECT3DSURFACE9 CurrentRT = null;
		VD3DHR(GDirect3DDevice->GetRenderTarget( 0, &CurrentRT ));


		if ( GRTSurfaceCopy == null )
		{

			D3DSURFACE_DESC RTSurfaceDesc;
			CurrentRT->GetDesc( &RTSurfaceDesc );
			VD3DHR(GDirect3DDevice->CreateOffscreenPlainSurface( 
				RTSurfaceDesc.Width, 
				RTSurfaceDesc.Height, 
				RTSurfaceDesc.Format, 
				D3DPOOL_SYSTEMMEM,
				&GRTSurfaceCopy,
				null ) );
		}		

		VD3DHR( GDirect3DDevice->GetRenderTargetData( CurrentRT, GRTSurfaceCopy ) );

		CurrentRT->Release();
	}



	void* D3D9RHI::LockRenderTargetSurface( dword& DestStride, Rect* RectToLock )
	{
		UpdateRTSurfaceCopy_RenderThread( );

		D3DLOCKED_RECT	LockedRect;
		VD3DHR( GRTSurfaceCopy->LockRect( &LockedRect, (RECT*)RectToLock, D3DLOCK_READONLY ) );

		DestStride = LockedRect.Pitch;

		return LockedRect.pBits;
	}

	void D3D9RHI::UnlockRenderTargetSurface( )
	{
		GRTSurfaceCopy->UnlockRect( );
	}


	RHITextureRef D3D9RHI::CreateTexture2D( dword SizeX, dword SizeY, dword MipCount, ePixelFormat Format)
	{
		Assert( IsInRenderingThread() );

		if ( SizeX == 0 || SizeY == 0 || MipCount == 0 )
			return RHITextureRef();

		D3D9Texture* NewTexture = new D3D9Texture( SizeX, SizeY );


		D3DFORMAT D3DFormat = (D3DFORMAT)Format;
		
		VD3DHR(GDirect3DDevice->CreateTexture(	SizeX, 
												SizeY, 
												MipCount, 
												0, 
												D3DFormat, 
												D3DPOOL_MANAGED, 
												&NewTexture->mDirect3DTexture9, 
												NULL ) );

		return NewTexture;
	}


	RHITextureRef D3D9RHI::CreateTargetableTexture2D( dword SizeX, dword SizeY, ePixelFormat Format)
	{
		Assert( IsInRenderingThread() );

		D3D9TargetableTexture* NewTexture = new D3D9TargetableTexture( SizeX, SizeY );


		D3DFORMAT D3DFormat = (D3DFORMAT)Format;

		VD3DHR(GDirect3DDevice->CreateTexture(	
			SizeX, 
			SizeY, 
			1, 
			D3DUSAGE_RENDERTARGET, 
			D3DFormat, 
			D3DPOOL_DEFAULT, 
			&NewTexture->mDirect3DTexture9, 
			NULL ) );


		VD3DHR( NewTexture->mDirect3DTexture9->GetSurfaceLevel(0, &NewTexture->mRenderTargetSurface ) );

		return NewTexture;
	}

	RHISurfaceRef D3D9RHI::CreateTargetableSurface( dword SizeX, dword SizeY, ePixelFormat Format, bool Readable )
	{
		Assert( IsInRenderingThread() );

		D3D9TargetableSurface* NewSurface = new D3D9TargetableSurface( SizeX, SizeY );


		D3DFORMAT D3DFormat = (D3DFORMAT)Format;

		VD3DHR(GDirect3DDevice->CreateRenderTarget(	
			SizeX, 
			SizeY, 
			D3DFormat, 
			D3DMULTISAMPLE_NONE,
			0,
			Readable,			
			&NewSurface->mRenderTargetSurface, 
			NULL ) );


		NewSurface->mWidth = SizeX;
		NewSurface->mHeight = SizeY;
		

		return NewSurface;
	}

}