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
		HitProxy”√Shader
	*/
	class ColorBrushPixelShader : public RenderShader
	{
		DECLARE_RTTI_CLASS( ColorBrushPixelShader, RenderShader, ClassFlag_Intrinsic| ClassFlag_Serializable )
		DECLARE_SHADER_METAINFO(ST_PixelShader, L"EngineShader.ColorBrushPixelShader.SF",L"Main")

	public:

		virtual void BindShaderParameter( eShaderType ShaderType, const ShaderParameterMap& ParameterMap );

		void SetColor( const Color& C );

		virtual void Serialize( BinarySerializer& Ser );

	private:
		ShaderParameter mFillColor;
	};

	/** @} */
	/** @} */
}
