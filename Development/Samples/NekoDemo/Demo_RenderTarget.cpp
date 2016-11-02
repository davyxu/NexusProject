#include "EngineDemoPCH.h"
#include "PCH_Scene.h"

IMPLEMENT_RTTI_CLASS(Demo_RenderTarget)

void Demo_RenderTarget::OnEnter()
{
	mWorld1 = GEngine->GetWorld( 0 );

	mTeapot = mWorld1->SpawnActor( RTTI_CLASS_INFO(ActorFactoryStaticMesh),L"Teapot", mShark );
	{
		StaticMeshComponent* Component = mTeapot->GetComponent<StaticMeshComponent>();
		Component->SetMesh( GObjectManager->LoadObject<StaticMeshResource>(L"EngineModel.Teapot.teapot.SM" ));

		Component->SetMaterialInstance( GObjectManager->LoadObject<MaterialInstance>(L"EngineMaterial.DiffuseLighting.MI") );
	}




	mWorld2 = GEngine->CreateWorld( );

	mShark = mWorld2->SpawnActor( RTTI_CLASS_INFO(ActorFactorySkeletonMesh),L"Shark" );
	{
		SkeletonMeshComponent* Component = mShark->GetComponent<SkeletonMeshComponent>();
		Component->SetSkin( GObjectManager->LoadObject<SkeletonMeshResource>(L"EngineModel.Shark.Shark.SK" )  );	
		Component->SetAnimation( GObjectManager->LoadObject<SkeletonAnimResource>(L"EngineModel.Shark.Shark.SA" ) );

		Component->SetMaterialInstance( GObjectManager->LoadObject<MaterialInstance>(L"EngineMaterial.SpecularLighting_Skinned.MI") );
	}



	RenderTarget* RT = GEngine->GetRenderTarget(0);
	RT->GetView( 0 )->SetDimension( 0.0f, 0.0f, 0.5f, 1.0f );

	//Camera* Cam2 = mWorld2->SpawnActor( RTTI_CLASS_INFO(ActorFactoryFPSCamera), L"Cam2" );

	//RT->AddRenderView( mWorld2,Cam2, 1, 0.5f, 0,0.5f,1.0f );


	////RTTexture = new RenderTargetableTexture2D;
	////
	////RTTexture->AddRenderView( mWorld2, Cam2, 4, 0.5f, 0.5f, 0.5f, 0.5f );
	////RTTexture->SetScaleSize( RT->GetView( 0 ), 0.5f, 0.5f );

	////RTTexture->BeginInitResource();
	////
	//
	//mWindow = GEngine->CreateGameWindow(L"OutSide view", 400, 300 , false );

	//
	//mWindow->AddRenderView( mWorld2, Cam2 );
}






void Demo_RenderTarget::OnLeave()
{
	//RTTexture->BeginReleaseResource();
	//RTTexture->WaitRelease();
	//SafeDelete( RTTexture );
}


