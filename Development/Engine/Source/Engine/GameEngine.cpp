#include "EnginePCH.h"
#include "NekoEngine_Render.h"
#include "NekoEngine_System.h"


namespace NekoEngine
{	
	SceneMaster* GSceneMaster	= null;
	GameEngine* GEngine		= null;
	dword		GGameThreadID	= 0;

	IMPLEMENT_RTTI_CLASS( GameEngine )

	GameEngine::GameEngine( )
		: mTestPoint( Vector3(10.0f,-50.0f,50.0f) )
		, mIsUpdate( true )
		, mUseThreadRendering( false )
		, mUseAsyncTask( false )
		, mUseShaderCache( false )
		, mListener( null )
		, mEnableDraw( true )
	{
		Assert( GEngine == null );
		GEngine = this;

		GObjectManager		= new ObjectManager;

		GGameThreadID = Thread::GetCurrentThreadID();

		mLogger = new SimpleLog;
		mLogger->SetPrefix(L"GameEngine");
		mLogger->Initialize();

		mFPSUnit			= new FrameSpeedUnit;
		GShaderMaster		= new ShaderMaster;
		
		LoadConfig();


		RHIInitialize();
	}

	void GameEngine::StaticInitProperty( )
	{
		PropertyCollection* PC = RTTI_CLASS_INFO(GameEngine)->GetPropertyCollection( );

		PropertyGroup* Group = PC->AddGroup( L"GameEngine" );

		PC->Add( PROPERTY_MEMBER_VAR( GameEngine,bool, mShowGrid )			, Group );
		PC->Add( PROPERTY_MEMBER_VAR( GameEngine,bool, mUseShaderCache )	, Group );

		// Enable字段为可动态调整的, Use表示需要重启
		PC->Add( PROPERTY_DELEGATE( GameEngine,bool, L"EnableParallelRendering", &GameEngine::OnPropertyUseParallelRendering ), Group );
		PC->Add( PROPERTY_DELEGATE( GameEngine,bool, L"EnableAsyncTask", &GameEngine::OnPropertyUseAsyncTask ), Group );

	}

	void GameEngine::OnPropertyUseParallelRendering( bool* Value, bool IsGet )
	{
		if ( IsGet )
		{
			*Value = GRenderThread.IsEnable();
		}
		else
		{
			if (GRenderThread.IsEnable() == *Value )
				return;

			if ( *Value )
			{
				GRenderThread.Start();				
			}
			else
			{
				GRenderThread.Stop();	
			}
		}
	}

	void GameEngine::OnPropertyUseAsyncTask( bool* Value, bool IsGet )
	{
		if ( IsGet )
		{
			*Value = GAsyncTaskThread.IsEnable();
		}
		else
		{
			if (GAsyncTaskThread.IsEnable() == *Value )
				return;

			if ( *Value )
			{
				GAsyncTaskThread.Start();				
			}
			else
			{
				GAsyncTaskThread.Stop();	
			}
		}
	}


	GameEngine::~GameEngine( )
	{
		SafeDelete( GShaderMaster );
		SafeDelete( mFPSUnit );
		SafeDelete( mLogger );
		SafeDelete( GObjectManager );

		GGameThreadID = 0;
		GEngine = null;
	}


	void GameEngine::Initialize( EngineEventListener* Listener )
	{
		mListener = Listener;

		// Device is ready now
		mLogger->Info( L"Initialize Static Render Resource...");
		WhiteTexture::StaticInit();
		CheckerTexture::StaticInit();
		BatchedElements::StaticInit();

		mLogger->Info( L"Initialize Shader Library ...");
		// 编译Shader并匹配寄存器
		GShaderMaster->InitializeShaderLibrary( mUseShaderCache );
	}

	GameWorld* GameEngine::CreateWorld( )
	{
		GameWorld* World = new GameWorld( mGameWorldList.size() );

		mGameWorldList.push_back( World );

		return World;
	}

	GameWorld* GameEngine::GetWorld( dword Index )
	{
		if ( Index >= mGameWorldList.size() )
			return null;

		return mGameWorldList[ Index ];
	}

	void GameEngine::ClearWorld( )
	{
		for ( GameWorldList::iterator it = mGameWorldList.begin();
			it != mGameWorldList.end();
			++it )
		{
			GameWorld* World = *it;

			delete World;
		}

		mGameWorldList.clear();
	}

	void GameEngine::Finalize( )
	{
		// 卸载ActorFactory
		ClearWorld( );

		CheckerTexture::StaticExit();
		WhiteTexture::StaticExit();

		GObjectManager->CollectGarbage( GCP_Finalize );

		// Shader有引用资源的对象
		GShaderMaster->Clear( );

		// 卸载设备和渲染资源
		ClearRenderTarget();


		RHIFinalize();

		// 考虑到异步加载完成后是渲染加载
		GAsyncTaskThread.Stop();

		GRenderThread.Stop();


		ManagedObject::StaticExit();
	}


	bool GameEngine::Tick( )
	{
		TimeSource::Tick();
		mFPSUnit->Tick();

		float Elapse = TimeSource::GetElapsedTime();

		if ( mListener != null )
			mListener->OnEngineFrameBegin( Elapse );

		GShaderMaster->Tick( Elapse );

		GAsyncTaskThread.NotifyFinishedTask();

		if ( mIsUpdate )
			Tickable::StaticTick( Elapse );

		if ( mEnableDraw )
		{
			DrawRenderTarget( );

			GRenderThread.Flush();
		}
		

		GObjectManager->CheckResourceProcessing( );

		GObjectManager->CollectGarbage( GCP_Full );

		// 更新输入设备
		for ( RenderTargetArray::reverse_iterator it = mRenderTargetArray.rbegin();
			it != mRenderTargetArray.rend();
			++it )
		{
			PresentWindow* Window = dynamic_cast<PresentWindow*>(*it);

			Window->GetInputDriver()->Tick( Elapse );
		}
		
		if ( mListener != null )
			mListener->OnEngineFrameEnd( Elapse );

		return true;
	}

	void GameEngine::CheckEngineShortcut( )
	{
		IInputDriver* InputDriver = dynamic_cast<PresentWindow*>( GEngine->GetRenderTarget( 0 ) )->GetInputDriver();

		// 设置调试点位置
		if ( InputDriver->IsKeyUp(KB_F1) )
		{
			mIsUpdate = !mIsUpdate;
		}


		if ( InputDriver->IsKeyUp(KB_PageDown) )
		{
			GSceneMaster->GotoNext();
		}

		if ( InputDriver->IsKeyUp(KB_PageUp) )
		{
			GSceneMaster->GotoPrev();
		}

		// 保存配置
		if ( InputDriver->IsKeyUp(KB_F4) )
		{
			SaveConfig();
		}


		// 重新编译shader
		if ( InputDriver->IsKeyUp(KB_F7) )
		{
			GShaderMaster->Rebuild();
		}

		// 切换多线程渲染
		if ( InputDriver->IsKeyUp( KB_F8 ) )
		{
			bool EnableParallelRendering = false;
			GetObjectPropertyT<bool>(L"EnableParallelRendering", EnableParallelRendering );
			SetObjectPropertyT<bool>( L"EnableParallelRendering", !EnableParallelRendering);
		}

		// 设置调试点位置
		//if ( InputDriver->IsKeyPressed(KB_F9) )
		//{
		//	mTestPoint = mCamera.GetPosition();
		//}
	}

	void GameEngine::DrawRenderTarget( )
	{
		for ( RenderTargetArray::iterator it = mRenderTargetArray.begin();
			it != mRenderTargetArray.end();
			++it )
		{
			RenderTarget* RT = *it;

			if ( RT->GetEnableDraw() )
				RT->Draw();
		}
	}

	



	const wchar* ENGINE_CONFIG_FILE = L"..\\Config\\Engine.ini";

	void GameEngine::SaveConfig( )
	{
		PropertySerializer Ser( ENGINE_CONFIG_FILE, false );
		
		SerializeConfig( Ser );
	}

	void GameEngine::SerializeConfig( PropertySerializer& Ser )
	{
		Super::SerializeConfig( Ser );

		GObjectManager->SerializeConfig( Ser );
	}

	void GameEngine::LoadConfig( )
	{
		PropertySerializer Ser( ENGINE_CONFIG_FILE, true );

		SerializeConfig( Ser );
	}



	bool RTSort(RenderTarget* A, RenderTarget* B) 
	{
		return A->GetPriority() > B->GetPriority();
	}

	void GameEngine::AttachRenderTarget( RenderTarget* RT )
	{
		// 1 Texture/ Surface   <-- begin
		// 0 Screen				<-- end

		mRenderTargetArray.push_back( RT );

		std::sort( mRenderTargetArray.begin(), mRenderTargetArray.end(), RTSort );
	}

	void GameEngine::DetachRenderTarget( RenderTarget* RT)
	{
		for ( RenderTargetArray::iterator it = mRenderTargetArray.begin();
			it != mRenderTargetArray.end();
			++it )
		{
			RenderTarget* LibRT = *it;

			if ( LibRT == RT )
			{
				mRenderTargetArray.erase( it );

				delete LibRT;

				break;
			}
		}
	}

	void GameEngine::ClearRenderTarget( )
	{
		// RHI层的析构同步
		GRenderThread.Flush();

		for ( RenderTargetArray::iterator it = mRenderTargetArray.begin();
			it != mRenderTargetArray.end();
			++it )
		{
			RenderTarget* RT = *it;

			delete RT;		
		}

		mRenderTargetArray.clear();
	}

	void GameEngine::UpdateRenderTarget(  )
	{
		// 从屏幕RT开始更新到纹理
		for ( RenderTargetArray::reverse_iterator it = mRenderTargetArray.rbegin();
			it != mRenderTargetArray.rend();
			++it )
		{
			RenderTarget* RT = *it;
			RT->UpdateViewDimension();
		}
	}


	void GameEngine::NotifyDestoryCamera( Camera* Cam )
	{
		// 从屏幕RT开始更新到纹理
		for ( RenderTargetArray::reverse_iterator it = mRenderTargetArray.rbegin();
			it != mRenderTargetArray.rend();
			++it )
		{
			RenderTarget* RT = *it;
			RT->NotifyDestoryCamera( Cam );
		}
	}

	GameWindow* GameEngine::CreateGameWindow( const wchar* Title, dword Width, dword Height, bool IsFullScreen, dword IconID )
	{
		GameWindow* Window = new GameWindow( Title, Width, Height, IsFullScreen, IconID  );

		AttachRenderTarget( Window );

		return Window;
	}

	ExternWindow* GameEngine::CreateGameWindow( void* WindowHandle, bool IsFullScreen )
	{
		ExternWindow* Window = new ExternWindow( WindowHandle, IsFullScreen );

		AttachRenderTarget( Window );

		return Window;
	}

	NekoEngine::dword GameEngine::GetRenderTargetCount()
	{
		return mRenderTargetArray.size();
	}

	RenderTarget* GameEngine::GetRenderTarget( dword Index )
	{
		if ( Index >= mRenderTargetArray.size() )
			return null;

		return mRenderTargetArray[ Index ];
	}

	PresentWindow* GameEngine::GetWindow( dword Index )
	{
		return dynamic_cast<PresentWindow*>( GetRenderTarget( Index ) );
	}

}
