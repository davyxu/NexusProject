#include "EngineDemoPCH.h"

IMPLEMENT_SCENELISTENERBJECT(Demo_PostEffect)

void Demo_PostEffect::OnEnter()
{
	//mRawModel = GModelMaster.CacheResource<RawSkinModel>( L"model\\toy" );
	
	mCompositor.Create( GRenderDriver, &GArchiveMaster );
	mCompositor.RegisterGeometryCallback(L"rt_input", CompositorDelegate( this, &Demo_PostEffect::OnRenderGeometry ) );
	mCompositor.Open( L"material\\bloom.compositor");

	GSceneMaster.Get( RTTI_TYPE_ID(Demo_CubeMap) )->OnEnter();
}

void Demo_PostEffect::OnRenderGeometry( RenderCompositor* cp, RenderContext* rc )
{
	//RenderHelper::DrawAxisGrid( rc->GetRenderDriver()->GetRenderContext() );

	/*if ( mRawModel != null )
		mRawModel->Render( rc->GetRenderDriver()->GetRenderContext() );*/

	GSceneMaster.Get( RTTI_TYPE_ID(Demo_CubeMap) )->OnRender( rc->GetRenderDriver()->GetRenderContext() );
}

void Demo_PostEffect::OnRender( RenderContext* rc  )
{

	mCompositor.Render( rc );
}


void Demo_PostEffect::OnLeave()
{
	GSceneMaster.Get( RTTI_TYPE_ID(Demo_CubeMap) )->OnLeave();

	//SafeRelease( mRawModel );
	mCompositor.Release();
}