#pragma once

namespace NekoEngine
{
	class D3D9VertexShader : public RHIShader
	{
	public:
		D3D9VertexShader(  eShaderType ShaderType, _byte* Code, dword Size );

		virtual ~D3D9VertexShader( );

		virtual void Commit();

		virtual void SetContantParameter( const ShaderParameter& Parameter, const float* NewValue, dword NumVector4 );

		virtual void SetTextureParameter( const ShaderParameter& Parameter, RHISamplerState* NewSamplerState, RHITexture* NewTexture );

	private:
		IDirect3DVertexShader9* mVertexShader;
	};



	class D3D9PixelShader : public RHIShader
	{
	public:
		D3D9PixelShader( eShaderType ShaderType, const _byte* Code, dword Size );

		virtual ~D3D9PixelShader( );

		virtual void Commit();

		virtual void SetContantParameter( const ShaderParameter& Parameter, const float* NewValue, dword NumVector4 );


		virtual void SetTextureParameter( const ShaderParameter& Parameter, RHISamplerState* NewSamplerState, RHITexture* NewTexture );

	private:
		IDirect3DPixelShader9* mPixelShader;
	};


	class D3D9ShaderSlot
	{
	public:
		D3D9ShaderSlot( );

		D3D9ShaderSlot( RHIShader* Shader );

		virtual dword GetCodeSize( ) const;
		virtual _byte* GetCodePointer( )const { return mShader->GetCodePointer(); }
		virtual eShaderType GetType( )const { return mShader->GetType(); }

		bool operator == ( const D3D9ShaderSlot& Other )
		{
			if ( GetCodeSize() != Other.GetCodeSize() )
				return false;

			return memcmp( GetCodePointer(), Other.GetCodePointer(), GetCodeSize() ) == 0;
		}

		friend dword GetTypeHash( const D3D9ShaderSlot& Other )
		{
			return Other.mHashCode;
		}

		RHIShader* GetShader( ){ return mShader; }

	protected:
		RHIShader*	mShader;
		dword		mHashCode;
	};


	class D3D9ShaderFinder : public D3D9ShaderSlot
	{
	public:
		D3D9ShaderFinder( eShaderType ShaderType, _byte* Code, dword Size );

		virtual dword GetCodeSize( ) const { return mSize;}

		virtual _byte* GetCodePointer( )const  {return mCode;}

		virtual eShaderType GetType( ) const { return mType; }

	private:
		_byte* mCode;
		dword mSize;
		eShaderType mType;
	};


}