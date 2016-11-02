#include "EngineDemoPCH.h"
#include "PCH_Scene.h"

IMPLEMENT_RTTI_CLASS(Demo_Pick)

Demo_Pick::Demo_Pick()
{

}


void Demo_Pick::OnCreate()
{

}

void Demo_Pick::OnEnter()
{

	for ( int i = 0;i< ArraySize( mTeaportArray );i++)
	{
		mTeaportArray[i] = GEngine->GetWorld(0)->SpawnActor( RTTI_CLASS_INFO(ActorFactoryStaticMesh),L"" );
		{
			StaticMeshComponent* Component = mTeaportArray[i]->GetComponent<StaticMeshComponent>();
			Component->SetMesh( GObjectManager->LoadObject<StaticMeshResource>(L"EngineModel.Teapot.SM_teapot" ));
			Component->SetMaterialInstance( GObjectManager->LoadObject<MaterialInstance>(L"EngineMaterial.MI_DiffuseLighting") );		
		}

		mTeaportArray[i]->SetPosition( Vector3( (float)(i % 10 *20 - 100), (float)(i / 10 * 20 - 100), 0 ) );

		//mTeaportArray[i]->SetUserData( i );

		//GEngine->GetLogger()->Debug(L"%f, %f", mTeaportArray[i]->GetTranslation().x, mTeaportArray[i]->GetTranslation().y );

	}

	
}

void Demo_Pick::OnLeave()
{
	//mShark.Release();
	//mTeaportArray.Release();
}

void Demo_Pick::OnDestroy()
{

}

void Demo_Pick::OnUpdate( float elapse )
{
	for ( int i = 0;i< ArraySize( mTeaportArray );i++)
	{
		mTeaportArray[i]->SetTestColor( -1 );
	}

	IInputDriver* InputDrv = GEngine->GetWindow( 0 )->GetInputDriver();

	if ( InputDrv->IsMouseDown(MB_Left) )
	{
		SceneView* View = GEngine->GetRenderTarget(0)->GetView( 0 );
		Point MP = InputDrv->GetMousePoint();
		Ray PickRay = View->CreateRay( MP.x, MP.y );
		WorldPick Pick( GEngine->GetWorld(0) );
		if ( Pick.Pick( PickRay ) )
		{
			for( int i = 0;i < 1; i++)
			{
				Actor* A = Pick.GetActor(i);

				A->SetTestColor( 1 );
			}	
		}
	}


}
