#include "EnginePCH.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS( ColorBrushPixelShader );

	void ColorBrushPixelShader::BindShaderParameter( eShaderType ShaderType, const ShaderParameterMap& ParameterMap )
	{
		mFillColor.Bind( ParameterMap, "FillColor" );
	}

	void ColorBrushPixelShader::SetColor( const Color& C )
	{
		if ( mShaderRHI.Valid() )
			mShaderRHI->SetContantParameter( mFillColor, C );
	}

	void ColorBrushPixelShader::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		Ser << mFillColor;
	}

}