#pragma once

class Demo_Model : public SceneListener
{
	DECLARE_RTTI_CLASS(Demo_Model, SceneListener, ClassFlag_Intrinsic);

public:
	Demo_Model( );

	virtual void OnCreate( );

	virtual void OnDestroy( );

	virtual void OnUpdate( float elapse );

	virtual void OnEnter();

	virtual void OnLeave();
	void PostRenderViewDraw( SceneView* RV );
private:
	Actor* mShark;
	Actor* mTeapot;
};