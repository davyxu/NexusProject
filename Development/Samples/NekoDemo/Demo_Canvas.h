#pragma once


class Demo_Canvas : public SceneListener
{
	DECLARE_RTTI_CLASS(Demo_Canvas, SceneListener, ClassFlag_Intrinsic);

public:
	virtual void OnEnter( );

	virtual void OnLeave( );

	virtual void OnUpdate( float elapse );

private:
	TextureResourcePtr TexturePtr1;
	TextureResourcePtr TexturePtr2;
};