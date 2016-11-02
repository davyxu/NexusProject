#pragma once


namespace NekoEngine	
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/


	/** 
		颜色类
	*/
	class Color
	{
	public:
		float r,g,b,a;

	public:
		/** 
			构造函数
			@remark
				初始化为白色
		*/
		Color( );

		/** 
			构造函数
			@remark
				通过一个ARGB的dword值初始化
		*/
		Color(dword color);

		/** 
			构造函数
			@remark
				通过颜色分量初始化
		*/
		Color(float alpha, float red, float green, float blue);
		
		/** 
			dword值设置
			@param
				color ARGB格式dword颜色
		*/
		dword operator = (const dword color);

		/** 
			赋值操作符
			@return
				本对象引用
		*/
		Color& operator = (const Color& color);

		/** 
			判断颜色相等
			@return
				颜色是否相等
		*/
		bool operator == (const Color& color);

		/** 
			判断颜色不等
			@return
				颜色是否不等
		*/
		bool operator != (const Color& color);

		/** 
			反色
			@return
				已反色后的颜色对象
		*/
		Color Inverse( );

		/** 
			dword操作符取色
			@return
				ARGB颜色
		*/
		operator dword( ) const;

		/** 
			获得颜色的dword格式颜色
			@return
				ARGB颜色
		*/
		dword ToDWORD( ) const ;

		/** 通过dword颜色设置颜色
			@param
				color ARGB颜色
			@return
				本对象引用
		*/
		Color& SetColor(dword color);

		/**	
			设置4个分量值
			@param
				alpha 透明度,范围(0~1)
			@param
				red 红,范围(0~1)
			@param
				green 绿,范围(0~1)
			@param
				blue 蓝,范围(0~1)
			@return
				本对象引用
		*/
		Color& SetColor(float alpha, float red, float green, float blue);

		/**	
			设置4个分量值
			@param
				alpha 透明度,范围(0~255)
			@param
				red 红,范围(0~255)
			@param
				green 绿,范围(0~255)
			@param
				blue 蓝,范围(0~255)
			@return
				本对象引用
		*/
		Color& SetColor255(_byte alpha,_byte red,_byte green,_byte blue);

		/** 
			dword设置颜色
			@param
				color ABGR格式颜色
			@return
				本对象引用
		*/
		Color& SetColorABGR(dword color);

		/** 
			4分量转为ARGB格式
			@param
				alpha 透明度,范围(0~1)
			@param
				red 红,范围(0~1)
			@param
				green 绿,范围(0~1)
			@param
				blue 蓝,范围(0~1)
			@return
				ARGB格式颜色
		*/
		static dword ToDWORD(float alpha, float red, float green, float blue);

		/** 
			颜色插值
			@param
				color1 颜色1
			@param
				color2 颜色2
			@param
				s 插值范围(0~1)
			@return
				插值后的颜色
		*/
		static Color Lerp(const Color& color1,const Color& color2,float s);

		/** 
			从HSB/HSV 转换到RGB空间
			@param
				Hue 色调(0~1)
			@param
				Saturation 饱和度(0~1) 
			@param
				Brightness 亮度(0~1)
			@return
				本对象引用
		*/
		Color& SetHSB(float Hue, float Saturation, float Brightness);

		/** 
			从RGB转换为HSB/HSV空间
			@param
				Hue 色调(0~1)
			@param
				Saturation 饱和度(0~1) 
			@param
				Brightness 亮度(0~1)
		*/
		void GetHSB(float* Hue, float* Saturation, float* Brightness) const;

		/** 
			RGB32转到灰度
			@param
				R 红色(0~255)
			@param
				G 绿色(0~255)
			@param
				B 蓝色(0~255)
			@return
				灰度色(0~255)
		*/
		static _byte RGB32ToGray( _byte R, _byte G, _byte B );

	public:
		/// 黑色
		static const Color cBlack;

		/// 红色
		static const Color cRed;

		/// 绿色
		static const Color cGreen;

		/// 蓝色
		static const Color cBlue;

		/// 黄色
		static const Color cYellow;

		/// 白色
		static const Color cWhite;
	};

	

}
