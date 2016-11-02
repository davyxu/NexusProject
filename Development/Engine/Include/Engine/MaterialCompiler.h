#pragma once

namespace NekoEngine
{
	struct MaterialMaskedValue 
	{
		MaterialMaskedValue( dword CodeIndex )
			: mCodeIndex( CodeIndex )
			, mMaskType( MVM_RGBA )
		{

		}

		MaterialMaskedValue( dword CodeIndex, eMaterialValueMask Mask )
			: mCodeIndex( CodeIndex )
			, mMaskType( Mask )
		{

		}

		bool Valid( ) const { return mCodeIndex != -1; }


		dword mCodeIndex;
		eMaterialValueMask mMaskType;

		static const MaterialMaskedValue cDefault;
	};

	typedef std::list<dword> MaterialCodeBody;

	class MaterialParameter;

	
	/** 
		材质编译器
		@remark
			提供基础像素操作指令,代码生成
	*/
	class MaterialCompiler
	{
	public:
		MaterialCompiler(  );

		virtual ~MaterialCompiler( );

		/// 纹理采样器指令
		dword Texture2DSampler( const MaterialMaskedValue& UV, MaterialParameter* Sampler  );

		/// 外部变量指令
		dword UniformVector( eMaterialValueType Type, MaterialParameter* Vector  );

		/// 
		dword MathOperator( eMaterialMathOperator Operator, const MaterialMaskedValue& A, const MaterialMaskedValue& B );

		dword Constant( float Value );

		dword Constant2( float X, float Y );

		dword Constant3( float X, float Y, float Z );

		dword Constant4( const Color& Value );

		dword Add( const MaterialMaskedValue& A, const MaterialMaskedValue& B );

		dword Sub( const MaterialMaskedValue& A, const MaterialMaskedValue& B );

		dword Mul( const MaterialMaskedValue& A, const MaterialMaskedValue& B );

		dword Div( const MaterialMaskedValue& A, const MaterialMaskedValue& B );

		dword Normalize( const MaterialMaskedValue& Data );

		// 将外部需要交互的变量注册到Compiler
		void RegisterUniformParameter( MaterialParameter* Parameter );

		MaterialParameterArray& GetParameterList( ){ return mMaterialParameterList; }

	public:
		dword		AddCodeChunk( eMaterialValueType ValueType, dword VarIndex, const char* Code, ... );

		AString*	GetCodeChunk( dword Index );

		AString		GetVar( dword Index );

		Logger*		GetLogger( ){ return mLogger;}

		Logger*		ErrorLogger( );

		dword		GetFreeVarIndex(  );

		dword		GetErrorCount( ){ return mError; }

		void		WriteFinalCode( dword CodeLength, const char* TemplateCode, void* ParameterList );
	
		void		SaveCode( const wchar* FileName );

		void		CombineCodeBody( const MaterialCodeBody& CodeBody, AString& Output );

		static eMaterialValueType GetMaskMappingValueType( eMaterialValueMask Mask );

		

	private:
		friend class MaterialSlot;
		MaterialCodeBody&	GetRelatedCode( dword CodeIndex );

		const char*			GetValueTypeString( eMaterialValueType ValueType );

		eMaterialValueType	GetCodeValueType( dword CodeIndex );

		AString				GetValueMasked( const MaterialMaskedValue& MaskValue );

		void				CollectRelatedCode( dword CodeIndex, MaterialCodeBody& CodeBody);

		eMaterialValueType	GetProperValueType( const MaterialMaskedValue& Value );
		

	private:
		struct CodeChunk 
		{
			AString*			mCode;
			eMaterialValueType	mValueType;
			MaterialCodeBody	mRelatedCode;// 要生成这个Code需要的CodeLine
		};

		typedef std::vector<CodeChunk> CodeFragment;
		CodeFragment	mCodeFragment;
		Logger*			mLogger;
		dword			mError;
		AString			mFinalCode;
		MaterialParameterArray mMaterialParameterList;
	};

}
