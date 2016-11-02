#include "EngineDemoPCH.h"
#include "PCH_Scene.h"

IMPLEMENT_RTTI_CLASS(Demo_Model)

Demo_Model::Demo_Model()
{

}


void Demo_Model::OnCreate()
{
	
}

void Demo_Model::OnEnter()
{
	if ( File::Exists(L"genmat.cfg") )
	{
		MaterialHelper::GenerateDiffuseLightingMaterial( L"EngineMaterial.DiffuseLighting.MT", false );
		MaterialHelper::GenerateDiffuseLightingMaterial( L"EngineMaterial.DiffuseLighting_Skinned.MT", true );
		MaterialHelper::GenerateSpecularLigtingMaterial( L"EngineMaterial.SpecularLighting.MT", false, L"" );
		MaterialHelper::GenerateSpecularLigtingMaterial( L"EngineMaterial.SpecularLighting_Skinned.MT", true, L"EngineModel.Shark.shark_hurt_heavy.DDS" );
		MaterialHelper::GenerateBumpMapMaterial( L"EngineMaterial.BumpMap.MT", false );
		MaterialHelper::GenerateBumpMapMaterial( L"EngineMaterial.BumpMap_Skinned.MT", true );
		MaterialHelper::GenerateConstantColorMaterial( L"EngineMaterial.ConstantColor.MT", false );
		MaterialHelper::GeneratePureEmissiveMaterial( L"EngineMaterial.PureEmissive.MT", true );
	}
	
	
	//mShark = GEngine->GetWorld(0)->SpawnActor( RTTI_CLASS_INFO(ActorFactorySkeletonMesh),L"Shark" );
	//{
	//	SkeletonMeshComponent* Component = mShark->GetComponent<SkeletonMeshComponent>();
	//	//Component->SetSkin( GObjectManager->LoadObject<SkinResource>(L"EngineModel.Shark.Shark.SK" )  );	
	//	//Component->SetAnimation( GObjectManager->LoadObject<AnimationResource>(L"EngineModel.Shark.Shark.SA" ) );

	//	mShark->SetPropertyByString<SkeletonMeshComponent>( L"Skin", L"EngineModel.Shark.Shark.SK");
	//	mShark->SetPropertyByString<SkeletonMeshComponent>( L"Animation", L"EngineModel.Shark.Shark.SA");
	//	mShark->SetPropertyByString<SkeletonMeshComponent>( L"Material", L"EngineMaterial.SpecularLighting_Skinned.MI" );
	//	

	//	//Component->SetSkin( GObjectManager->LoadObject<SkinResource>(L"EngineModel.SkinTest.SkinTest.SK" )  );	
	//	//Component->SetAnimation( GObjectManager->LoadObject<AnimationResource>(L"EngineModel.SkinTest.SkinTest.SA" ) );

	//	//Component->SetMaterial( GObjectManager->LoadObject<MaterialInstance>(L"EngineMaterial.MI_DiffuseLighting_Skinned") );
	//	//Component->SetMaterialInstance( GObjectManager->LoadObject<MaterialInstance>(L"EngineMaterial.SpecularLighting_Skinned.MI") );
	//}
	//

	mTeapot = GEngine->GetWorld(0)->SpawnActor( RTTI_CLASS_INFO(ActorFactoryStaticMesh), L"Teapot" );
	{
		StaticMeshComponent* Component = mTeapot->GetComponent<StaticMeshComponent>();
		Component->SetMesh( GObjectManager->LoadObject<StaticMeshResource>(L"EngineModel.Teapot.teapot.SM" ));		
		Component->SetMaterialInstance( GObjectManager->LoadObject<MaterialInstance>(L"EngineMaterial.DiffuseLighting.MI") );
	}

	mTeapot->Translate( Vector3(30.0f, 0.0f, 0.0f) );


}

void Demo_Model::OnLeave()
{
	//GEngine->GetWorld( 0 )->Kill( mShark );
	//mShark.Release();
	//mTeapot.Release();
}

void Demo_Model::OnDestroy()
{

}

void Demo_Model::OnUpdate( float elapse )
{
	
}
