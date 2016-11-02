#pragma once

namespace NekoEngine
{
	/**
		D3D9 Sampler State й╣ож
	*/
	class D3D9SamplerState : public RHISamplerState
	{
	public:
		virtual void Commit( dword SamplerIndex );

		virtual void Commit( );

		D3DTEXTUREFILTERTYPE MagFilter;
		D3DTEXTUREFILTERTYPE MinFilter;
		D3DTEXTUREFILTERTYPE MipFilter;
		D3DTEXTUREADDRESS AddressU;
		D3DTEXTUREADDRESS AddressV;
		D3DTEXTUREADDRESS AddressW;
	};


	class D3D9RasterizerState : public RHIRasterizerState
	{
	public:
		virtual void Commit( );

		D3DFILLMODE FillMode;
		D3DCULL CullMode;
		FLOAT DepthBias;
		FLOAT SlopeScaleDepthBias;
	};


	class D3D9DepthState : public RHIDepthState
	{
	public:
		virtual void Commit( );

		bool bZEnable;
		bool bZWriteEnable;
		D3DCMPFUNC ZFunc;
	};

	class D3D9StencilState : public RHIStencilState
	{
	public:
		virtual void Commit( );

		bool bStencilEnable;
		bool bTwoSidedStencilMode;
		D3DCMPFUNC StencilFunc;
		D3DSTENCILOP StencilFail;
		D3DSTENCILOP StencilZFail;
		D3DSTENCILOP StencilPass;
		D3DCMPFUNC CCWStencilFunc;
		D3DSTENCILOP CCWStencilFail;
		D3DSTENCILOP CCWStencilZFail;
		D3DSTENCILOP CCWStencilPass;
		DWORD StencilReadMask;
		DWORD StencilWriteMask;
		DWORD StencilRef;
	};


	class D3D9BlendState : public RHIBlendState
	{
	public:
		virtual void Commit( );

		bool bAlphaBlendEnable;
		D3DBLENDOP ColorBlendOperation;
		D3DBLEND ColorSourceBlendFactor;
		D3DBLEND ColorDestBlendFactor;
		bool bSeparateAlphaBlendEnable;
		D3DBLENDOP AlphaBlendOperation;
		D3DBLEND AlphaSourceBlendFactor;
		D3DBLEND AlphaDestBlendFactor;
		bool bAlphaTestEnable;
		D3DCMPFUNC AlphaFunc;
		DWORD AlphaRef;
	};

}