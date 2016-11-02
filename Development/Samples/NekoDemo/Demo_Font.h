#pragma once

#include "Engine/RenderFont.h"

class Demo_Font :	public SceneListener, 
					public SceneViewListener
{
	DECLARE_RTTI_CLASS(Demo_Font, SceneListener, ClassFlag_Intrinsic);

public:
	Demo_Font( );

	virtual void OnCreate( );

	virtual void OnDestroy( );

	virtual void OnUpdate( float elapse );

	virtual void OnEnter();

	virtual void OnLeave();

	virtual void PostSceneViewDraw( SceneView* RV );

private:
	RenderFont* mFontObject;

};