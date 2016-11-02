#include "D3D9PCH.h"


namespace NekoEngine
{

	D3DTEXTUREADDRESS TranslateAddressMode(eSamplerAddressMode AddressMode)
	{
		switch(AddressMode)
		{
		case AM_Clamp: return D3DTADDRESS_CLAMP;
		case AM_Mirror: return D3DTADDRESS_MIRROR;
		default: return D3DTADDRESS_WRAP;
		};
	}

	D3DCULL TranslateCullMode(eRasterizerCullMode CullMode)
	{
		switch(CullMode)
		{
		case CM_CW: return D3DCULL_CW;
		case CM_CCW: return D3DCULL_CCW;
		default: return D3DCULL_NONE;
		};
	}

	D3DFILLMODE TranslateFillMode(eRasterizerFillMode FillMode)
	{
		switch(FillMode)
		{
		case FM_Point: return D3DFILL_POINT;
		case FM_Wireframe: return D3DFILL_WIREFRAME;
		default: return D3DFILL_SOLID;
		};
	}

	D3DCMPFUNC TranslateCompareFunction(eCompareFunction CompareFunction)
	{
		switch(CompareFunction)
		{
		case CF_Less: return D3DCMP_LESS;
		case CF_LessEqual: return D3DCMP_LESSEQUAL;
		case CF_Greater: return D3DCMP_GREATER;
		case CF_GreaterEqual: return D3DCMP_GREATEREQUAL;
		case CF_Equal: return D3DCMP_EQUAL;
		case CF_NotEqual: return D3DCMP_NOTEQUAL;
		case CF_Never: return D3DCMP_NEVER;
		default: return D3DCMP_ALWAYS;
		};
	}

	D3DSTENCILOP TranslateStencilOp(eStencilOp StencilOp)
	{
		switch(StencilOp)
		{
		case SO_Zero: return D3DSTENCILOP_ZERO;
		case SO_Replace: return D3DSTENCILOP_REPLACE;
		case SO_SaturatedIncrement: return D3DSTENCILOP_INCRSAT;
		case SO_SaturatedDecrement: return D3DSTENCILOP_DECRSAT;
		case SO_Invert: return D3DSTENCILOP_INVERT;
		case SO_Increment: return D3DSTENCILOP_INCR;
		case SO_Decrement: return D3DSTENCILOP_DECR;
		default: return D3DSTENCILOP_KEEP;
		};
	}

	D3DBLENDOP TranslateBlendOp(eBlendOperation BlendOp)
	{
		switch(BlendOp)
		{
		case BO_Subtract: return D3DBLENDOP_SUBTRACT;
		case BO_Min: return D3DBLENDOP_MIN;
		case BO_Max: return D3DBLENDOP_MAX;
		default: return D3DBLENDOP_ADD;
		};
	}

	D3DBLEND TranslateBlendFactor(eBlendFactor BlendFactor)
	{
		switch(BlendFactor)
		{
		case BF_One: return D3DBLEND_ONE;
		case BF_SourceColor: return D3DBLEND_SRCCOLOR;
		case BF_InverseSourceColor: return D3DBLEND_INVSRCCOLOR;
		case BF_SourceAlpha: return D3DBLEND_SRCALPHA;
		case BF_InverseSourceAlpha: return D3DBLEND_INVSRCALPHA;
		case BF_DestAlpha: return D3DBLEND_DESTALPHA;
		case BF_InverseDestAlpha: return D3DBLEND_INVDESTALPHA;
		case BF_DestColor: return D3DBLEND_DESTCOLOR;
		case BF_InverseDestColor: return D3DBLEND_INVDESTCOLOR;
		default: return D3DBLEND_ZERO;
		};
	}


	RHISamplerStateRef D3D9RHI::CreateSamplerState(const SamplerStateInitializer& Initializer)
	{
		D3D9SamplerState* SamplerState = new D3D9SamplerState;
		SamplerState->AddressU = TranslateAddressMode(Initializer.AddressU);
		SamplerState->AddressV = TranslateAddressMode(Initializer.AddressV);
		SamplerState->AddressW = TranslateAddressMode(Initializer.AddressW);
		switch(Initializer.Filter)
		{
		case SF_AnisotropicLinear:
			SamplerState->MinFilter = D3DTEXF_ANISOTROPIC;
			SamplerState->MagFilter	= D3DTEXF_LINEAR;
			SamplerState->MipFilter	= D3DTEXF_LINEAR;
			break;
		case SF_Linear:
			SamplerState->MinFilter = D3DTEXF_LINEAR;
			SamplerState->MagFilter	= D3DTEXF_LINEAR;
			SamplerState->MipFilter	= D3DTEXF_LINEAR;
			break;
		case SF_Nearest:
			SamplerState->MinFilter = D3DTEXF_POINT;
			SamplerState->MagFilter	= D3DTEXF_POINT;
			SamplerState->MipFilter	= D3DTEXF_POINT;
			break;
		}
		return SamplerState;
	}

	RHIRasterizerStateRef D3D9RHI::CreateRasterizerState(const RasterizerStateInitializer& Initializer)
	{
		D3D9RasterizerState* RasterizerState = new D3D9RasterizerState;
		RasterizerState->CullMode = TranslateCullMode(Initializer.CullMode);
		RasterizerState->FillMode = TranslateFillMode(Initializer.FillMode);
		RasterizerState->DepthBias = Initializer.DepthBias;
		RasterizerState->SlopeScaleDepthBias = Initializer.SlopeScaleDepthBias;
		return RasterizerState;
	}


	RHIDepthStateRef D3D9RHI::CreateDepthState(const DepthStateInitializer& Initializer)
	{
		D3D9DepthState* DepthState = new D3D9DepthState;
		DepthState->bZEnable = Initializer.DepthTest != CF_Always || Initializer.EnableDepthWrite;
		DepthState->bZWriteEnable = Initializer.EnableDepthWrite;
		DepthState->ZFunc = TranslateCompareFunction(Initializer.DepthTest);
		return DepthState;
	}



	RHIStencilStateRef D3D9RHI::CreateStencilState(const StencilStateInitializer& Initializer)
	{
		D3D9StencilState* StencilState = new D3D9StencilState;
		StencilState->bStencilEnable			= Initializer.EnableFrontFaceStencil || Initializer.EnableBackFaceStencil;
		StencilState->bTwoSidedStencilMode		= Initializer.EnableBackFaceStencil;
		StencilState->StencilFunc				= TranslateCompareFunction(Initializer.FrontFaceStencilTest);
		StencilState->StencilFail				= TranslateStencilOp(Initializer.FrontFaceStencilFailStencilOp);
		StencilState->StencilZFail				= TranslateStencilOp(Initializer.FrontFaceDepthFailStencilOp);
		StencilState->StencilPass				= TranslateStencilOp(Initializer.FrontFacePassStencilOp);
		StencilState->CCWStencilFunc			= TranslateCompareFunction(Initializer.BackFaceStencilTest);
		StencilState->CCWStencilFail			= TranslateStencilOp(Initializer.BackFaceStencilFailStencilOp);
		StencilState->CCWStencilZFail			= TranslateStencilOp(Initializer.BackFaceDepthFailStencilOp);
		StencilState->CCWStencilPass			= TranslateStencilOp(Initializer.BackFacePassStencilOp);
		StencilState->StencilReadMask			= Initializer.StencilReadMask;
		StencilState->StencilWriteMask			= Initializer.StencilWriteMask;
		StencilState->StencilRef				= Initializer.StencilRef;
		return StencilState;
	}

	RHIBlendStateRef D3D9RHI::CreateBlendState(const BlendStateInitializer& Initializer)
	{
		D3D9BlendState* BlendState = new D3D9BlendState;
		BlendState->bAlphaBlendEnable			= Initializer.ColorDestBlendFactor != BF_Zero || Initializer.ColorSourceBlendFactor != BF_One;
		BlendState->ColorBlendOperation			= TranslateBlendOp(Initializer.ColorBlendOperation);
		BlendState->ColorSourceBlendFactor		= TranslateBlendFactor(Initializer.ColorSourceBlendFactor);
		BlendState->ColorDestBlendFactor		= TranslateBlendFactor(Initializer.ColorDestBlendFactor);
		BlendState->bSeparateAlphaBlendEnable	= Initializer.AlphaDestBlendFactor != BF_Zero || Initializer.AlphaSourceBlendFactor != BF_One;
		BlendState->AlphaBlendOperation			= TranslateBlendOp(Initializer.AlphaBlendOperation);
		BlendState->AlphaSourceBlendFactor		= TranslateBlendFactor(Initializer.AlphaSourceBlendFactor);
		BlendState->AlphaDestBlendFactor		= TranslateBlendFactor(Initializer.AlphaDestBlendFactor);
		BlendState->bAlphaTestEnable			= Initializer.AlphaTest != CF_Always;
		BlendState->AlphaFunc					= TranslateCompareFunction(Initializer.AlphaTest);
		BlendState->AlphaRef					= Initializer.AlphaRef;
		return BlendState;
	}


	void D3D9SamplerState::Commit(dword SamplerIndex )
	{
		Assert( IsInRenderingThread() );

		//GDirect3DDevice->SetSamplerState(SamplerIndex,D3DSAMP_SRGBTEXTURE,NewTexture.IsSRGB());
		GDirect3DDevice->SetSamplerState(SamplerIndex,D3DSAMP_MAGFILTER,MagFilter);
		GDirect3DDevice->SetSamplerState(SamplerIndex,D3DSAMP_MINFILTER,MinFilter);
		GDirect3DDevice->SetSamplerState(SamplerIndex,D3DSAMP_MIPFILTER,MipFilter);
		GDirect3DDevice->SetSamplerState(SamplerIndex,D3DSAMP_ADDRESSU,AddressU);
		GDirect3DDevice->SetSamplerState(SamplerIndex,D3DSAMP_ADDRESSV,AddressV);
		GDirect3DDevice->SetSamplerState(SamplerIndex,D3DSAMP_ADDRESSW,AddressW);
	}

	void D3D9SamplerState::Commit()
	{
		Commit( 0 );
	}

	void D3D9RasterizerState::Commit()
	{
		Assert( IsInRenderingThread() );

		GDirect3DDevice->SetRenderState(D3DRS_FILLMODE,FillMode);
		GDirect3DDevice->SetRenderState(D3DRS_CULLMODE,CullMode);
		GDirect3DDevice->SetRenderState(D3DRS_DEPTHBIAS,FLOAT_TO_DWORD(DepthBias));
		GDirect3DDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS,FLOAT_TO_DWORD(SlopeScaleDepthBias));
	}

	void D3D9DepthState::Commit()
	{
		Assert( IsInRenderingThread() );

		GDirect3DDevice->SetRenderState(D3DRS_ZENABLE,bZEnable);
		GDirect3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,bZWriteEnable);
		GDirect3DDevice->SetRenderState(D3DRS_ZFUNC,ZFunc);
	}

	void D3D9StencilState::Commit()
	{
		Assert( IsInRenderingThread() );

		GDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE,bStencilEnable);
		GDirect3DDevice->SetRenderState(D3DRS_STENCILFUNC,StencilFunc);
		GDirect3DDevice->SetRenderState(D3DRS_STENCILFAIL,StencilFail);
		GDirect3DDevice->SetRenderState(D3DRS_STENCILZFAIL,StencilZFail);
		GDirect3DDevice->SetRenderState(D3DRS_STENCILPASS,StencilPass);
		GDirect3DDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE,bTwoSidedStencilMode);
		GDirect3DDevice->SetRenderState(D3DRS_CCW_STENCILFUNC,CCWStencilFunc);
		GDirect3DDevice->SetRenderState(D3DRS_CCW_STENCILFAIL,CCWStencilFail);
		GDirect3DDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL,CCWStencilZFail);
		GDirect3DDevice->SetRenderState(D3DRS_CCW_STENCILPASS,CCWStencilPass);
		GDirect3DDevice->SetRenderState(D3DRS_STENCILMASK,StencilReadMask);
		GDirect3DDevice->SetRenderState(D3DRS_STENCILWRITEMASK,StencilWriteMask);
		GDirect3DDevice->SetRenderState(D3DRS_STENCILREF,StencilRef);
	}

	void D3D9BlendState::Commit()
	{

		Assert( IsInRenderingThread() );

		bool& tAlphaBlend = bAlphaBlendEnable;
		D3DBLENDOP& tColorBlendOperation = ColorBlendOperation;
		D3DBLEND& tColorSourceBlendFactor = ColorSourceBlendFactor;
		D3DBLEND& tColorDestBlendFactor = ColorDestBlendFactor;
		D3DBLENDOP& tAlphaBlendOperation = AlphaBlendOperation;
		D3DBLEND& tAlphaSourceBlendFactor = AlphaSourceBlendFactor;
		D3DBLEND& tAlphaDestBlendFactor = AlphaDestBlendFactor;
		bool& tbAlphaTestEnable = bAlphaTestEnable;

		GDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,bAlphaBlendEnable);
		GDirect3DDevice->SetRenderState(D3DRS_BLENDOP,ColorBlendOperation);
		GDirect3DDevice->SetRenderState(D3DRS_SRCBLEND,ColorSourceBlendFactor);
		GDirect3DDevice->SetRenderState(D3DRS_DESTBLEND,ColorDestBlendFactor);
		GDirect3DDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE,bSeparateAlphaBlendEnable);
		GDirect3DDevice->SetRenderState(D3DRS_BLENDOPALPHA,AlphaBlendOperation);
		GDirect3DDevice->SetRenderState(D3DRS_SRCBLENDALPHA,AlphaSourceBlendFactor);
		GDirect3DDevice->SetRenderState(D3DRS_DESTBLENDALPHA,AlphaDestBlendFactor);
		GDirect3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,bAlphaTestEnable);
		GDirect3DDevice->SetRenderState(D3DRS_ALPHAFUNC,AlphaFunc);
		GDirect3DDevice->SetRenderState(D3DRS_ALPHAREF,AlphaRef);
	}
}
