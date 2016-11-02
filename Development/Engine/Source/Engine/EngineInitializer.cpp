#include "EnginePCH.h"
#include "NekoEngine.h"

namespace NekoEngine
{
	EngineInitializer::EngineInitializer()
	{
		InitGlobalObject();
	}

	void EngineInitializer::InitGlobalObject() 
	{

		RTTI_REGISTER( MeshVertexFactory )
			RTTI_REGISTER( SkeletonMeshVertexFactory );
			RTTI_REGISTER( StaticMeshVertexFactory );
		
		RTTIGroup* GlobalShaderGroup = RTTIObject::GetGroup( L"GlobalShader" );

		RTTI_REGISTER( RenderShader );
			RTTI_REGISTER_GROUP( ElementVertexShader, GlobalShaderGroup );
			RTTI_REGISTER_GROUP( ElementPixelShader, GlobalShaderGroup  )
			RTTI_REGISTER_GROUP( ColorBrushPixelShader, GlobalShaderGroup  )
			RTTI_REGISTER( MaterialShader );
				RTTI_REGISTER( MaterialPixelShader );
				RTTI_REGISTER( MaterialVertexShader );
					RTTI_REGISTER( ModelMaterialVertexShader );

		RTTIGroup* ShaderFactoryGroup = RTTIObject::GetGroup( L"ShaderFactory" );
		RTTI_REGISTER( ShaderFactory )
			RTTI_REGISTER_GROUP( MaterialShaderFactory,ShaderFactoryGroup );
			RTTI_REGISTER_GROUP( GlobalShaderFactory,ShaderFactoryGroup );

		RTTI_REGISTER( ManagedObject )
			RTTI_REGISTER( Actor );
				RTTI_REGISTER( CameraActor );
			RTTI_REGISTER( MaterialDescriptor );
			RTTI_REGISTER( MaterialInstance );
			RTTI_REGISTER( SkeletonAnimResource );
			RTTI_REGISTER( TextureResource );
			RTTI_REGISTER( MeshResource );
				RTTI_REGISTER( StaticMeshResource );
				RTTI_REGISTER( SkeletonMeshResource );
		

		RTTI_REGISTER( MaterialNode);
			RTTI_REGISTER( MaterialNodeTextureSampler );
			RTTI_REGISTER( MaterialNodeConstant );
			RTTI_REGISTER( MaterialNodeMathOperator );
			RTTI_REGISTER( MaterialNodeBumpOffset );
			RTTI_REGISTER( MaterialNodePointLight );
			RTTI_REGISTER( MaterialNodeNormalize );
			RTTI_REGISTER( MaterialNodeUniformVector );

		RTTI_REGISTER( MaterialParameter );
			RTTI_REGISTER( MaterialParameterTextureSampler );
			RTTI_REGISTER( MaterialParameterVector );
			RTTI_REGISTER( MaterialParameterBool );

		RTTI_REGISTER( ActorComponent );
			RTTI_REGISTER( GeometryComponent );
				RTTI_REGISTER( CameraControllerComponent )
				RTTI_REGISTER( GridAxisComponent );
				RTTI_REGISTER( ActorHelperComponent );
				RTTI_REGISTER( MeshComponent );
					RTTI_REGISTER( StaticMeshComponent );
					RTTI_REGISTER( SkeletonMeshComponent );
				RTTI_REGISTER( ActorControllerComponent );
					RTTI_REGISTER( FPSActorControllerComponent );
					RTTI_REGISTER( FocusActorControllerComponent );

		RTTI_REGISTER( ActorFactory )
			RTTI_REGISTER( ActorFactoryStaticMesh );
			RTTI_REGISTER( ActorFactorySkeletonMesh );
			RTTI_REGISTER( ActorFactoryArcBallCamera );
			RTTI_REGISTER( ActorFactoryFPSCamera );
			RTTI_REGISTER( ActorFactoryFocusCamera );

		RTTI_REGISTER( SceneListener );
		RTTI_REGISTER( GameWorld );
		RTTI_REGISTER( GameEngine );
		RTTI_REGISTER( ObjectManager );

		ActorHelperComponent::StaticInitProperty();
		MaterialInstance::StaticInitProperty();
		MeshComponent::StaticInitProperty();
		SkeletonMeshComponent::StaticInitProperty();
		StaticMeshComponent::StaticInitProperty();
		GameEngine::StaticInitProperty();
		ObjectManager::StaticInitProperty();
		MaterialNode::StaticInitProperty();

		ManagedObject::StaticInit();


	}

	void EngineInitializer::ExitGlobalObject()
	{

	}

	EngineInitializer::~EngineInitializer()
	{
		ExitGlobalObject();
	}




}