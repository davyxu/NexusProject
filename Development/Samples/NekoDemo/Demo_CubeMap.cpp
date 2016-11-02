#include "EngineDemoPCH.h"

IMPLEMENT_SCENELISTENERBJECT(Demo_CubeMap)

Demo_CubeMap::Demo_CubeMap()
{

}

void Demo_CubeMap::OnCreate()
{
	
}

void Demo_CubeMap::OnDestroy()
{
	
}

void Demo_CubeMap::OnEnter()
{
	//GArchiveMaster.MountPack( L"pack/model.ngp", L"model" );
	//GArchiveMaster.MountPack( L"pack/tex.ngp", L"tex" );

	mTeapot = GModelMaster.CacheResource<RawModel>( L"model\\teapot" );
	mSphere = GModelMaster.CacheResource<RawModel>( L"model\\sphere" );

	mCubeTexture.SetName( L"tex\\Mars.dds");
	mCubeMapEffect.mTexture.SetTexture( &mCubeTexture );
	mSphereMapEffect.mTexture.SetTexture( &mCubeTexture );
}

void Demo_CubeMap::OnLeave()
{
	SafeRelease( mTeapot );
	SafeRelease( mSphere );
	GRenderDriver->ManualUncache( &mCubeMapEffect );
	GRenderDriver->ManualUncache( &mSphereMapEffect );
	GRenderDriver->ManualUncache( &mCubeTexture );
}


void Demo_CubeMap::OnUpdate( float elapse )
{

}

void Demo_CubeMap::OnRender( RenderContext* rc  )
{
	Camera* camera = rc->GetCamera();

	if ( mSphere != null )
	{
		mSphereMapEffect.mEyePosition.SetVector3( camera->mPosition );
		mSphereMapEffect.mViewProject.SetMatrix( (camera->mViewMatrix * camera->mProjectMatrix).Transpose() );

		mSphere->GetFirstSubMesh()->Render( rc, &mSphereMapEffect );
	}

	if ( mTeapot != null )
	{
		mCubeMapEffect.mWorld.SetMatrix( Matrix4::cIdentity );
		mCubeMapEffect.mEyePosition.SetVector3( camera->mPosition );
		mCubeMapEffect.mViewProject.SetMatrix( (camera->mViewMatrix * camera->mProjectMatrix).Transpose() );

		mTeapot->GetFirstSubMesh()->Render( rc, &mCubeMapEffect );
	}
}