#pragma once

class Demo_Transform : public SceneListener
{
	DECLARE_RTTI_CLASS(Demo_Transform, SceneListener, ClassFlag_Intrinsic);

public:
	Demo_Transform( );

	virtual ~Demo_Transform( );

	virtual void OnCreate( );

	virtual void OnDestroy( );

	virtual void OnUpdate( float elapse );

	virtual void OnEnter();

	virtual void OnLeave();

private:	
	Actor* CreateTestActor( const Color& C, Actor* Parent );
	
private:
	ActorPtr mTeapotA;
	ActorPtr mTeapotB;
	ActorPtr mTeapotC;

	float mRotB;
	float mRotC;

	struct TransformProfile
	{
		float mRotation;
		float mRotationSpeed;

		TransformProfile()
			: mRotation( 0.0f )
			, mRotationSpeed( 0.001f )
		{

		}
	};

	void ModifyTransform( Actor* A, TransformProfile& Profile, float Elapse );

	TransformProfile mTransformA;
	TransformProfile mTransformB;
	TransformProfile mTransformC;
};