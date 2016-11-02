#include "EnginePCH.h"
#include "NekoEngine_Material.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// MaterialShader
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( MaterialShader )

	void MaterialShader::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		Ser << mShaderSource;
	}



	//////////////////////////////////////////////////////////////////////////
	// MaterialPixelShader
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( MaterialPixelShader)

	MaterialPixelShader::MaterialPixelShader( )
		: mMetaInfo( null )
	{

	}

	MaterialPixelShader::~MaterialPixelShader()
	{
		delete mMetaInfo;
	}

	ShaderMetaInfo* MaterialPixelShader::GetShaderMetaInfo()
	{
		return mMetaInfo;
	}


	void MaterialPixelShader::BindMaterialParameter( MaterialInstance* Instance, eShaderType ShaderType,  const ShaderParameterMap& ParameterMap  )
	{
		for (dword i = 0; i< Instance->GetShaderParameterCount();i++ )
		{
			MaterialParameter* Parameter = Instance->GetShaderParameter( i );

			if ( !Parameter->IsUniformParameter() )
				continue;

			MaterialShaderParameter NewParameter;
			NewParameter.mShaderParameter.Bind( ParameterMap, Parameter->MakeParameterName(  ).c_str() );
			NewParameter.mUniformIndex = Parameter->GetUniformIndex();

			mParameterList.push_back( NewParameter );
		}
	}

	void MaterialPixelShader::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		Ser << mParameterList;

		if ( Ser.IsLoading() )
		{
			InitMetaData( );
		}
	}

	void MaterialPixelShader::CommitMaterialParameter( MaterialInstance* Instance )
	{
		for ( ParameterList::iterator it = mParameterList.begin();
			it != mParameterList.end();
			++it )
		{
			MaterialShaderParameter& MSP = *it;

			// 根据ShaderUniform的索引，获得对应的参数
			MaterialParameter* Parameter = Instance->GetShaderParameter( MSP.mUniformIndex );
			if ( Parameter == null )
				continue;

			Parameter->Commit( mShaderRHI, MSP.mShaderParameter );
		}
	}

	void MaterialPixelShader::InitMetaData(  )
	{
		if ( mMetaInfo != null )
			return;

		mMetaInfo = new ShaderMetaInfo( ST_PixelShader, GetShaderSource() , L"Main" );
	}



	IMPLEMENT_RTTI_CLASS( MaterialVertexShader )
}