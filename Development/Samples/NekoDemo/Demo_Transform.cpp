#include "EngineDemoPCH.h"
#include "PCH_Scene.h"

IMPLEMENT_RTTI_CLASS(Demo_Transform)

Demo_Transform::Demo_Transform()
: mRotB( 0.0f )
, mRotC( 0.0f )
{

}


void Demo_Transform::OnCreate()
{

}

void Demo_Transform::OnEnter()
{
	mTeapotA = CreateTestActor( Color().SetColor255(255,250,72,20), null );

	mTeapotB = CreateTestActor( Color().SetColor255(255,228,252,18), mTeapotA );

	mTeapotB->Translate( Vector3( 30.0f, 30.0f, 0.0f ));

	mTeapotC = CreateTestActor( Color().SetColor255(255,120,233,37), mTeapotB );

	mTeapotC->Translate( Vector3( 10.0f, 10.0f, 0.0f ));

	mTeapotA->Translate( Vector3( 10.0f, 10.0f, 10.0f));

	mTransformB.mRotationSpeed *= -2.0f;

	mTransformC.mRotationSpeed *= 3.0f;

	mTeapotA->Scale( Vector3( 2.0f, 2.0f , 2.0f ));

	mTeapotC->Scale( Vector3( 0.5f, 0.5f , 0.5f ));
	
}

Actor* Demo_Transform::CreateTestActor( const Color& C, Actor* Parent )
{
	
	Actor* A = GEngine->GetWorld(0)->SpawnActor( RTTI_CLASS_INFO(ActorFactoryStaticMesh),L"Test", Parent );
	{
		MaterialDescriptorPtr Mat = GObjectManager->LoadObject<MaterialDescriptor>(L"EngineMaterial.MT_ConstantColor");

		StaticMeshComponent* Component = A->GetComponent<StaticMeshComponent>();
		Component->SetMesh( GObjectManager->LoadObject<StaticMeshResource>(L"EngineModel.Teapot.SM_teapot" ));


		MaterialInstancePtr MatIns = GObjectManager->ConstructObject<MaterialInstance>( value_tostringW( C ) );
		MatIns->LinkMaterial( Mat );
		MatIns->SetColor( L"Color",  C );
	

		Component->SetMaterialInstance( MatIns );
	}

	return A;
}



void Demo_Transform::OnLeave()
{

}

void Demo_Transform::OnDestroy()
{

}

void Demo_Transform::OnUpdate( float Elapse )
{
	ModifyTransform( mTeapotA, mTransformA, Elapse ); 
	ModifyTransform( mTeapotB, mTransformB, Elapse );
	ModifyTransform( mTeapotC, mTransformC, Elapse );

	if ( 0 )
	{
		mTeapotA->SetPosition( Vector3( 10, 0, 0 ));
	}
}


void Demo_Transform::ModifyTransform( Actor* A, TransformProfile& Profile, float Elapse )
{	
	A->Rotate( Quaternion().RotationZ( Profile.mRotationSpeed ) );
}

Demo_Transform::~Demo_Transform()
{

}

