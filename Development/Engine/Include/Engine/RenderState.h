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
		采样过滤器
	*/
	enum eSamplerFilter
	{
		SF_Nearest,				///< 最近点采样
		SF_Linear,				///< 线性采样
		SF_AnisotropicLinear	///< 各向异性采样
	};

	/** 
		采样器寻址方式
	*/
	enum eSamplerAddressMode
	{
		AM_Wrap,				///< 封装
		AM_Clamp,				///< 裁剪
		AM_Mirror				///< 镜像
	};

	/** 
		光栅填充模式
	*/
	enum eRasterizerFillMode
	{
		FM_Point,				///< 点显示
		FM_Wireframe,			///< 线框显示
		FM_Solid				///< 实体显示
	};

	/** 
		光栅裁剪方式
	*/
	enum eRasterizerCullMode
	{
		CM_None,				///< 无裁剪
		CM_CW,					///< 顺时针裁剪
		CM_CCW					///< 逆时针裁剪
	};

	/** 
		颜色写入蒙版
	*/
	enum eColorWriteMask
	{
		CW_RED		= 0x01,
		CW_GREEN	= 0x02,
		CW_BLUE		= 0x04,
		CW_ALPHA	= 0x08,

		CW_RGB		= 0x07,
		CW_RGBA		= 0x0f,
	};

	/** 
		比较函数
	*/
	enum eCompareFunction
	{
		CF_Less,			///< 小于
		CF_LessEqual,		///< 小于等于
		CF_Greater,			///< 大于
		CF_GreaterEqual,	///< 大于等于
		CF_Equal,			///< 等于
		CF_NotEqual,		///< 不等
		CF_Never,			///< 绝不
		CF_Always			///< 始终
	};

	/** 
		模板操作
	*/
	enum eStencilOp
	{
		SO_Keep,
		SO_Zero,
		SO_Replace,
		SO_SaturatedIncrement,
		SO_SaturatedDecrement,
		SO_Invert,
		SO_Increment,
		SO_Decrement
	};

	/** 
		混合操作
	*/
	enum eBlendOperation
	{
		BO_Add,
		BO_Subtract,
		BO_Min,
		BO_Max
	};

	/** 
		混合因子
	*/
	enum eBlendFactor
	{
		BF_Zero,
		BF_One,
		BF_SourceColor,
		BF_InverseSourceColor,
		BF_SourceAlpha,
		BF_InverseSourceAlpha,
		BF_DestAlpha,
		BF_InverseDestAlpha,
		BF_DestColor,
		BF_InverseDestColor
	};





	struct SamplerStateInitializer
	{
		eSamplerFilter		Filter;
		eSamplerAddressMode AddressU;
		eSamplerAddressMode AddressV;
		eSamplerAddressMode AddressW;
	};
	struct RasterizerStateInitializer
	{
		eRasterizerFillMode FillMode;
		eRasterizerCullMode CullMode;
		float				DepthBias;
		float				SlopeScaleDepthBias;
	};
	struct DepthStateInitializer
	{
		bool				EnableDepthWrite;
		eCompareFunction	DepthTest;
	};

	struct StencilStateInitializer
	{
		bool				EnableFrontFaceStencil;
		eCompareFunction	FrontFaceStencilTest;
		eStencilOp			FrontFaceStencilFailStencilOp;
		eStencilOp			FrontFaceDepthFailStencilOp;
		eStencilOp			FrontFacePassStencilOp;
		bool				EnableBackFaceStencil;
		eCompareFunction	BackFaceStencilTest;
		eStencilOp			BackFaceStencilFailStencilOp;
		eStencilOp			BackFaceDepthFailStencilOp;
		eStencilOp			BackFacePassStencilOp;
		dword				StencilReadMask;
		dword				StencilWriteMask;
		dword				StencilRef;
	};
	struct BlendStateInitializer
	{
		eBlendOperation		ColorBlendOperation;
		eBlendFactor		ColorSourceBlendFactor;
		eBlendFactor		ColorDestBlendFactor;
		eBlendOperation		AlphaBlendOperation;
		eBlendFactor		AlphaSourceBlendFactor;
		eBlendFactor		AlphaDestBlendFactor;
		eCompareFunction	AlphaTest;
		_byte				AlphaRef;
	};



	/** @} */
	/** @} */

}