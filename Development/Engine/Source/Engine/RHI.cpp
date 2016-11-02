#include "EnginePCH.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{



	void RHIShader::SetTextureParameter( const ShaderParameter& Parameter, RenderTexture* NewTexture )
	{
		if ( NewTexture == null )
		{
			SetTextureParameter( Parameter, GCheckerTexture->GetSamplerStateRHI(), GCheckerTexture->GetTextureRHI() );
		}
		else
		{
			SetTextureParameter( Parameter, NewTexture->GetSamplerStateRHI(), NewTexture->GetTextureRHI() );
		}
	}

	_byte* RHIShader::GetCodePointer()
	{
		return &mCodeBuffer[0];
	}

	NekoEngine::dword RHIShader::GetCodeSize()
	{
		return mCodeBuffer.size();
	}



}