#include "EngineDemoPCH.h"
#include "PCH_Scene.h"
#include "resource.h"

class EmptyScene : public SceneListener
{
	DECLARE_RTTI_CLASS(EmptyScene, SceneListener, ClassFlag_Intrinsic );
};

IMPLEMENT_RTTI_CLASS(EmptyScene )


class DemoFramework : public EngineEventListener
{
public:
	CameraActorPtr mCameraActor;

	DemoFramework( )
	{
		GSceneMaster		= new SceneMaster;
	}

	virtual ~DemoFramework( )
	{
		SafeDelete( GSceneMaster );
	}


	void OnEngineLoad( )
	{ 
		IInputDriver* InputDrv = GEngine->GetWindow( 0 )->GetInputDriver();
		mCameraActor = RTTI_CAST( CameraActor, GEngine->GetWorld(0)->SpawnActor( RTTI_CLASS_INFO(ActorFactoryFPSCamera), L"SampleCamera", null, InputDrv ) );

		GEngine->GetRenderTarget(0)->AddSceneView( GEngine->GetWorld(0), mCameraActor->GetCamera() );

		// 场景创建
		GSceneMaster->Create( );

		CmdLineParser CmdParser;

		NameRef InitSceneName( CmdParser.GetArgContent( L"-initscene" ), NameBuild_Find );

		// 使用命令行标识场景进入游戏
		if ( InitSceneName.IsValid() )
		{
			GSceneMaster->Goto( InitSceneName );
		}
		// 使用默认场景
		else
		{
			GSceneMaster->Goto( RTTI_CLASS_NAME(EmptyScene) );
		}
	}

	void OnEngineUnload( )
	{ 
		// 场景卸载
		GSceneMaster->Destroy( );
		GSceneMaster->Release( );

		//GEngine->GetWorld(0)->Kill( mCameraActor );
	}

	virtual void OnEngineFrameBegin( float Elapsed )
	{ 
		GSceneMaster->Update(Elapsed);

		GEngine->CheckEngineShortcut();
	}

	virtual void OnEngineFrameEnd( float Elapsed )
	{
		
	}

};

static void OnPropertyAutoSleep( RTTIObject* Instance, bool* Value, bool IsGet )
{
	if ( IsGet )
	{
		*Value = Application::GetAutoSleep();
	}
	else
	{
		Application::SetAutoSleep( *Value );
	}
}





int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{

	NEKO_CORE_INIT
	NEKO_ENGINE_INIT

	Application::SetStartupPathByExecutable();

	RTTIGroup* SceneListenerGroup = RTTIObject::GetGroup( L"SceneListener" );
	RTTI_REGISTER_GROUP( EmptyScene, SceneListenerGroup )
	RTTI_REGISTER_GROUP( Demo_Canvas, SceneListenerGroup );
	RTTI_REGISTER_GROUP( Demo_Model, SceneListenerGroup );
	RTTI_REGISTER_GROUP( Demo_Material, SceneListenerGroup );
	RTTI_REGISTER_GROUP( Demo_RenderTarget, SceneListenerGroup );
	RTTI_REGISTER_GROUP( Demo_Pick, SceneListenerGroup );
	RTTI_REGISTER_GROUP( Demo_Transform, SceneListenerGroup );
	RTTI_REGISTER_GROUP( Demo_Font, SceneListenerGroup );

	//播放器匹配AutoSleep功能,编辑器有自己的实现方法
	{
		PropertyCollection* PC = RTTI_CLASS_INFO(GameEngine)->GetPropertyCollection( );

		PropertyGroup* Group = PC->QueryGroup( L"GameEngine" );
		Assert( Group != null );
		PC->Add( PROPERTY_CALLBACK( GameEngine, bool, L"AutoSleep", OnPropertyAutoSleep ), Group );
	}
	

	GameEngine Engine;

	{
		DemoFramework Demo;


		GameWindow* Window = GEngine->CreateGameWindow(L"Neko", 800, 600 , false, IDI_ICON1 );


		GEngine->Initialize( &Demo );

		GEngine->CreateWorld();

		Demo.OnEngineLoad();

		Window->Go();

		GEngine->SetEventListener( null );

		Demo.OnEngineUnload();
	}


	Assert( GEngine != null );

	GEngine->Finalize();



	return TRUE;
}