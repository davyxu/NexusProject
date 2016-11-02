#include "D3D9PCH.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// D3D9VertexShader
	//////////////////////////////////////////////////////////////////////////

	D3D9VertexShader::D3D9VertexShader( eShaderType ShaderType, _byte* Code, dword Size ) : mVertexShader( null )
	{
		mShaderType = ShaderType;

		mCodeBuffer.resize( Size );
		::memcpy(&mCodeBuffer[0], Code, Size );
	}

	D3D9VertexShader::~D3D9VertexShader()
	{
		SafeRelease( mVertexShader );
	}

	void D3D9VertexShader::Commit()
	{
		Assert( IsInRenderingThread() );

		if ( mVertexShader == null && mCodeBuffer.size() > 0 )
		{
			VD3DHR(GDirect3DDevice->CreateVertexShader((DWORD*)&mCodeBuffer[0], &mVertexShader ));
		}

		if ( mVertexShader != null )
		{
			VD3DHR(GDirect3DDevice->SetVertexShader( mVertexShader ));
		}
		else
		{
			VD3DHR(GDirect3DDevice->SetVertexShader( null ));
		}
	}

	void D3D9VertexShader::SetContantParameter( const ShaderParameter& Parameter, const float* NewValue, dword NumVector4 )
	{
		Assert( IsInRenderingThread() );

		if ( !Parameter.IsBound() )
			return;

		VD3DHR( GDirect3DDevice->SetVertexShaderConstantF( Parameter.GetBaseRegisterIndex(), NewValue, NumVector4 ) );
	}

	void D3D9VertexShader::SetTextureParameter( const ShaderParameter& Parameter, RHISamplerState* NewSamplerState, RHITexture* NewTexture )
	{
		Assert( "Vertex Shader Don't Have This Function" == 0 );
	}

	//////////////////////////////////////////////////////////////////////////
	// D3D9PixelShader
	//////////////////////////////////////////////////////////////////////////

	D3D9PixelShader::D3D9PixelShader( eShaderType ShaderType, const _byte* Code, dword Size ) : mPixelShader( null )
	{
		mShaderType = ShaderType;

		mCodeBuffer.resize( Size );
		::memcpy(&mCodeBuffer[0], Code, Size );
	}

	D3D9PixelShader::~D3D9PixelShader()
	{
		SafeRelease( mPixelShader );
	}

	void D3D9PixelShader::Commit()
	{
		Assert( IsInRenderingThread() );

		if ( mPixelShader == null && mCodeBuffer.size() > 0 )			
		{
			VD3DHR(GDirect3DDevice->CreatePixelShader((DWORD*)&mCodeBuffer[0], &mPixelShader ));
		}

		if ( mPixelShader != null )
		{
			VD3DHR(GDirect3DDevice->SetPixelShader( mPixelShader ));
		}
		else
		{
			VD3DHR(GDirect3DDevice->SetPixelShader( null ));
		}
	}

	void D3D9PixelShader::SetContantParameter( const ShaderParameter& Parameter, const float* NewValue, dword NumVector4 )
	{
		Assert( IsInRenderingThread() );

		if ( !Parameter.IsBound() )
			return;

		VD3DHR( GDirect3DDevice->SetPixelShaderConstantF( Parameter.GetBaseRegisterIndex(), NewValue, NumVector4 ) );
	}

	void D3D9PixelShader::SetTextureParameter( const ShaderParameter& Parameter, RHISamplerState* NewSamplerState, RHITexture* NewTexture )
	{
		if ( !Parameter.IsBound() )
			return;		
			

		if ( NewTexture != null )
		{
			if ( NewSamplerState != null )
				NewSamplerState->Commit( Parameter.GetBaseRegisterIndex() );

			NewTexture->Commit( Parameter.GetBaseRegisterIndex() );
		}
		else
		{
			GCheckerTexture->Commit( Parameter.GetBaseRegisterIndex() );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// D3D9ShaderFinder
	//////////////////////////////////////////////////////////////////////////

	D3D9ShaderFinder::D3D9ShaderFinder( eShaderType ShaderType, _byte* Code, dword Size )
		: mCode( Code )
		, mSize( Size )
		, mType( ShaderType )
	{
		

		mHashCode = CheckSum::MemCRC( Code, Size );
	}


	D3D9ShaderSlot::D3D9ShaderSlot()
		: mShader( null )
		, mHashCode( 0 )
	{

	}

	D3D9ShaderSlot::D3D9ShaderSlot( RHIShader* Shader )
		: mShader( Shader )
	{
		mHashCode = CheckSum::MemCRC( GetCodePointer(), GetCodeSize() );

	}

	NekoEngine::dword D3D9ShaderSlot::GetCodeSize() const
	{

		return mShader->GetCodeSize();
	}




}