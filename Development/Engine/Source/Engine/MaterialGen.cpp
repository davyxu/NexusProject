#include "EnginePCH.h"
#include "Engine/FileVersion.h"
#include "NekoEngine_Material.h"

namespace NekoEngine
{

	void SaveMaterial( const wchar* MatLocation, MaterialDescriptor* Mat, MaterialInstance* MatIns )
	{
		WString ShaderFile;
		ResourceLocation::ModifyType( MatLocation,  ShaderFile, RFT_ShaderFile );

		Mat->GenerateShaderFile( ShaderFile.c_str() );

		
		MatIns->LinkMaterial( Mat );

		GObjectManager->SaveToFile( MatIns, FILE_VER_MATERIAL_INS);

		GObjectManager->SaveToFile( Mat, FILE_VER_MATERIAL_DESCRIPTOR );
	}

	void CreateMaterial( const wchar* Location, MaterialDescriptorPtr& Mat, MaterialInstancePtr& MatIns )
	{
		Mat = GObjectManager->ConstructObject<MaterialDescriptor>( Location );

		WString MILocation;
		ResourceLocation::ModifyType( Location, MILocation, RFT_MaterialInstance );
		MatIns = GObjectManager->ConstructObject<MaterialInstance>( MILocation );
	}

	void MaterialHelper::GenerateConstantColorMaterial( const wchar* Location, bool Skinned )
	{
		MaterialDescriptorPtr Mat;
		MaterialInstancePtr MatIns;
		CreateMaterial( Location, Mat, MatIns );

		if ( Skinned )
			MatIns->SetObjectPropertyT( L"UseSkeletonMesh", true );


		MaterialNodeUniformVector* MixColor = Mat->AddNode<MaterialNodeUniformVector>();
		MixColor->mParameterData->SetName( L"Color");


		MaterialNodePointLight* PointLightNode = Mat->AddNode<MaterialNodePointLight>( );

		PointLightNode->mEmissive.Connect( MixColor->GetID(), MVM_RGB );
		PointLightNode->mOpacity.Connect( MixColor->GetID(), MVM_A );

		SaveMaterial( Location, Mat , MatIns );
	}



	void MaterialHelper::GenerateDiffuseLightingMaterial( const wchar* Location, bool Skinned )
	{
		MaterialDescriptorPtr Mat;
		MaterialInstancePtr MatIns;
		CreateMaterial( Location, Mat, MatIns );

		if ( Skinned )
			MatIns->SetObjectPropertyT( L"UseSkeletonMesh", true );
		
		MaterialNodeTextureSampler* BaseMap = Mat->AddNode<MaterialNodeTextureSampler>( );
		BaseMap->mSampler->SetName( L"DiffuseTexture" );
		BaseMap->mSampler->SetValue( L"EngineTexture.test.DDS" );

		MaterialNodeConstant* MixColor = Mat->AddNode<MaterialNodeConstant>();
		MixColor->mData = Color::cWhite;

		MaterialNodeMathOperator* Mixer = Mat->AddNode<MaterialNodeMathOperator>();
		Mixer->mOperatorType = MMO_Add;

		Mixer->mDataX.Connect( BaseMap->GetID(), MVM_RGB );
		Mixer->mDataY.Connect( MixColor->GetID(), MVM_RGB );



		MaterialNodePointLight* PointLightNode = Mat->AddNode<MaterialNodePointLight>( );

		PointLightNode->mDiffuse.Connect( BaseMap->GetID(), MVM_RGB );

		SaveMaterial( Location, Mat , MatIns );
	}



	void MaterialHelper::GenerateSpecularLigtingMaterial( const wchar* Location, bool Skinned , const wchar* BaseMapTextureName )
	{
		MaterialDescriptorPtr Mat;
		MaterialInstancePtr MatIns;
		CreateMaterial( Location, Mat, MatIns );

		if ( Skinned )
			MatIns->SetObjectPropertyT( L"UseSkeletonMesh", true );

		MaterialNodeTextureSampler* BaseMap = Mat->AddNode<MaterialNodeTextureSampler>( );
		BaseMap->mSampler->SetName( BaseMapTextureName );
		BaseMap->mSampler->SetValue( BaseMapTextureName );

		MaterialNodeConstant* SpecularColor = Mat->AddNode<MaterialNodeConstant>();
		SpecularColor->mData = Color::cWhite;

		MaterialNodePointLight* PointLightNode = Mat->AddNode<MaterialNodePointLight>( );

		PointLightNode->mDiffuse.Connect( BaseMap->GetID(), MVM_RGB );
		PointLightNode->mSpecular.Connect( SpecularColor->GetID(), MVM_RGB );

		SaveMaterial( Location, Mat , MatIns );
	}



	void MaterialHelper::GenerateBumpMapMaterial( const wchar* Location , bool Skinned )
	{
		MaterialDescriptorPtr Mat;
		MaterialInstancePtr MatIns;
		CreateMaterial( Location, Mat, MatIns );

		MatIns->SetObjectPropertyT( L"UseTangentSpace", true );

		if ( Skinned )
			MatIns->SetObjectPropertyT( L"UseSkeletonMesh", true );


		MaterialNodeTextureSampler* BaseMap = Mat->AddNode<MaterialNodeTextureSampler>( );
		BaseMap->mSampler->SetName( L"BaseMap" );
		BaseMap->mSampler->SetValue( L"EngineTexture.Fieldstone.DDS" );

		MaterialNodeTextureSampler* NormalMap = Mat->AddNode<MaterialNodeTextureSampler>( );
		NormalMap->mSampler->SetName( L"NormalMap" );
		NormalMap->mSampler->SetValue( L"EngineTexture.FieldstoneBumpDOT3.DDS" );

		MaterialNodeBumpOffset* NormalOffset = Mat->AddNode<MaterialNodeBumpOffset>();
		NormalOffset->mUV.Connect( NormalMap->GetID(), MVM_RGB );

		MaterialNodeConstant* SpecularColor = Mat->AddNode<MaterialNodeConstant>();
		SpecularColor->mData = Color::cWhite;

		MaterialNodePointLight* PointLightNode = Mat->AddNode<MaterialNodePointLight>( );

		PointLightNode->mDiffuse.Connect( BaseMap->GetID(), MVM_RGB );
		PointLightNode->mNormal.Connect( NormalOffset->GetID(), MVM_RGB );
		PointLightNode->mSpecular.Connect( SpecularColor->GetID(), MVM_RGB );

		SaveMaterial( Location, Mat , MatIns );
	}

	void MaterialHelper::GeneratePureEmissiveMaterial( const wchar* Location, bool Skinned )
	{
		MaterialDescriptorPtr Mat;
		MaterialInstancePtr MatIns;
		CreateMaterial( Location, Mat, MatIns );

		if ( Skinned )
			MatIns->SetObjectPropertyT( L"UseSkeletonMesh", true );
		MatIns->SetObjectPropertyT( L"TwoSided", true );

		MaterialNodeTextureSampler* BaseMap = Mat->AddNode<MaterialNodeTextureSampler>( );
		BaseMap->mSampler->SetName( L"DiffuseTexture" );
		BaseMap->mSampler->SetValue( L"EngineTexture.test.DDS" );

		MaterialNodeConstant* MixColor = Mat->AddNode<MaterialNodeConstant>();
		MixColor->mData = Color::cWhite;

		MaterialNodeMathOperator* Mixer = Mat->AddNode<MaterialNodeMathOperator>();
		Mixer->mOperatorType = MMO_Add;

		Mixer->mDataX.Connect( BaseMap->GetID(), MVM_RGB );
		Mixer->mDataY.Connect( MixColor->GetID(), MVM_RGB );



		MaterialNodePointLight* PointLightNode = Mat->AddNode<MaterialNodePointLight>( );

		PointLightNode->mEmissive.Connect( BaseMap->GetID(), MVM_RGB );

		SaveMaterial( Location, Mat , MatIns );
	}
}