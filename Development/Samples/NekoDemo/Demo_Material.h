#pragma once

class Demo_Material : public SceneListener
{
	DECLARE_RTTI_CLASS(Demo_Material, SceneListener, ClassFlag_Intrinsic);

public:
	Demo_Material( );

	virtual void OnEnter();

	virtual void OnLeave();

	virtual void OnUpdate( float elapse );

private:
	void GenerateMaterial( );

private:
	Actor* mTeapot;
};