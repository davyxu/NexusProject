#pragma once

class Demo_Pick : public SceneListener
{
	DECLARE_RTTI_CLASS(Demo_Pick, SceneListener, ClassFlag_Intrinsic);

public:
	Demo_Pick( );

	virtual void OnCreate( );

	virtual void OnDestroy( );

	virtual void OnUpdate( float elapse );

	virtual void OnEnter();

	virtual void OnLeave();
private:
	ActorPtr mTeaportArray[1];
};