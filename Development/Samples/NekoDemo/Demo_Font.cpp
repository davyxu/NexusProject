#include "EngineDemoPCH.h"
#include "PCH_Scene.h"


IMPLEMENT_RTTI_CLASS(Demo_Font)

Demo_Font::Demo_Font()
{

}


void Demo_Font::OnCreate()
{
	

	RenderFont::StaticInit();
}

void Demo_Font::OnEnter()
{

	GEngine->GetRenderTarget( 0 )->AddListener( this );

	mFontObject = new RenderFont( L"宋体", L"", 10, 0, false,false, false);

}

void Demo_Font::OnLeave()
{
	delete mFontObject;
}

void Demo_Font::OnDestroy()
{
	RenderFont::StaticExit();
}

void Demo_Font::OnUpdate( float elapse )
{
	
}

void Demo_Font::PostSceneViewDraw( SceneView* RV )
{
	//Vector2 v1, v2;
	//mFontObject->GetFontTexture( L'擎', v1, v2 );

	const wchar* a = L" : 554,user(gl#10002625) lock bean total cost 31, time利比亚反对派进入首都中心 正搜捕卡扎菲 快讯卡扎菲长子投降 次子被捕 战斗致逾6千人死伤·全国铁路降速调图方案公布 上铁停运9对高铁列车·拜登川大演讲 称中国繁荣为美带来更多就业机会";
	RV->GetCanvas()->DrawString( mFontObject, Vector2(100, 100), Color::cWhite, L"引擎Console.Print(\"hello world\");");
	if ( 0 )
	{
		mFontObject->Save(L"t.png");
	}
}
