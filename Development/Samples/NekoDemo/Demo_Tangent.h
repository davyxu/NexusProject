#pragma once

class Demo_Tangent : public SceneListener
{
	DECLARE_RTTI_CLASS(Demo_Tangent, SceneListener, ClassFlag_Intrinsic);

public:
	Demo_Tangent( );

	virtual void OnCreate( );

	virtual void OnDestroy( );

	virtual void OnUpdate( float elapse );

	virtual void OnEnter();

	virtual void OnLeave();
private:
	Actor* mModel;
};