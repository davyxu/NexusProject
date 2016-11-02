#pragma once


class ActorFactoryEditor : public ActorFactory
{
	DECLARE_RTTI_CLASS( ActorFactoryEditor, ActorFactory, ClassFlag_Intrinsic )

public:
	virtual Actor* CreateActor( const WString& Name, void* Parameter );	
};


class EditorDrawUtilityComponent : public GeometryComponent
{
	DECLARE_RTTI_CLASS( EditorDrawUtilityComponent, GeometryComponent, ClassFlag_Intrinsic )
public:
	EditorDrawUtilityComponent( );

	virtual void Draw( SceneView* RV );
};