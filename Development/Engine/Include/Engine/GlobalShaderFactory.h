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
		全局Shader的自动引用识别指针
		@remark
			这是一个方便的指针,减少重复代码生成
			GlobalShaderRef<ElementVertexShader> VertexShaderRef;
			VertexShaderRef->SetParameter( Transform.Transpose() );
	*/
	template<typename TShaderType>
	class GlobalShaderRef
	{
	public:
		GlobalShaderRef( )
		{
			GlobalShaderFactory* Factoy = GShaderMaster->GetFactory<GlobalShaderFactory>( );

			Shader = (TShaderType*)Factoy->Find( RTTI_CLASS_INFO(TShaderType) );
		}

		TShaderType* operator->() const
		{
			return Shader;
		}

		TShaderType* operator*() const
		{
			return Shader;
		}

	private:
		TShaderType* Shader;
	};

	/** 
		全局Shader工厂
		@remark
			管理全局Shader的编译及寻找
	*/
	class GlobalShaderFactory : public ShaderFactory
	{
		DECLARE_RTTI_CLASS( GlobalShaderFactory, ShaderFactory, ClassFlag_Intrinsic| ClassFlag_Serializable )
	public:
		virtual ~GlobalShaderFactory( );

		virtual dword Compile( );

		virtual void Serialize( BinarySerializer& Ser );

		virtual void Clear( );
		
		virtual void LoadCache( );

		virtual void SaveCache( );

		/** 
			根据全局Shader的类型查到操作入口
		*/
		RenderShader* Find( RTTIClass* ClassInfo );

		
	private:
		typedef dynamic_hash<RTTIClass*, RenderShader*> GlobalShaderMap;

		bool CompileGlobalShader( RenderShader* Binder, ShaderCompilerEnvironment& CompilerEnvironment );
		

		GlobalShaderMap mGlobalShaderMap;
	};

	/** @} */
	/** @} */
}
