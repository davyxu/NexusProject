#include "EnginePCH.h"
#include "NekoEngine_Render.h"


namespace NekoEngine
{

	//////////////////////////////////////////////////////////////////////////
	// RenderViewport
	//////////////////////////////////////////////////////////////////////////

	SceneView::SceneView( GameWorld* World, RenderTarget* Owner, Camera* Cam, dword ZOrder, float Left, float Top, float Width, float Height )
		: mCamera( Cam )		
		, mOwner( Owner )
		, mZOrder( ZOrder )
		, mRelativeLeft( Left )
		, mRelativeTop( Top )
		, mRelativeWidth( Width )
		, mRelativeHeight( Height )
		, mHitProxyRT( null )
		, mNumPendingFences( 0 )
		, mWorld( World )
		, mCanvas( null )
	{
		UpdateDimension();
	}

	void SceneView::SetDimension( float Left /*= 0.0f*/, float Top /*= 0.0f*/, float Width /*= 1.0f*/, float Height /*= 1.0f */ )
	{
		mRelativeLeft = Left;
		mRelativeTop = Top;
		mRelativeWidth = Width;
		mRelativeHeight = Height;
		UpdateDimension();
	}

	bool SceneView::IsInFrustum( const AABB& BoundingBox )
	{
		return mFrustum.Inside( Box(BoundingBox) );
	}

	void SceneView::Draw( SceneViewListener* Listener )
	{
		if ( mCamera == null )
			return;

		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(BeginDrawCommand,Viewport,VP, mViewport, { 

			RHI->SetDrawArea( VP.X, VP.Y, VP.Width, VP.Height );

		} );


		mCamera->Update( mViewport );

		mFrustum = Frustum( mCamera->GetViewMatrix(), mCamera->GetProjectMatrix() );

		Canvas Painter( mViewport );
		mCanvas = &Painter;


		if ( Listener != null )
			Listener->PreSceneViewDraw( this );

		mWorld->Draw( this );

		if ( Listener != null )
			Listener->PostSceneViewDraw( this );

		Painter.Flush();

		mCanvas = null;

	}


	void SceneView::UpdateDimension()
	{
		mViewport.X = int( mRelativeLeft * mOwner->GetWidth() );
		mViewport.Y = int( mRelativeTop * mOwner->GetWidth() );
		mViewport.Width = int( mRelativeWidth * mOwner->GetWidth() );
		mViewport.Height = int( mRelativeHeight * mOwner->GetHeight() );
	}

	void SceneView::StartQueryPixel( int X, int Y )
	{
		if ( mHitProxyRT == null )
		{
			mHitProxyRT = new RenderTargetableSurface;
			mHitProxyRT->SetScaleSize( this );
			mHitProxyRT->BeginInitResource();

			// 不自动绘制，手动绘制
			mHitProxyRT->SetEnableDraw( false );

			//mHitProxyRT->AddRenderView( &GEngine->mCamera );
		}


		// BeginScene/EndScene只能有一对
		ENQUEUE_UNIQUE_RENDER_COMMAND(BeginDrawCommand, { 

			RHI->BeginDraw();
		} );


		mHitProxyRT->Draw();
		

		ENQUEUE_UNIQUE_RENDER_COMMAND(DrawDebugStatus, { 

			RHI->RestoreRenderTarget( 0 );

			RHI->EndDraw();

			//@TODO 取view相关的target
			PresentWindow* RW = dynamic_cast<PresentWindow*>( GEngine->GetRenderTarget( 0 ) );

			RW->GetRHIVideoDevice()->Present();
		} );

		mColorQueryResult = 0;
		mNumPendingFences = 0;

		QueryPixelParameter Parameter ={
			X, Y,
			this
		};

		LockObject::IncrementLock( (int*)&mNumPendingFences );

		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER( QueryPixel, QueryPixelParameter, mParameter,Parameter, {
			QueryPixel_RenderThread( mParameter );	
		})


	}
	void SceneView::QueryPixel_RenderThread( QueryPixelParameter& Parameter )
	{

		Rect RCToLock( Parameter.X, Parameter.Y, Parameter.X + 1, Parameter.Y + 1);

		dword DestStride;

		RHISurface* SurfacePtr = Parameter.ClassPtr->mHitProxyRT->mRHISurface;

		//@ TODO 注意其他格式纹理
		dword* DestMipData = (dword*)SurfacePtr->Lock( DestStride, &RCToLock );		

		Parameter.ClassPtr->mColorQueryResult = *DestMipData;

		SurfacePtr->Unlock();

		LockObject::DecrementLock( (int*)&Parameter.ClassPtr->mNumPendingFences );
	}

	bool SceneView::GetPixel( dword& ColorResult, bool Wait /*= true */ )
	{
		// 等待Query线程完成查询
		if( Wait )
		{
			while ( mNumPendingFences > 0 )
			{
				Application::Sleep( 0 );
			}
		}

		if ( mNumPendingFences > 0 )
			return false;

		ColorResult = mColorQueryResult;

		return true;
	}

	NekoEngine::Ray SceneView::CreateRay( int X, int Y )
	{
		Assert( mCamera != null );

		X -= mViewport.X;
		Y -= mViewport.Y;

		return Ray( 2.0f * X / mViewport.Width - 1.0f, 2.0f * Y / mViewport.Height - 1.0f, mCamera->GetViewMatrix(), mCamera->GetProjectMatrix() );
	}

	Ray SceneView::CreateRay( const Vector2& ScreenPos )
	{
		Assert( mCamera != null );

		return Ray( 2.0f * ScreenPos.x - 1.0f, 2.0f * ScreenPos.y - 1.0f, mCamera->GetViewMatrix(), mCamera->GetProjectMatrix() );
	}

}