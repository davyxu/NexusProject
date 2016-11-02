#include "EnginePCH.h"
#include "NekoEngine_System.h"
#include "NekoEngine_Model.h"
#include "NekoEngine_Extend.h"

namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS( ActorFactory )

	IMPLEMENT_RTTI_CLASS( ActorFactoryStaticMesh )
	Actor* ActorFactoryStaticMesh::CreateActor( const WString& Name, void* Parameter )
	{
		Actor* NewActor = GObjectManager->ConstructObject<Actor>( Name );

		NewActor->AddComponent<StaticMeshComponent>();
		//NewActor->AddComponent<ActorControllerComponent>( );

		return NewActor;
	}


	IMPLEMENT_RTTI_CLASS( ActorFactorySkeletonMesh)
	Actor* ActorFactorySkeletonMesh::CreateActor( const WString& Name, void* Parameter )
	{
		Actor* NewActor = GObjectManager->ConstructObject<Actor>( Name );
		
		NewActor->AddComponent<SkeletonMeshComponent>();

		return NewActor;
	}


	IMPLEMENT_RTTI_CLASS( ActorFactoryArcBallCamera )
	Actor* ActorFactoryArcBallCamera::CreateActor( const WString& Name, void* Parameter )
	{
		Actor* NewActor = GObjectManager->ConstructObject<CameraActor>( Name );

		NewActor->AddComponent<FPSActorControllerComponent>()->SetInputDriver( (IInputDriver*)Parameter );

		return NewActor;
	}


	IMPLEMENT_RTTI_CLASS( ActorFactoryFPSCamera )
	Actor* ActorFactoryFPSCamera::CreateActor( const WString& Name, void* Parameter )
	{
		Actor* NewActor = GObjectManager->ConstructObject<CameraActor>( Name );

		NewActor->AddComponent<FPSActorControllerComponent>()->SetInputDriver( (IInputDriver*)Parameter );

		return NewActor;
	}

	IMPLEMENT_RTTI_CLASS( ActorFactoryFocusCamera )
	Actor* ActorFactoryFocusCamera::CreateActor( const WString& Name, void* Parameter )
	{
		Actor* NewActor = GObjectManager->ConstructObject<CameraActor>( Name );

		NewActor->AddComponent<FocusActorControllerComponent>()->SetInputDriver( (IInputDriver*)Parameter );

		return NewActor;
	}

}