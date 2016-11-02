#include "EngineDemoPCH.h"
#include "PCH_Scene.h"

IMPLEMENT_RTTI_CLASS(Demo_Material)


Demo_Material::Demo_Material()
{

}



void Demo_Material::GenerateMaterial( )
{

	//{
	//	MaterialInstance* MatIns = GResourceMaster->Create<MaterialInstance>(L"EngineModel.Teapot.MI_Teapot" );
	//	MatIns->AddMaterialName(L"EngineMaterial.MT_DefaultModelMaterial");
	//	MatIns->AddTextureName(L"EngineTexture.TE_test");
	//	MaterialInstanceData Config;
	//	Config.mMaterialShaderIndex = 0;
	//	Config.mTextureUseList.push_back( 0 );
	//	MatIns->AddInstance( Config );
	//	MatIns->SaveResouce( false );
	//}

	//{
	//	MaterialInstance* MatIns = GResourceMaster->Create<MaterialInstance>(L"EngineModel.Teapot.MI_TeapotSpecularLigting" );
	//	MatIns->AddMaterialName(L"EngineMaterial.MT_SpecularLighting");
	//	MatIns->AddTextureName(L"EngineTexture.TE_Fieldstone");
	//	MaterialInstanceData Config;
	//	Config.mMaterialShaderIndex = 0;
	//	Config.mTextureUseList.push_back( 0 );
	//	MatIns->AddInstance( Config );
	//	MatIns->SaveResouce( false );
	//}

	//{
	//	MaterialInstance* MatIns = GResourceMaster->Create<MaterialInstance>(L"EngineModel.Teapot.MI_TeapotBumpMap" );
	//	MatIns->AddMaterialName(L"EngineMaterial.MT_BumpMap");
	//	MatIns->AddTextureName(L"EngineTexture.TE_Fieldstone");
	//	MatIns->AddTextureName(L"EngineTexture.TE_FieldstoneBumpDOT3");
	//	MaterialInstanceData Config;
	//	Config.mMaterialShaderIndex = 0;
	//	Config.mTextureUseList.push_back( 0 );
	//	Config.mTextureUseList.push_back( 1 );
	//	MatIns->AddInstance( Config );
	//	MatIns->SaveResouce( false );
	//}
}



void Demo_Material::OnEnter()
{
	GenerateMaterial();

	mTeapot = GEngine->GetWorld(0)->SpawnActor( RTTI_CLASS_INFO(ActorFactoryStaticMesh),L"Teapot" );
	StaticMeshComponent* Component = mTeapot->GetComponent<StaticMeshComponent>();
	Component->SetMesh( GObjectManager->LoadObject<StaticMeshResource>(L"Model\\teapot.msh" ));
	Component->SetMaterialInstance( GObjectManager->LoadObject<MaterialInstance>(L"Model\\teapot.mti") );

	
}

void Demo_Material::OnLeave()
{
	//mTeapot.Release();
}

void Demo_Material::OnUpdate( float elapse )
{
	if ( mTeapot != null )
		mTeapot->Tick( elapse );

	StaticMeshComponent* Component = mTeapot->GetComponent<StaticMeshComponent>();

	IInputDriver* InputDrv = GEngine->GetWindow( 0 )->GetInputDriver();

	if ( InputDrv->IsKeyUp( KB_1 ) )
	{
		Component->SetMaterialInstance( GObjectManager->LoadObject<MaterialInstance>(L"Model\\teapot.mti"  ) );
	}
	if ( InputDrv->IsKeyUp( KB_2 ) )
	{
		Component->SetMaterialInstance( GObjectManager->LoadObject<MaterialInstance>(L"Model\\teapot_bumpmap.mti") );
	}
	if ( InputDrv->IsKeyUp( KB_3 ) )
	{
		Component->SetMaterialInstance( GObjectManager->LoadObject<MaterialInstance>(L"Model\\teapot_specularlighting.mti" ) );
	}

}
