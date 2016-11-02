#pragma once

class Demo_CubeMap : public SceneListener
{
	DECLARE_RTTIOBJECT(Demo_CubeMap);

public:
	Demo_CubeMap( );

	virtual void OnCreate( );

	virtual void OnDestroy( );

	virtual void OnUpdate( float elapse );

	virtual void OnRender( RenderContext* rc  );

	virtual void OnEnter();

	virtual void OnLeave();

private:
	RawModel* mTeapot;
	RawModel* mSphere;

	struct CubeMapEffect : RenderEffect
	{
		ShaderParameter	mViewProject;
		ShaderParameter	mWorld;
		ShaderParameter	mTexture;
		ShaderParameter	mEyePosition;	

		CubeMapEffect( )
		{
			SetName( L"material/cubemap.effect" );
			Bind( &mViewProject, "matViewProj"  );
			Bind( &mWorld, "matWorld");
			Bind( &mTexture, "texCubeMap" );
			Bind( &mEyePosition, "vecEyePosition" );
		}
	};

	CubeMapEffect mCubeMapEffect;

	struct SphereMapEffect : RenderEffect
	{
		ShaderParameter	mViewProject;
		ShaderParameter	mTexture;
		ShaderParameter	mEyePosition;	

		SphereMapEffect( )
		{
			SetName( L"material\\spheremap.effect" );
			Bind( &mViewProject, "matViewProj"  );
			Bind( &mTexture, "texEnvMap" );
			Bind( &mEyePosition, "vecEyePosition" );
		}
	};

	SphereMapEffect mSphereMapEffect;

	CubeTexture	mCubeTexture;
};