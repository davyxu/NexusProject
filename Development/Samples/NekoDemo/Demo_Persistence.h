#pragma once


class Demo_Persistence : public SceneListener
{
	DECLARE_RTTI_CLASS(Demo_Persistence, SceneListener, ClassFlag_Intrinsic )

public:
	Demo_Persistence( );

	

	virtual void OnEnter();

	virtual void OnLeave();
};