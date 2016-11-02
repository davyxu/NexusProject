#pragma once

namespace NekoEngine
{
	/** 
		材质Shader集合
	*/
	struct MaterialShaderCollection
	{
		MaterialVertexShader* mVertexShader;

		MaterialPixelShader* mPixelShader;

		MaterialShaderCollection()
			: mVertexShader( null )
			, mPixelShader( null )
		{

		}

		friend BinarySerializer& operator <<( NekoEngine::BinarySerializer& Ser, MaterialShaderCollection& Data )
		{
			Ser << Data.mVertexShader
				<< Data.mPixelShader;
		
			return Ser;
		}
	};

	/** 
		
	*/
	class MaterialShaderFactory : public ShaderFactory
	{
		DECLARE_RTTI_CLASS( MaterialShaderFactory , ShaderFactory, ClassFlag_Intrinsic| ClassFlag_Serializable)
	public:
		MaterialShaderFactory( );

		virtual ~MaterialShaderFactory( );

		/// 编译材质Shader
		virtual dword Compile( );

		virtual void Serialize( BinarySerializer& Ser );

		virtual void Clear( );

		virtual void Tick( float Elapse );

		virtual void LoadCache( );

		virtual void SaveCache( );

		MaterialShaderCollection GetShader( MaterialInstance* Instance , const wchar* MaterialName, RTTIClass* VertexShaderClass );

		/// 序列化材质Shader
		bool SerializeMaterialShaderCache( const wchar* MaterialName, MaterialShaderCollection& Collection, bool IsLoading );

	private:
		bool CompileMaterialShader( MaterialInstance* MatIns , MaterialShader* Binder, ShaderCompilerEnvironment& CompilerEnvironment );

		bool IsShaderOutofTime( MaterialShader* Binder );

		void RebuildMaterialIndex( );

	private:
		typedef dynamic_hash<WString, MaterialShaderCollection> MaterialShaderMap;

		MaterialShaderMap mMaterialShaderMap;
		dword mNeedRebuildIndex;
	};

}
