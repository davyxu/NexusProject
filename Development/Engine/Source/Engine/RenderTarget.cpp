#include "EnginePCH.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{



	//////////////////////////////////////////////////////////////////////////
	// RenderTarget
	//////////////////////////////////////////////////////////////////////////

	RenderTarget::RenderTarget()		
		: mListener( null )
		, mClearColor( 0x003f3f3f )
		, mEnableDraw( true )
	{
		
	}


	RenderTarget::~RenderTarget()
	{
		ClearView();
	}


	SceneView* RenderTarget::AddSceneView( GameWorld* World, Camera* Cam, dword ZOrder, float Left, float Top, float Width, float Height )
	{
		SceneView* VP = new SceneView( World, this, Cam, ZOrder, Left, Top, Width, Height );

		ViewMap::iterator it = mViewMap.find( ZOrder );
		if ( it != mViewMap.end() )
		{
			Assert( "Already has same ZOrder SceneView" );
			return null;
		}

		mViewMap.insert( std::pair<dword, SceneView*>( ZOrder, VP ) );

		return VP;
	}


	void RenderTarget::DrawSceneView()
	{
		for (ViewMap::iterator it = mViewMap.begin();
			it != mViewMap.end();
			++it )
		{
			SceneView* RV = it->second;


			RV->Draw( mListener );


		}
	}



	void RenderTarget::ClearView()
	{
		for (ViewMap::iterator it = mViewMap.begin();
			it != mViewMap.end();
			++it )
		{
			SceneView* VP = it->second;
			delete VP;
		}

		mViewMap.clear();
	}

	void RenderTarget::UpdateViewDimension()
	{
		for (ViewMap::iterator it = mViewMap.begin();
			it != mViewMap.end();
			++it )
		{
			SceneView* VP = it->second;
			VP->UpdateDimension();
		}
	}

	void RenderTarget::AddListener( SceneViewListener* Listener )
	{
		mListener = Listener;
	}

	SceneView* RenderTarget::GetView( dword Index )
	{
		Assert( Index < mViewMap.size() );

		ViewMap::iterator it = mViewMap.begin();
		while (Index--)
			++it;

		return it->second;
	}

	void RenderTarget::NotifyDestoryCamera(Camera* Cam )
	{
		for (ViewMap::iterator it = mViewMap.begin();
			it != mViewMap.end();
			++it )
		{
			SceneView* RV = it->second;

			if ( RV->GetCamera() == Cam )
				RV->SetCamera( null );
		}
	}


	//////////////////////////////////////////////////////////////////////////
	// RenderTargetableResource
	//////////////////////////////////////////////////////////////////////////

	RenderTargetableResource::RenderTargetableResource( )
		: mWidth( 0 )
		, mHeight( 0 )
		, mView( null )
		, mScaleX( 1.0f )
		, mScaleY( 1.0f )
	{
		// 设置为黑色不透明
		mClearColor = 0xFF000000;
	}


	RenderTargetableResource::~RenderTargetableResource()
	{

	}

	void RenderTargetableResource::SetScaleSize( SceneView* View , float ScaleX /*= 1.0f*/, float ScaleY /*= 1.0f*/ )
	{
		mView	= View;
		mScaleX = ScaleX;
		mScaleY = ScaleY;
	}

	void RenderTargetableResource::SetSize( dword Width, dword Height  )
	{
		mWidth  = Width;
		mHeight = Height;
		mView   = null;
	}
	dword RenderTargetableResource::GetWidth()
	{
		if ( mView != null )
			return dword( mView->GetWidth() * mScaleX);

		// 这里不能取纹理大小，纹理此时不一定初始化好
		return mWidth;
	}

	dword RenderTargetableResource::GetHeight()
	{
		if ( mView != null )
			return dword( mView->GetHeight() * mScaleY );

		// 这里不能取纹理大小，纹理此时不一定初始化好
		return mHeight;
	}

	void RenderTargetableResource::InitDynamicRHI()
	{
		// 使用View
		if ( mView != null )
		{
			mWidth  = GetWidth();

			mHeight = GetHeight();
		}
	}

	void RenderTargetableResource::Draw()
	{
		// 可能纹理还没创建好
		//if ( !IsResourceReady() )
		//	return;

		// BeginScene/EndScene只能有一对
		ENQUEUE_UNIQUE_RENDER_COMMAND(BeginDrawCommand, { 

			RHI->BeginDraw();
		} );

		// 注意，RT的透明背景效果是在绘制时，选择透明绘制而成
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(RTTSet,RHISurfaceBase*,mSurfaceBase, GetSurfaceBase(), dword, mClearColor, mClearColor,
		{ 

			mSurfaceBase->CommitAsRenderTarget( 0 );

			RHI->Clear( true, mClearColor , true, 1.0f, false, 0 );
		} );




		// 将内容绘制到纹理上
		DrawSceneView();

		// BeginScene/EndScene只能有一对
		ENQUEUE_UNIQUE_RENDER_COMMAND(EndDrawCommand, { 

			RHI->EndDraw();
		} );


	}


	//////////////////////////////////////////////////////////////////////////
	// RenderTargetableTexture2D
	//////////////////////////////////////////////////////////////////////////
	void RenderTargetableTexture2D::InitDynamicRHI()
	{
		RenderTargetableResource::InitDynamicRHI();

		mTextureRHI = RHI->CreateTargetableTexture2D( mWidth, mHeight, PF_A8R8G8B8 );

		RenderTexture::InitSampler();
	}


	void RenderTargetableTexture2D::ReleaseDynamicRHI()
	{
		RenderTexture::ReleaseSamplerTexture();
	}

	bool RenderTargetableTexture2D::IsResourceReady()
	{
		return mTextureRHI.Valid();
	}

	RHISurfaceBase* RenderTargetableTexture2D::GetSurfaceBase()
	{
		return mTextureRHI;
	}
	//////////////////////////////////////////////////////////////////////////
	// RenderTargetableSurface
	//////////////////////////////////////////////////////////////////////////

	void RenderTargetableSurface::BeginReleaseResource()
	{
		// 开始卸载设备对象
		RenderResource::BeginReleaseResource();

		mReleaseFence.BeginFence();
	}

	bool RenderTargetableSurface::ReadyForDestory( )
	{
		return mReleaseFence.GetNumPendingFences() == 0;
	}

	void RenderTargetableSurface::InitDynamicRHI()
	{
		RenderTargetableResource::InitDynamicRHI();

		mRHISurface = RHI->CreateTargetableSurface( mWidth, mHeight, PF_A8R8G8B8, true );		
	}


	void RenderTargetableSurface::ReleaseDynamicRHI()
	{
		mRHISurface.Release();
	}

	bool RenderTargetableSurface::IsResourceReady()
	{
		return mRHISurface.Valid();
	}

	RHISurfaceBase* RenderTargetableSurface::GetSurfaceBase()
	{
		return mRHISurface;
	}






}