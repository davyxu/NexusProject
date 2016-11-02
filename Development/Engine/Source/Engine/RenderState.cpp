#include "EnginePCH.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{
#define IMPLEMENT_STATEOBJECT( TClass ) IMPLEMENT_RTTIOBJECT_STRING( TClass, #TClass, "RenderStateObject" )

	//////////////////////////////////////////////////////////////////////////
	// RasterizerState
	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_STATEOBJECT( RHIRasterizerState )
		RHIRasterizerState::RHIRasterizerState() 
		: FillMode( FM_Fill)
		, CullMode( CM_Back )
		, FrontFaceCCW( false )
		, DepthBias( 0.0f )
		, SlopeScaledDepthBias( 0.0f )
		, DepthClipEnable( true )
		, ScissorEnable( false )
		, MultisampleEnable( false )
	{

	}

	//////////////////////////////////////////////////////////////////////////
	// DepthStencilState
	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_STATEOBJECT( DepthStencilState )
		DepthStencilState::DepthStencilState() 
		: DepthEnable( true )
		, DepthWriteMask( true )
		, DepthFunc( CF_Less )
		, StencilReadMask( 0xFFFF )
		, StencilWriteMask( 0xFFFF )
	{

	}

	DepthStencilState::DepthStencilStateOp::DepthStencilStateOp() 
		: StencilFailOp( SOP_Keep )
		, StencilDepthFailOp( SOP_Keep )
		, StencilPassOp( SOP_Keep )
		, StencilFunc( CF_AlwaysPass )
		, StencilEnable( false )
	{

	}

	//////////////////////////////////////////////////////////////////////////
	// BlendState
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_STATEOBJECT( RHIBlendState )
		RHIBlendState::RHIBlendState() 
		: AlphaToCoverageEnable( false )
		, BlendEnable( false )
		, SrcBlend( BF_One )
		, DestBlend( BF_Zero )
		, BlendOp( BOP_Add )
		, SrcBlendAlpha( BF_One )
		, DestBlendAlpha( BF_Zero )
		, BlendOpAlpha( BOP_Add )
	{

	}

	//////////////////////////////////////////////////////////////////////////
	// SamplerState
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_STATEOBJECT( RHISamplerState )
		RHISamplerState::RHISamplerState() 
		: Filter( TFO_Min_Mag_Mip_Point )
		, AddressU(TAM_Wrap)
		, AddressV(TAM_Wrap)
		, AddressW(TAM_Wrap)
		, MipLODBias( 0.0f )
		, MaxAnisotropy( 16 )
		, ComparisonFunc( CF_AlwaysFail )
		, BorderColor( 0 )
		, MinLOD( 0.0f )
		, MaxLOD( Math::cMaxFloat )
	{

	}




	IMPLEMENT_ENUMOBJECT_BEGIN( eFillMode, 1, 3 )
		ENUMOBJECT_ADD( FM_Point )
		ENUMOBJECT_ADD( FM_Line )
		ENUMOBJECT_ADD( FM_Fill )
	IMPLEMENT_ENUMOBJECT_END

	IMPLEMENT_ENUMOBJECT_BEGIN( eCullMode, 1, 3 )
		ENUMOBJECT_ADD( CM_None )
		ENUMOBJECT_ADD( CM_Front )
		ENUMOBJECT_ADD( CM_Back )
	IMPLEMENT_ENUMOBJECT_END

	IMPLEMENT_ENUMOBJECT_BEGIN( eBlendOperation, 1, 4 )
		ENUMOBJECT_ADD( BOP_Add )
		ENUMOBJECT_ADD( BOP_Sub )
		ENUMOBJECT_ADD( BOP_RevSub )
		ENUMOBJECT_ADD( BOP_Min )
		ENUMOBJECT_ADD( BOP_Max )
	IMPLEMENT_ENUMOBJECT_END

	IMPLEMENT_ENUMOBJECT_BEGIN( eBlendFactor, 1, 3 )
		ENUMOBJECT_ADD( BF_Zero )
		ENUMOBJECT_ADD( BF_One )
		ENUMOBJECT_ADD( BF_SrcAlpha )
		ENUMOBJECT_ADD( BF_DestAlpha )
		ENUMOBJECT_ADD( BF_InvSrcAlpha )
		ENUMOBJECT_ADD( BF_InvDestAlpha)
		ENUMOBJECT_ADD( BF_SrcColor)
		ENUMOBJECT_ADD( BF_DestColor)
		ENUMOBJECT_ADD( BF_InvSrcColor)
		ENUMOBJECT_ADD( BF_InvDestColor)
		ENUMOBJECT_ADD( BF_SrcAlphaSat)
	IMPLEMENT_ENUMOBJECT_END

	IMPLEMENT_ENUMOBJECT_BEGIN( eCompareFunction, 1, 3 )
		ENUMOBJECT_ADD( CF_AlwaysFail )
		ENUMOBJECT_ADD( CF_AlwaysPass )
		ENUMOBJECT_ADD( CF_Less )
		ENUMOBJECT_ADD( CF_LessEqual )
		ENUMOBJECT_ADD( CF_Equal )
		ENUMOBJECT_ADD( CF_NotEqual )
		ENUMOBJECT_ADD( CF_GreaterEqual )
		ENUMOBJECT_ADD( CF_Greater )
	IMPLEMENT_ENUMOBJECT_END

	IMPLEMENT_ENUMOBJECT_BEGIN( eStencilOperation, 1, 4 )
		ENUMOBJECT_ADD( SOP_Keep )
		ENUMOBJECT_ADD( SOP_Zero )
		ENUMOBJECT_ADD( SOP_Replace )		
		ENUMOBJECT_ADD( SOP_Increment )
		ENUMOBJECT_ADD( SOP_Decrement )
		ENUMOBJECT_ADD( SOP_Invert )
	IMPLEMENT_ENUMOBJECT_END

	IMPLEMENT_ENUMOBJECT_BEGIN( eTexAddressingMode, 1, 4 )
		ENUMOBJECT_ADD( TAM_Wrap )
		ENUMOBJECT_ADD( TAM_Mirror )
		ENUMOBJECT_ADD( TAM_Clamp )
		ENUMOBJECT_ADD( TAM_Border )
	IMPLEMENT_ENUMOBJECT_END

	IMPLEMENT_ENUMOBJECT_BEGIN( eTexFilterOp, 1, 4 )
		ENUMOBJECT_ADD( TFO_Min_Mag_Mip_Point )	
		ENUMOBJECT_ADD( TFO_Min_Mag_Point_Mip_Linear )
		ENUMOBJECT_ADD( TFO_Min_Point_Mag_Linear_Mip_Point )
		ENUMOBJECT_ADD( TFO_Min_Point_Mag_Mip_Linear )
		ENUMOBJECT_ADD( TFO_Min_Linear_Mag_Mip_Point )		
		ENUMOBJECT_ADD( TFO_Min_Linear_Mag_Point_Mip_Linear	)
		ENUMOBJECT_ADD( TFO_Min_Mag_Linear_Mip_Point )
		ENUMOBJECT_ADD( TFO_Min_Mag_Mip_Linear )				
	IMPLEMENT_ENUMOBJECT_END


	void RenderStateObject::BuildReflection( SettingObject& so )
	{
		BIND_SETTINGOBJECT_BEGIN( RHIRasterizerState )
			BIND_SO_ENUM	( eFillMode	, FillMode )
			BIND_SO_ENUM	( eCullMode	, CullMode )
			BIND_SO_MEMBER	( bool		, FrontFaceCCW )
			BIND_SO_MEMBER	( float		, DepthBias )
			BIND_SO_MEMBER	( float		, SlopeScaledDepthBias)
			BIND_SO_MEMBER	( bool		, DepthClipEnable)
			BIND_SO_MEMBER	( bool		, ScissorEnable)
			BIND_SO_MEMBER	( bool		, MultisampleEnable)
		BIND_SETTINGOBJECT_END
	

		BIND_SETTINGOBJECT_BEGIN( DepthStencilState )
			BIND_SO_MEMBER	( bool		,					DepthEnable )
			BIND_SO_MEMBER	( float		,					DepthWriteMask )
			BIND_SO_ENUM	( eCompareFunction	,			DepthFunc )
			BIND_SO_MEMBER	( dword		,					StencilReadMask)
			BIND_SO_MEMBER	( dword		,					StencilWriteMask)

			BIND_SO_MEMBER_NAME	( bool					,	FrontFace.StencilEnable,		"FrontFace.StencilEnable" )
			BIND_SO_ENUM_NAME	( eStencilOperation		,	FrontFace.StencilFailOp,		"FrontFace.StencilFailOp" )
			BIND_SO_ENUM_NAME	( eStencilOperation		,	FrontFace.StencilDepthFailOp,	"FrontFace.StencilDepthFailOp" )
			BIND_SO_ENUM_NAME	( eStencilOperation		,	FrontFace.StencilPassOp,		"FrontFace.StencilPassOp" )
			BIND_SO_ENUM_NAME	( eCompareFunction		,	FrontFace.StencilFunc,			"FrontFace.StencilFunc" )

			BIND_SO_MEMBER_NAME	( bool					,	BackFace.StencilEnable,			"BackFace.StencilEnable" )
			BIND_SO_ENUM_NAME	( eStencilOperation		,	BackFace.StencilFailOp,			"BackFace.StencilFailOp" )
			BIND_SO_ENUM_NAME	( eStencilOperation		,	BackFace.StencilDepthFailOp,	"BackFace.StencilDepthFailOp" )
			BIND_SO_ENUM_NAME	( eStencilOperation		,	BackFace.StencilPassOp,			"BackFace.StencilPassOp" )
			BIND_SO_ENUM_NAME	( eCompareFunction		,	BackFace.StencilFunc,			"BackFace.StencilFunc" )
		BIND_SETTINGOBJECT_END


		BIND_SETTINGOBJECT_BEGIN( RHIBlendState )
			BIND_SO_MEMBER	( bool				, AlphaToCoverageEnable)
			BIND_SO_MEMBER	( bool				, BlendEnable)
			BIND_SO_ENUM	( eBlendFactor		, SrcBlend )
			BIND_SO_ENUM	( eBlendFactor		, DestBlend )
			BIND_SO_ENUM	( eBlendOperation	, BlendOp )
			BIND_SO_ENUM	( eBlendFactor		, SrcBlendAlpha )
			BIND_SO_ENUM	( eBlendFactor		, DestBlendAlpha )
			BIND_SO_ENUM	( eBlendOperation	, BlendOpAlpha )
		BIND_SETTINGOBJECT_END

		BIND_SETTINGOBJECT_BEGIN( RHISamplerState )
			BIND_SO_ENUM	( eTexFilterOp			, Filter )
			BIND_SO_ENUM	( eTexAddressingMode	, AddressU )
			BIND_SO_ENUM	( eTexAddressingMode	, AddressV )
			BIND_SO_ENUM	( eTexAddressingMode	, AddressW )
			BIND_SO_MEMBER	( float					, MipLODBias)
			BIND_SO_MEMBER	( dword					, MaxAnisotropy)
			BIND_SO_ENUM	( eCompareFunction		, ComparisonFunc )
			BIND_SO_MEMBER	( Color					, BorderColor)
			BIND_SO_MEMBER	( float					, MinLOD)
			BIND_SO_MEMBER	( float					, MaxLOD)
		BIND_SETTINGOBJECT_END

	}


	void RenderStateObject::StaticInitRenderStateObject( )
	{
		NameRef::AddNameConst( "RenderStateObject", RenderStateNameConst_RenderStateObject );
		NameRef::AddNameConst( "RasterizerState",	RenderStateNameConst_RasterizerState );
		NameRef::AddNameConst( "DepthStencilState", RenderStateNameConst_DepthStencilState );
		NameRef::AddNameConst( "BlendState",		RenderStateNameConst_BlendState );
		NameRef::AddNameConst( "SamplerState",		RenderStateNameConst_SamplerState );

		RHIRasterizerState::StaticInit();
		DepthStencilState::StaticInit();
		RHIBlendState::StaticInit();
		RHISamplerState::StaticInit();

		ENUMOBJECT_STATICINIT( eFillMode )
		ENUMOBJECT_STATICINIT( eCullMode )
		ENUMOBJECT_STATICINIT( eBlendOperation )
		ENUMOBJECT_STATICINIT( eBlendFactor )
		ENUMOBJECT_STATICINIT( eCompareFunction )
		ENUMOBJECT_STATICINIT( eStencilOperation )
		ENUMOBJECT_STATICINIT( eTexAddressingMode )
		ENUMOBJECT_STATICINIT( eTexFilterOp )
	}

	void RenderStateObject::StaticExitRenderStateObject( )
	{
		RTTIObject::UnRegisterGroup( "RenderStateObject" );
	}


}

