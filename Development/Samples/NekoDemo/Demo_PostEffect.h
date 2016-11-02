#pragma once


class Demo_PostEffect : public SceneListener
{
	DECLARE_RTTIOBJECT(Demo_PostEffect);

public:
	virtual void OnEnter( );

	virtual void OnLeave( );

	virtual void OnRender( RenderContext* rc  );
	void OnRenderGeometry( RenderCompositor* cp, RenderContext* rc );
private:
	RenderCompositor mCompositor;
	RawSkinModel* mRawModel;
};