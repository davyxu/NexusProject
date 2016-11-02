#include "EnginePCH.h"
#include "NekoEngine_Material.h"

namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS( MaterialParameter )

	MaterialParameter::MaterialParameter() 
		: mUniformIndex( -1 )
	{

	}

	void MaterialParameter::Serialize( BinarySerializer& Ser )
	{
		Ser << mUniformIndex
			<< mName;
	}

	MaterialParameter::~MaterialParameter()
	{

	}

	MaterialParameter* MaterialParameter::Clone( )
	{
		MaterialParameter* NewParameter = RTTI_CAST( MaterialParameter, GetClass()->CreateInstance() );
		NewParameter->mUniformIndex = mUniformIndex;
		NewParameter->mName = mName;

		return NewParameter;
	}

	IMPLEMENT_RTTI_CLASS( MaterialParameterTextureSampler)

	AString MaterialParameterTextureSampler::MakeParameterName(  )
	{
		return StringHelperA::FormatString( "UniformTexture%d", mUniformIndex );
	}

	void MaterialParameterTextureSampler::PrepareResource()
	{
		mTexture = GObjectManager->LoadObject<TextureResource>( mResourceLocation );
	}

	void MaterialParameterTextureSampler::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		Ser << mResourceLocation;
	}

	bool MaterialParameterTextureSampler::IsResourceReady()
	{
		return mTexture.Valid() && mTexture->IsResourceReady();
	}

	void MaterialParameterTextureSampler::Commit( RHIShader* Shader, const ShaderParameter& Parameter )
	{
		if ( mTexture.Valid() )
			Shader->SetTextureParameter( Parameter, mTexture->GetRenderTexture() );
		else
			Shader->SetTextureParameter( Parameter, null );
	}

	void MaterialParameterTextureSampler::SetValue( const wchar* Value )
	{
		mResourceLocation = Value;
	}

	WString MaterialParameterTextureSampler::GetValue()
	{
		return mResourceLocation;
	}

	MaterialParameter* MaterialParameterTextureSampler::Clone()
	{
		MaterialParameterTextureSampler* Parameter = RTTI_CAST(MaterialParameterTextureSampler, Super::Clone() );
		Parameter->mResourceLocation = mResourceLocation;
		Parameter->mTexture = mTexture;
		return Parameter;
	}


	IMPLEMENT_RTTI_CLASS( MaterialParameterVector)
	MaterialParameterVector::MaterialParameterVector()
	: mValueType( MVT_Float4 )
	{

	}

	AString MaterialParameterVector::MakeParameterName(  )
	{
		return StringHelperA::FormatString( "UniformVector%d", mUniformIndex );
	}

	void MaterialParameterVector::Commit( RHIShader* Shader, const ShaderParameter& Parameter )
	{
		// TODO 这里NumVector4需要调试
		Shader->SetContantParameter( Parameter, (const float*)&mValue, 1 );
	}

	void MaterialParameterVector::SetValue( const wchar* Value )
	{
		switch( mValueType )
		{
		case MVT_Float:
			value_parse( Value, mValue.x );
			break;
		case MVT_Float2:
			{
				Vector2 v;
				if ( value_parse( Value, v ) )
				{
					mValue = Vector4( v.x, v.y, 0, 0 );
				}
				break;
			}
		case MVT_Float3:
			{
				Vector3 v;
				if ( value_parse( Value, v ) )
				{
					mValue = Vector4( v.x, v.y, v.z, 0 );
				}
				break;
			}

		case MVT_Float4:
			value_parse( Value, mValue );
			break;
		}
		
	}

	NekoEngine::WString MaterialParameterVector::GetValue()
	{
		switch( mValueType )
		{
		case MVT_Float:
			return value_tostringW( mValue.x );
		case MVT_Float2:
			return value_tostringW( Vector2(mValue.x, mValue.y) );
		case MVT_Float3:
			return value_tostringW( Vector3(mValue.x, mValue.y, mValue.z ) );
		case MVT_Float4:
			return value_tostringW( mValue );
		}

		return L"";
	}

	MaterialParameter* MaterialParameterVector::Clone()
	{
		MaterialParameterVector* Parameter = RTTI_CAST(MaterialParameterVector, Super::Clone() );
		Parameter->mValueType = mValueType;
		Parameter->mValue = mValue;
		return Parameter;
	}

	IMPLEMENT_RTTI_CLASS( MaterialParameterBool)
	void MaterialParameterBool::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		Ser << mValue;
	}

	void MaterialParameterBool::SetValue( const wchar* Value )
	{
		value_parse( Value, mValue );
	}

	WString MaterialParameterBool::GetValue()
	{
		return value_tostringW( mValue );
	}

	MaterialParameter* MaterialParameterBool::Clone()
	{
		MaterialParameterBool* Parameter = RTTI_CAST(MaterialParameterBool, Super::Clone() );
		Parameter->mValue = mValue;
		return Parameter;
	}
}