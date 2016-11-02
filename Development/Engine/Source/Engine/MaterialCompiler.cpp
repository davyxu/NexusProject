#include "EnginePCH.h"
#include "NekoEngine_Material.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// MaterialCompiler
	//////////////////////////////////////////////////////////////////////////

	const MaterialMaskedValue MaterialMaskedValue::cDefault  = MaterialMaskedValue(-1);

	MaterialCompiler::MaterialCompiler( )
		: mError( 0 )
	{
		mLogger = new SimpleLog;
		mLogger->SetPrefix(L"MaterialCompiler");
	}

	MaterialCompiler::~MaterialCompiler()
	{
		// 删除代码
		for ( CodeFragment::iterator it = mCodeFragment.begin();
			it != mCodeFragment.end();
			++it)
		{
			delete (*it).mCode;
		}

		mCodeFragment.clear();


		// 删除注册到Compiler的参数类
		//for ( MaterialParameterArray::iterator it = mMaterialParameterList.begin();
		//	it != mMaterialParameterList.end();
		//	++it )
		//{
		//	MaterialParameter* Parameter = *it;
		//	delete Parameter;
		//}

		//mMaterialParameterList.clear();

		delete mLogger;
	}

	MaterialCodeBody& MaterialCompiler::GetRelatedCode( dword CodeIndex )
	{
		return mCodeFragment[ CodeIndex - 1].mRelatedCode;
	}

	dword MaterialCompiler::AddCodeChunk( eMaterialValueType ValueType, dword VarIndex, const char* Code, ... )
	{
		AString UserCode = StringHelperA::FormatStringV( Code, (char*)( &Code ) + sizeof( Code ) );		

		CodeChunk Chunk;
		Chunk.mCode = new AString;
		Chunk.mValueType = ValueType;

		*Chunk.mCode = StringHelperA::FormatString(	"\t%s %s = %s\r\n", 
												GetValueTypeString( ValueType ),
												GetVar( VarIndex ).c_str(),
												UserCode.c_str() 
											);

		// 一些常量不会被添加2次
		for ( dword i = 0;i< mCodeFragment.size();i++)
		{
			AString& ExistCode = *mCodeFragment[i].mCode;
			if ( ExistCode == *Chunk.mCode )
			{
				delete Chunk.mCode;
				return i + 1;
			}
		}

		mCodeFragment.push_back( Chunk );

		return mCodeFragment.size();
	}

	

	const char* MaterialCompiler::GetValueTypeString( eMaterialValueType ValueType )
	{
		switch ( ValueType )
		{
		case MVT_Float:
			return "float ";
		case MVT_Float2:
			return "float2";
		case MVT_Float3:
			return "float3";
		case MVT_Float4:
			return "float4";
		}

		Assert("Unknown Value Type !");

		return "float4";
	}




	AString MaterialCompiler::GetVar( dword VarIndex )
	{
		return StringHelperA::FormatString( "Local%d", VarIndex );
	}



	dword MaterialCompiler::GetFreeVarIndex( )
	{
		return mCodeFragment.size() + 1;
	}



	Logger* MaterialCompiler::ErrorLogger()
	{
		mError++;
		return mLogger;
	}

	AString* MaterialCompiler::GetCodeChunk( dword Index )
	{
		return mCodeFragment[ Index ].mCode;
	}

	eMaterialValueType MaterialCompiler::GetCodeValueType( dword CodeIndex )
	{
		return mCodeFragment[ CodeIndex -1 ].mValueType;
	}

	void MaterialCompiler::WriteFinalCode( dword CodeLength, const char* TemplateCode, void* ParameterList )
	{
		mFinalCode.resize( CodeLength + 1 );

		// 以模板文件为蓝本,构建最终文件
		StringHelperA::FormatBufferV( &mFinalCode[0], mFinalCode.size(), TemplateCode,ParameterList );

		mFinalCode[ CodeLength ] = 0;
	}

	void MaterialCompiler::SaveCode( const wchar* FileName )
	{
		FileStream FileGen( FileName, FAM_Write );

		FileGen.WriteBuffer( mFinalCode.c_str(), StringHelperA::Length(mFinalCode.c_str() ) );

		FileGen.Close();
	}

	dword MaterialCompiler::Texture2DSampler( const MaterialMaskedValue& UV, MaterialParameter* Sampler )
	{

		if ( UV.Valid() )
		{
			dword CodeIndex = AddCodeChunk( MVT_Float4, GetFreeVarIndex(), "tex2D( %s, %s );", Sampler->MakeParameterName().c_str() , GetValueMasked( UV ).c_str() );

			GetRelatedCode( CodeIndex ).push_back( UV.mCodeIndex );
			return CodeIndex;
		}

		return AddCodeChunk( MVT_Float4, GetFreeVarIndex(), "tex2D( %s , Parameters.TexCoords );", Sampler->MakeParameterName().c_str()  );
	}

	dword MaterialCompiler::UniformVector( eMaterialValueType Type, MaterialParameter* Vector )
	{

		return AddCodeChunk( Type, GetFreeVarIndex(), "%s;", Vector->MakeParameterName( ).c_str() );
	}

	dword MaterialCompiler::Constant( float Value )
	{
		return AddCodeChunk( MVT_Float, GetFreeVarIndex(),  "%f;", Value);
	}

	dword MaterialCompiler::Constant2( float X, float Y )
	{
		return AddCodeChunk( MVT_Float2, GetFreeVarIndex(),  "float2( %f, %f );",  X, Y );
	}

	dword MaterialCompiler::Constant3( float X, float Y, float Z )
	{
		return AddCodeChunk( MVT_Float3, GetFreeVarIndex(),  "float3( %f, %f, %f );", X, Y, Z );
	}

	dword MaterialCompiler::Constant4( const Color& Value )
	{
		return AddCodeChunk( MVT_Float4, GetFreeVarIndex(),  "float4( %f, %f, %f, %f );", Value.r, Value.g, Value.b, Value.a );
	}




	dword MaterialCompiler::Add( const MaterialMaskedValue& A, const MaterialMaskedValue& B )
	{
		return MathOperator( MMO_Add, A, B );
	}

	dword MaterialCompiler::Sub( const MaterialMaskedValue& A, const MaterialMaskedValue& B )
	{
		return MathOperator( MMO_Sub, A, B );
	}

	dword MaterialCompiler::Mul( const MaterialMaskedValue& A, const MaterialMaskedValue& B )
	{
		return MathOperator( MMO_Mul, A, B );
	}

	dword MaterialCompiler::Div( const MaterialMaskedValue& A, const MaterialMaskedValue& B )
	{
		return MathOperator( MMO_Div, A, B );
	}

	dword MaterialCompiler::Normalize( const MaterialMaskedValue& Data )
	{
		eMaterialValueType InputValueType = GetCodeValueType( Data.mCodeIndex );

		dword CodeIndex = AddCodeChunk( InputValueType, GetFreeVarIndex(), "normalize( %s );", GetValueMasked( Data ).c_str() );

		GetRelatedCode( CodeIndex).push_back( Data.mCodeIndex );

		return CodeIndex;
	}



	eMaterialValueType MaterialCompiler::GetProperValueType( const MaterialMaskedValue& Value )
	{
		eMaterialValueType CodeValueType = GetCodeValueType( Value.mCodeIndex );
		eMaterialValueType MaskValueType = GetMaskMappingValueType( Value.mMaskType );

		// Mask的类型在使用时，使用Mask
		if ( MaskValueType < CodeValueType )
			return MaskValueType;

		//否则使用原生的类型
		return CodeValueType;
	}

	dword MaterialCompiler::MathOperator( eMaterialMathOperator Operator, const MaterialMaskedValue& A, const MaterialMaskedValue& B )
	{
		eMaterialValueType ValueTypeA = GetProperValueType( A );

		eMaterialValueType ValueTypeB = GetProperValueType( B );

		eMaterialValueType ResultValueType;
		if ( ValueTypeA == MVT_Float )
			ResultValueType = ValueTypeB;
		else if ( ValueTypeB == MVT_Float )
			ResultValueType = ValueTypeA;
		else if ( ValueTypeA == ValueTypeB )
			ResultValueType = ValueTypeA;
		else
			return -1;

		char OperatorChar;

		switch ( Operator )
		{
		case MMO_Add:
			OperatorChar = '+';
			break;
		case MMO_Sub:
			OperatorChar = '-';
			break;
		case MMO_Mul:
			OperatorChar = '*';
			break;
		case MMO_Div:
			OperatorChar = '/';
			break;
		}

		dword CodeIndex = AddCodeChunk( ResultValueType, 
							 GetFreeVarIndex(),
							 "%s %c %s;", 
							 GetValueMasked( A ).c_str(), 
							 OperatorChar, 
							 GetValueMasked( B ).c_str() );

		MaterialCodeBody& RelatedCode = GetRelatedCode( CodeIndex );
		RelatedCode.push_back( A.mCodeIndex );
		RelatedCode.push_back( B.mCodeIndex );

		return CodeIndex;
	}

	eMaterialValueType MaterialCompiler::GetMaskMappingValueType( eMaterialValueMask Mask )
	{
		switch ( Mask )
		{
		case MVM_RGBA:
			return MVT_Float4;
		case MVM_RGB:
			return MVT_Float3;
		case MVM_R:
		case MVM_G:
		case MVM_B:
		case MVM_A:
			return MVT_Float;
		}

		return MVT_None;
	}


	AString MaterialCompiler::GetValueMasked( const MaterialMaskedValue& MaskValue )
	{
		eMaterialValueType ValueType = GetCodeValueType( MaskValue.mCodeIndex );

		eMaterialValueType MaskValueType = GetMaskMappingValueType( MaskValue.mMaskType );
		
		const char* MaskStr = "";

		// 类型是一样的，就不用做mask了
		if ( ValueType > MaskValueType )
		{
			switch ( MaskValue.mMaskType )
			{
			case MVM_RGBA:
				break;
			case MVM_RGB:
				MaskStr = ".rgb";
				break;
			case MVM_R:
				MaskStr = ".r";
				break;
			case MVM_G:
				MaskStr = ".g";
				break;
			case MVM_B:
				MaskStr = ".b";
				break;
			case MVM_A:
				MaskStr = ".a";
				break;
			default:
				Assert( "Not supported Mask format" );
				break;
			}
		}

		// 如果跟输出类型一样,就不必mask

		return StringHelperA::FormatString( "Local%d%s", MaskValue.mCodeIndex, MaskStr );
	}

	void MaterialCompiler::CollectRelatedCode( dword CodeIndex, MaterialCodeBody& CodeBody)
	{
		MaterialCodeBody& RelatedCode = GetRelatedCode( CodeIndex );
		
		for ( MaterialCodeBody::iterator it = RelatedCode.begin();
			it != RelatedCode.end();
			++it )
		{
			dword ThisRelated = *it;
			CollectRelatedCode( ThisRelated, CodeBody );
		}

		CodeBody.push_back( CodeIndex );
	}

	void MaterialCompiler::CombineCodeBody( const MaterialCodeBody& CodeBody, AString& Output )
	{
		// 计算需要合并代码的总长度
		dword TotalLength = 0;
		for ( MaterialCodeBody::const_iterator it = CodeBody.begin();
			it != CodeBody.end();
			++it)
		{
			TotalLength += GetCodeChunk(*it - 1)->length();
		}

		Output.resize( TotalLength + 1);

		// 合并代码
		dword Offset = 0;
		for ( MaterialCodeBody::const_iterator it = CodeBody.begin();
			it != CodeBody.end();
			++it)
		{
			AString* Str = GetCodeChunk(*it - 1);
			::memcpy( (void*)(Output.data( ) + Offset), Str->data( ), sizeof(char) * Str->length() );
			Offset += sizeof(char) * Str->length();
		}

		Output[TotalLength] = 0;
	}

	void MaterialCompiler::RegisterUniformParameter( MaterialParameter* Parameter )
	{
		Assert( Parameter != null );
		Assert ( Parameter->IsUniformParameter() && "Parameter alloced by MaterialCompiler should be uniform parameter" );
		Parameter->mUniformIndex = mMaterialParameterList.size();

		mMaterialParameterList.push_back( Parameter );
	}




}