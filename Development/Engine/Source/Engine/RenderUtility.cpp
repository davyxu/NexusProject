#include "EnginePCH.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	//FrameSpeedUnit
	//////////////////////////////////////////////////////////////////////////

	FrameSpeedUnit::FrameSpeedUnit(void)
	{
		mFPSTimer = 0;
		mAccumulator = 0;
		mDisplayNumber = 0;
	}

	void FrameSpeedUnit::Tick()
	{
		mFPSTimer += TimeSource::GetElapsedTime();

		if (mFPSTimer > 1000.0f)
		{
			mFPSTimer		-= 1000.0f;
			mDisplayNumber	= mAccumulator;
			mAccumulator = 0;
		}

		++ mAccumulator;
	}

	int FrameSpeedUnit::GetFramePerSecond( void )
	{
		return mDisplayNumber;
	}

	//////////////////////////////////////////////////////////////////////////
	// RenderHelper
	//////////////////////////////////////////////////////////////////////////

#define USE_POLYGON_AXIS_TEXT

	void RenderHelper::DrawAxisGrid( Camera* Cam, Canvas* CV, const Viewport& VP, float Range, float Size )
	{
		CV->PushTransform( Cam->GetViewMatrix() * Cam->GetProjectMatrix() );

		for (float i = -Range; i <= Range; i += Size)
		{
			CV->DrawLine(Vector3( -Range, i,  0 ), Vector3(Range, i, 0 ),i == 0? Color::cWhite: Color(0.5f,1.0f,1.0f,1.0f) );
			CV->DrawLine(Vector3(   i, -Range, 0), Vector3(i, Range, 0),i == 0? Color::cWhite: Color(0.5f,1.0f,1.0f,1.0f) );
		}

		const  float Distance = 10.0f;

		const Vector3 XPoint( Distance,	0,			0 );
		const Vector3 YPoint( 0,			Distance,	0 );
		const Vector3 ZPoint( 0,			0 ,			Distance );


		CV->DrawLine(Vector3::cZero,YPoint, Color::cGreen );
		CV->DrawLine(Vector3::cZero,ZPoint, Color::cBlue );
		CV->DrawLine(Vector3::cZero,XPoint, Color::cRed );

#ifdef USE_POLYGON_AXIS_TEXT

		/*
		const float FrontDistance = 1.0f;

		Vector3 FrontXPoint( XPoint + Vector3( FrontDistance, 0, FrontDistance ) );
		Vector3 FrontYPoint( YPoint + Vector3( 0, FrontDistance, FrontDistance ) );
		Vector3 FrontZPoint( ZPoint + Vector3( 0,0,FrontDistance ) );

		const float CharOffset = 0.5f;

		// Char X
		CV->DrawLine( FrontXPoint + Vector3( CharOffset, 0, CharOffset), FrontXPoint + Vector3( -CharOffset, 0, -CharOffset), Color::cYellow );
		CV->DrawLine( FrontXPoint + Vector3( -CharOffset, 0, CharOffset), FrontXPoint + Vector3( CharOffset, 0, -CharOffset), Color::cYellow );

		// Char Y
		CV->DrawLine( FrontYPoint + Vector3( 0, CharOffset, CharOffset), FrontYPoint, Color::cYellow );
		CV->DrawLine( FrontYPoint + Vector3( 0, -CharOffset, CharOffset), FrontYPoint, Color::cYellow );
		CV->DrawLine( FrontYPoint + Vector3( 0, 0, -CharOffset), FrontYPoint, Color::cYellow );

		// Char Z
		CV->DrawLine( FrontZPoint + Vector3( CharOffset, CharOffset, 0), FrontZPoint+ Vector3( -CharOffset, CharOffset, 0), Color::cYellow );
		CV->DrawLine( FrontZPoint + Vector3( -CharOffset, CharOffset, 0), FrontZPoint +  Vector3( CharOffset, -CharOffset, 0), Color::cYellow );
		CV->DrawLine( FrontZPoint + Vector3( CharOffset, -CharOffset, 0), FrontZPoint + Vector3( -CharOffset, -CharOffset, 0), Color::cYellow );
		*/
		CV->PopTransform();

		Vector3 XPos = VP.Project( XPoint, *Cam );
		Vector3 YPos = VP.Project( YPoint, *Cam );
		Vector3 ZPos = VP.Project( ZPoint, *Cam );


		const float Scale = 5.0f;

		if ( XPos.z < VP.FarZ )
			DrawVectorFont( CV, Vector2( XPos.x, XPos.y), XFont, Scale );

		if ( YPos.z < VP.FarZ)
			DrawVectorFont( CV, Vector2( YPos.x, YPos.y), YFont, Scale );

		if ( ZPos.z < VP.FarZ)
			DrawVectorFont( CV, Vector2( ZPos.x, ZPos.y), ZFont, Scale );

#else
		Vector3 XPos = VP.Project( XPoint, Cam );
		Vector3 YPos = VP.Project( YPoint, Cam );
		Vector3 ZPos = VP.Project( ZPoint, Cam );


		if ( XPos.z < VP.FarZ )
			RHI->DrawDebugText((int)XPos.x,(int)XPos.y,Color::cYellow,L"X");

		if ( YPos.z < VP.FarZ)
			RHI->DrawDebugText((int)YPos.x,(int)YPos.y,Color::cYellow,L"Y");	

		if ( ZPos.z < VP.FarZ)
			RHI->DrawDebugText((int)ZPos.x,(int)ZPos.y,Color::cYellow,L"Z");	
#endif

		
	}

	void RenderHelper::DrawVectorFont( Canvas* CV, const Vector2& BasePointSCR, eVectorFontType VFT, float Scale )
	{
		switch ( VFT )
		{
		case XFont:
			{
				// X
				CV->DrawLine2D( BasePointSCR + Vector2( -Scale, -Scale ), BasePointSCR + Vector2( Scale, Scale ), Color::cYellow );
				CV->DrawLine2D( BasePointSCR + Vector2( Scale, -Scale ), BasePointSCR + Vector2( -Scale, Scale ), Color::cYellow );

			}
			break;
		case YFont:
			{
				// Y
				CV->DrawLine2D( BasePointSCR + Vector2( -Scale, -Scale ), BasePointSCR , Color::cYellow );
				CV->DrawLine2D( BasePointSCR + Vector2( Scale, -Scale ), BasePointSCR , Color::cYellow );
				CV->DrawLine2D( BasePointSCR + Vector2( 0.0f, Scale ), BasePointSCR , Color::cYellow );

			}
			break;
		case ZFont:
			{
				// Z
				CV->DrawLine2D( BasePointSCR + Vector2( -Scale, -Scale ), BasePointSCR + Vector2( Scale, -Scale ), Color::cYellow );
				CV->DrawLine2D( BasePointSCR + Vector2( Scale, -Scale ), BasePointSCR + Vector2( -Scale, Scale ), Color::cYellow );
				CV->DrawLine2D( BasePointSCR + Vector2( -Scale, Scale ), BasePointSCR + Vector2( Scale, Scale ), Color::cYellow );

			}
			break;
		}



	}


	Vector2 MapAxisPoint( const Vector3& Pt, const Matrix4& Mat, const Viewport& VP, float XRatio, float YRatio )
	{
		Vector4 a(Pt, 1.0f);

		Vector4 b =  a * Mat;

		Vector4 c = b / b.w;

		Vector2 out;

		// 屏幕左上角坐标叠加一个修正坐标
		out.x = VP.X + VP.Width  * c.x + XRatio * VP.Width ;
		out.y = VP.Y - VP.Height *  c.y + VP.Height * (1 - YRatio );


		return out;
	}

	void RenderHelper::DrawAxisDirection( Camera* Cam, Canvas* CV, const Viewport& VP, float XRatio, float YRatio, float AxisScale )
	{
		// 用View矢量和零点构建View矩阵,再叠加投射矩阵
		Matrix4 Mat =  Matrix4::LookAtLH(	Cam->GetViewDir().Normalize() * AxisScale, 
											Vector3::cZero, 
											Vector3::cZAxis ) 
									* Cam->GetProjectMatrix();



		// 手动将4个点映射到屏幕坐标
		Vector2 XPoint = MapAxisPoint( Vector3::cXAxis, Mat, VP, XRatio, YRatio );
		Vector2 YPoint = MapAxisPoint( Vector3::cYAxis, Mat, VP, XRatio, YRatio );
		Vector2 ZPoint = MapAxisPoint( Vector3::cZAxis, Mat, VP, XRatio, YRatio );
		Vector2 ZeroPoint = MapAxisPoint( Vector3::cZero, Mat, VP, XRatio, YRatio );

		// 绘制3个轴
		CV->DrawLine2D(ZeroPoint,YPoint, Color::cGreen );
		CV->DrawLine2D(ZeroPoint,ZPoint, Color::cBlue );
		CV->DrawLine2D(ZeroPoint,XPoint, Color::cRed );

		const float Scale = 3.0f;

		// 绘制轴上文字
		DrawVectorFont( CV, XPoint, XFont, Scale );
		DrawVectorFont( CV, YPoint, YFont, Scale );
		DrawVectorFont( CV, ZPoint, ZFont, Scale );
	}

	
}