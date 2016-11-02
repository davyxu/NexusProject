#include "EngineDemoPCH.h"
#include "PCH_Scene.h"

IMPLEMENT_RTTI_CLASS(Demo_Canvas)

void Demo_Canvas::OnEnter()
{
	TexturePtr1 = GObjectManager->LoadObject<TextureResource>( L"EngineTexture.test.DDS" );	
	
}





void Demo_Canvas::OnUpdate( float elapse )
{
	//Canvas canvas( GEngine->GetRenderTarget(0)->GetView( 0 )->GetViewport() );
	//canvas.DrawTile( Vector2(100.0f, 100.0f), Vector2(100.0f, 100.0f) + Vector2((float)TexturePtr1->GetSizeX(), (float)TexturePtr1->GetSizeY()),Color::cWhite, TexturePtr1->GetRenderTexture() );

	////canvas.DrawTile( 120.0f, 120.0f, (float)TexturePtr2->GetSizeX(), (float)TexturePtr2->GetSizeY(),Color::cWhite, TexturePtr2->GetRenderTexture()  );


	//canvas.Flush();

}


void Demo_Canvas::OnLeave()
{
	TexturePtr1.Release();


	TexturePtr2.Release();
}