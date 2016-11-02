#pragma once

namespace NekoEngine
{
	class MaterialInstance;

	/**
		材质用Shader
	*/
	class MaterialShader : public RenderShader
	{
		DECLARE_RTTI_CLASS( MaterialShader, RenderShader, ClassFlag_Intrinsic| ClassFlag_Serializable )
	public:
		virtual void PreprocessMacro( MaterialInstance* Instance , ShaderCompilerEnvironment& CompilerEnvironment){ }

		virtual void BindMaterialParameter( MaterialInstance* Instance, eShaderType ShaderType, const ShaderParameterMap& ParameterMap ){}

		virtual void Serialize( BinarySerializer& Ser );

		virtual bool IsMaterialShader( ){ return true; }

		const wchar* GetShaderSource( ){ return mShaderSource.c_str(); }

	protected:
		friend class MaterialShaderFactory;
		WString		mShaderSource;
	};

	/** 
		材质像素Shader
	*/
	class MaterialPixelShader : public MaterialShader
	{
		DECLARE_RTTI_CLASS( MaterialPixelShader, MaterialShader, ClassFlag_Intrinsic| ClassFlag_Serializable )
	public:
		MaterialPixelShader( );

		virtual ~MaterialPixelShader( );		

		void InitMetaData( );

		virtual ShaderMetaInfo* GetShaderMetaInfo( );

		virtual void BindMaterialParameter( MaterialInstance* Instance, eShaderType ShaderType, const ShaderParameterMap& ParameterMap );

		void CommitMaterialParameter( MaterialInstance* Instance );

		virtual void Serialize( BinarySerializer& Ser );
		
	private:
		struct MaterialShaderParameter
		{
			ShaderParameter mShaderParameter;
			dword mUniformIndex;

			friend BinarySerializer& operator <<( NekoEngine::BinarySerializer& Ser, MaterialShaderParameter& Data )
			{	
				Ser << Data.mShaderParameter
					<< Data.mUniformIndex;

				return Ser;
			}
		};

		typedef std::list<MaterialShaderParameter> ParameterList;
		ParameterList mParameterList;

		ShaderMetaInfo* mMetaInfo;
	};

	/** 
		材质顶点Shader
	*/
	class MaterialVertexShader : public MaterialShader
	{
	DECLARE_RTTI_CLASS( MaterialVertexShader, MaterialShader, ClassFlag_Intrinsic )

	protected:
		friend class MaterialVertexShaderKey;
		friend class ShaderMaster;
	};

}
