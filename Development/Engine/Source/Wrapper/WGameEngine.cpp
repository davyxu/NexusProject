#include "WrapperPCH.h"
#include "PrivateInclude.h"

using namespace NekoEngine;

namespace Wrapper
{


		class Demo_Model : public SceneListener
		{
			DECLARE_RTTI_CLASS(Demo_Model, SceneListener, ClassFlag_Intrinsic );

		public:


			virtual void OnEnter()
			{
				mTeapot = GWorld->SpawnActor( RTTI_CLASS_INFO(ActorFactoryStaticMesh) );
				StaticMeshComponent* Component = mTeapot->GetComponent<StaticMeshComponent>();
				Component->SetMesh( GResourceMaster->Load<StaticMeshResource>(L"EngineModel.Teapot.SM_teapot" ));
				Component->SetMaterial( GResourceMaster->Load<MaterialInstance>(L"EngineMaterial.MI_BumpMap") );
			}

			virtual void OnLeave()
			{

			}

			virtual void OnUpdate( float elapse )
			{


			}


		private:
			ActorPtr mTeapot;
		};


	IMPLEMENT_RTTI_CLASS(Demo_Model)


	class DemoFramework : public EngineEventListener
	{
	public:
		Camera* mCamera;

		DemoFramework( )
		{
			
			GSceneMaster		= new SceneMaster;
		}

		virtual ~DemoFramework( )
		{
			SafeDelete( GSceneMaster );
		}



		virtual void OnEngineLoad( )
		{ 
			MessageInputDriver* MessageInput = dynamic_cast<MessageInputDriver*>( GInputDriver );

			mCamera = GWorld->CreateCamera( L"SampleCamera" );

			GEngine->GetPrimaryTarget()->AddRenderView( mCamera );
			GEngine->GetPrimaryTarget()->SetEnableDraw( false );

			// 场景创建
			GSceneMaster->Create( );

			GSceneMaster->Goto( RTTI_CLASS_NAME(Demo_Model) );
		}

		virtual void OnEngineUnload( )
		{ 
			// 场景卸载
			GSceneMaster->Destroy( );
			GSceneMaster->Release( );
		}

		virtual void OnEngineFrameBegin( float Elapsed )
		{ 
			GSceneMaster->Update(Elapsed);
		}

	};

	DemoFramework GDemo;

	GameEngine::GameEngine()
		: mWindow( null )
	{
		CoreInitializer::InitGlobalObject();
		EngineInitializer::InitGlobalObject();

		gcnew ReflectorManager( );

		mEngine = new NekoEngine::GameEngine;

		RTTI_REGISTER( Demo_Model  );

		RTTIGroup* SceneListenerGroup = RTTIObject::GetGroup( L"SceneListener" );
		RTTI_REGISTER_GROUP( Demo_Model, SceneListenerGroup )
	}

	GameEngine::~GameEngine()
	{
		EngineInitializer::ExitGlobalObject();
		CoreInitializer::ExitGlobalObject();
	}

	void GameEngine::InitializeEngine( int Handle )
	{
		mWindow = new ExternWindow( (void*)Handle, false );
		mEngine->Initialize( mWindow, &GDemo );
	}

	void GameEngine::ResizeWindow( int X, int Y, int NewHandle )
	{
		if ( mWindow == null )
			return;

		mWindow->GetRHIVideoDevice()->Resize( X, Y, false, (void*)NewHandle );
		GEngine->UpdateRenderTarget();
		mWindow->GetRHIVideoDevice()->UpdateDevice();
	}

	void GameEngine::FinalizeEngine()
	{
		mEngine->Finalize();
		delete mEngine;
		mEngine = null;
		mWindow = null;
	}

	void GameEngine::Update( )
	{
		if ( mEngine != null )
			mEngine->Tick();
	}


	void GameEngine::NotifyMouseMove( int x ,int y )
	{
		if ( mEngine != null )
		{
			MessageInputDriver* MessageInput = dynamic_cast<MessageInputDriver*>( GInputDriver );
			MessageInput->NotifyMousePos( Point( x, y ));
		}
	}

	void GameEngine::NotifyMouseScroll( int z )
	{
		if ( mEngine != null )
		{
			MessageInputDriver* MessageInput = dynamic_cast<MessageInputDriver*>( GInputDriver );
			MessageInput->NotifyMouseScroll( z );
		}
	}

	void GameEngine::NotifyKeyUp( int KeyCode )
	{
		if ( mEngine != null )
		{
			MessageInputDriver* MessageInput = dynamic_cast<MessageInputDriver*>( GInputDriver );
			MessageInput->NotifyKeyPressed( (eKeyButton)KeyCode );
		}
	}

	void GameEngine::NotifyMouseUp( int KeyCode )
	{
		if ( mEngine != null )
		{
			MessageInputDriver* MessageInput = dynamic_cast<MessageInputDriver*>( GInputDriver );
			MessageInput->NotifyMousePressed( (eMouseButton)KeyCode );
		}
	}


	Actor^ GameEngine::CreateActor( )
	{
		Actor^ A = gcnew Actor();

		A->mActor = GWorld->SpawnActor( RTTI_CLASS_INFO(ActorFactoryStaticMesh) );
		StaticMeshComponent* Component = A->mActor->GetComponent<StaticMeshComponent>();
		Component->SetMesh( GResourceMaster->Load<StaticMeshResource>(L"EngineModel.Teapot.SM_teapot" ));
		Component->SetMaterial( GResourceMaster->Load<MaterialInstance>(L"EngineMaterial.MI_BumpMap") );

		return A;
	}

}