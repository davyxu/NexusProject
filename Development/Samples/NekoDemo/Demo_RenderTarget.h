#pragma once


class Demo_RenderTarget : public SceneListener
{
	DECLARE_RTTI_CLASS(Demo_RenderTarget, SceneListener, ClassFlag_Intrinsic);

public:
	virtual void OnEnter( );

	virtual void OnLeave( );


private:
	ActorPtr mTeapot;
	ActorPtr mShark;

	GameWorld* mWorld1;
	GameWorld* mWorld2;

	RenderTargetableTexture2D* RTTexture;

	GameWindow* mWindow;
};