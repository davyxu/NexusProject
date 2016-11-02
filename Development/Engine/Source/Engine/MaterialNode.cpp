#include "EnginePCH.h"
#include "NekoEngine_Material.h"

namespace NekoEngine
{
	#define IMPLEMENT_MATERIAL_STATIC_PROPERTY( TClass, Name, Desc ) \
	void TClass::StaticInitProperty() \
	{\
		PropertyCollection* PC = RTTI_CLASS_INFO(TClass)->GetPropertyCollection( );\
		PC->Add( PROPERTY_CALLBACK( TClass,WString,L"Name", &TClass::OnPropertyName ) )->SetReadOnly( true );\
		PC->Add( PROPERTY_CALLBACK( TClass,WString,L"Desc", &TClass::OnPropertyDesc ) )->SetReadOnly( true );\
	}\
	void TClass::OnPropertyName( TClass*, WString* Value, bool IsGet )\
	{\
		if ( IsGet ){	*Value = Name; }\
	}\
	void TClass::OnPropertyDesc( TClass*, WString* Value, bool IsGet )\
	{\
		if ( IsGet ){	*Value = Desc; }\
	}



	//////////////////////////////////////////////////////////////////////////
	// MaterialNodeSampler
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( MaterialNodeTextureSampler  )

	//IMPLEMENT_MATERIAL_STATIC_PROPERTY( MaterialNodeTextureSampler, L"Texture Sampler", L"Contain texture sampler an a uv input slot")
	MaterialNodeTextureSampler::MaterialNodeTextureSampler( )
	{
		mUV.Bind( L"UV", this, MVT_Float2, "Parameters.TexCoords" );

		mSampler = new MaterialParameterTextureSampler;
	}

	MaterialNodeTextureSampler::~MaterialNodeTextureSampler()
	{
		delete mSampler;
	}

	void MaterialNodeTextureSampler::Init(  MaterialCompiler* Compiler, MaterialDescriptor* Mat  )
	{
		Super::Init( Compiler, Mat );

		/// Compiler会管理这个内存
		Compiler->RegisterUniformParameter( mSampler );
	}

	void MaterialNodeTextureSampler::Compile( MaterialCompiler* Compiler )
	{
		Super::Compile( Compiler );

		mOutputResult = Compiler->Texture2DSampler( mUV.GetResult(), mSampler );
	}

	void MaterialNodeTextureSampler::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		mSampler->Serialize( Ser );
	}

	//////////////////////////////////////////////////////////////////////////
	// MaterialNodeConstant
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( MaterialNodeConstant  )

	MaterialNodeConstant::MaterialNodeConstant( )
		: mData( Color::cWhite )
	{
	
	}

	void MaterialNodeConstant::Compile( MaterialCompiler* Compiler )
	{
		Super::Compile( Compiler );

		mOutputResult = Compiler->Constant4( mData );
	}

	void MaterialNodeConstant::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		Ser << mData;
	}

	//////////////////////////////////////////////////////////////////////////
	// MaterialNodeMathOperator
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( MaterialNodeMathOperator  )

	MaterialNodeMathOperator::MaterialNodeMathOperator( )
		: mOperatorType( MMO_Add )
	{
		mDataX.Bind( L"X", this );
		mDataY.Bind( L"Y", this );
	}

	void MaterialNodeMathOperator::Compile(MaterialCompiler* Compiler)
	{
		Super::Compile( Compiler );

		mDataX.SolveInputMaskAsValueType();

		mDataY.SolveInputMaskAsValueType();

		mOutputResult = Compiler->MathOperator( mOperatorType, mDataX.GetResult(), mDataY.GetResult() );
	}

	void MaterialNodeMathOperator::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		Ser << mOperatorType;
	}

	NekoEngine::WString MaterialNodeMathOperator::GetName()
	{
		switch ( mOperatorType )
		{
		case MMO_Add:	return L"Math.Add";
		case MMO_Sub:	return L"Math.Substract";
		case MMO_Mul:	return L"Math.Multipy";
		case MMO_Div:	return L"Math.Divide";
		case MMO_Dot:	return L"Math.Dot";
		}

		return L"Unknown Math";
	}


	//////////////////////////////////////////////////////////////////////////
	// MaterialNodeBumpOffset
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( MaterialNodeBumpOffset )

	MaterialNodeBumpOffset::MaterialNodeBumpOffset()
	{
		mUV.Bind( L"UV", this, MVT_Float3 );
	}

	void MaterialNodeBumpOffset::Compile(MaterialCompiler* Compiler)
	{
		Super::Compile(Compiler);

		mOutputResult = Compiler->Normalize( Compiler->Sub( Compiler->Mul( mUV.GetResult(), 
															  Compiler->Constant(2.0f) ), 
											  Compiler->Constant(1.0f) ) 
										    );
	}


	//////////////////////////////////////////////////////////////////////////
	// MaterialNodeNormalize
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( MaterialNodeNormalize )

	MaterialNodeNormalize::MaterialNodeNormalize()
	{
		mData.Bind( L"Input", this );
	}

	void MaterialNodeNormalize::Compile(MaterialCompiler* Compiler)
	{
		Super::Compile( Compiler );

		mOutputResult = Compiler->Normalize( mData.GetResult() );
	}


	//////////////////////////////////////////////////////////////////////////
	// MaterialNodeUniformVector
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( MaterialNodeUniformVector )

	MaterialNodeUniformVector::MaterialNodeUniformVector()
	: mParameterData(null)
	{
		mParameterData = new MaterialParameterVector;
	}

	MaterialNodeUniformVector::~MaterialNodeUniformVector()
	{
		delete mParameterData;
	}


	void MaterialNodeUniformVector::Init(  MaterialCompiler* Compiler, MaterialDescriptor* Mat  )
	{
		Super::Init( Compiler, Mat );

	
		Compiler->RegisterUniformParameter( mParameterData );
	}
	void MaterialNodeUniformVector::Compile(MaterialCompiler* Compiler)
	{
		Super::Compile(Compiler);		
		MaterialParameterVector* VectorParameter = RTTI_CAST(MaterialParameterVector, mParameterData );

		mOutputResult = Compiler->UniformVector( VectorParameter->mValueType, mParameterData );
	}

	void MaterialNodeUniformVector::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		mParameterData->Serialize( Ser );
	}

	//////////////////////////////////////////////////////////////////////////
	// MaterialPointLight
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( MaterialNodePointLight)

	MaterialNodePointLight::MaterialNodePointLight( )
	{
		mEmissive	.Bind( L"Emissive",		this, MVT_Float3, "float3(0.0f,0.0f,0.0f)" );
		mDiffuse	.Bind( L"Diffuse",		this, MVT_Float3, "float3(0.0f,0.0f,0.0f)" );
		mNormal		.Bind( L"Normal",		this, MVT_Float3, "Parameters.VertexNormal" );
		mSpecular	.Bind( L"Specular",		this, MVT_Float3, "float3(0.0f,0.0f,0.0f)");
		mSpecularPow.Bind( L"SpecularPow",	this, MVT_Float,  "16.0f");
		mOpacity	.Bind( L"Opacity",		this, MVT_Float,  "1.0f");
	}


	typedef std::vector<void*> FormatParameterArray;

	void MaterialNodePointLight::Compile(MaterialCompiler* Compiler)
	{
		Super::Compile(Compiler);
	}

	void MaterialNodePointLight::CombineCode(MaterialCompiler* Compiler)
	{
		Super::CombineCode(Compiler);

		// 读取模板文件
		FileStream ShaderFile;
		if ( !ShaderFile.Open(ResourceLocation::Resovle(L"EngineShader.MaterialTemplatePixelShader.SF").c_str(), FAM_Read ) )
		{
			Assert( false );
		}

		dword ShaderFileSize = ShaderFile.GetSize();

		AString ShaderTemplateCode;
		ShaderTemplateCode.resize( ShaderFileSize + 1);
		ShaderFile.ReadBuffer( (void*)ShaderTemplateCode.data(), ShaderFileSize * sizeof( char ) );
		ShaderTemplateCode[ShaderFileSize] = 0;

		dword CodeGenLength = 0;

		// 说明:
		//float3 GetMaterialEmissive( MaterialParameters Parameters )
		//{
		//	%s					<- CodeBody
		//		return %s;		<- CodeResult
		//}


		// 格式化数组
		FormatParameterArray tFormatParameterArray;



		// 填充顶部变量定义
		MaterialParameterArray& ParamterList = Compiler->GetParameterList();

		AString HeadDefine;
		StringConverter conv;
		for ( MaterialParameterArray::iterator it = ParamterList.begin();
			it != ParamterList.end();
			++it )
		{
			MaterialParameter* Parameter = *it;

			// 类型 变量名 + Parameter Name 做注释
			char Buffer[256];
			StringHelperA::FormatBuffer( Buffer, 256, "%s %s; // %s\r\n", Parameter->MakeDefineType(), Parameter->MakeParameterName().c_str(), conv.UnicodeToAnsi( Parameter->GetName() ) );
			HeadDefine += Buffer;
		}

		CodeGenLength += HeadDefine.length();
		tFormatParameterArray.push_back( (void*)HeadDefine.c_str() );



		// 编译代码,并构建格式化数组
		for ( dword i = 0;i< mSlotArray.size();i++)
		{
			MaterialSlot* Slot = mSlotArray[i];

			CodeGenLength += StringHelperA::Length( Slot->GetCombinedCodeBody() );
			CodeGenLength += StringHelperA::Length( Slot->GetCombinedCodeResult() );

			tFormatParameterArray.push_back( (void*)Slot->GetCombinedCodeBody() );
			tFormatParameterArray.push_back( (void*)Slot->GetCombinedCodeResult() );
		}


		Compiler->WriteFinalCode( ShaderFileSize + CodeGenLength , ShaderTemplateCode.c_str(), &tFormatParameterArray[0] );
	}





	




}