#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Render
	*  @{
	*/


	/** 
		帧率计算器
	*/
	class FrameSpeedUnit
	{
	public:
		FrameSpeedUnit( );

		/** 
			更新
		*/
		void Tick( );

		/** 
			获得当前的帧率
		*/
		int GetFramePerSecond( );

	private:
		int				mDisplayNumber;
		int				mAccumulator;
		float			mFPSTimer;
	};

	/** 
		渲染帮手
	*/
	class RenderHelper
	{
	public:
		/** 
			绘制调试用网格
			@param
				Cam 摄像机
			@param
				VP 视口
			@param
				网格范围
			@param
				格子大小
		*/

		enum eVectorFontType
		{
			XFont,
			YFont,
			ZFont,
		};

		/** 
			绘制坐标轴及网格
			@param
				Cam 摄像机
			@param
				CV 画布,绘制用
			@param
				VP 视口
			@param
				Range 网格范围,3D单位
			@param
				Size 轴大小
		*/
		static void DrawAxisGrid( Camera* Cam, Canvas* CV, const Viewport& VP , float Range = 100.0f, float Size = 10.0f );

		/** 
			绘制世界轴向参考
			@param
				Cam 摄像机
			@param
				CV 画布,绘制用
			@param
				VP 视口
			@param
				XRatio 屏幕坐标偏移,相对左下角
			@param
				YRatio 屏幕坐标偏移,相对左下角
			@param
				AxisScale 轴缩放
		*/
		static void DrawAxisDirection( Camera* Cam, Canvas* CV, const Viewport& VP, float XRatio = 0.05f, float YRatio = 0.1f, float AxisScale = 30.0f );

		/** 
			绘制平面矢量XYZ轴字
			@param
				CV 画布,绘制用
			@param
				BasePointSCR 绘制基点坐标,屏幕坐标系
			@param
				VFT XYZ轴指示
			@param
				Scale 缩放像素单位
		*/
		static void DrawVectorFont( Canvas* CV, const Vector2& BasePointSCR, eVectorFontType VFT, float Scale );
	};

	/** @} */
	/** @} */
}
