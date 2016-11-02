#include "EnginePCH.h"
#include "NekoEngine_Material.h"

namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS( MaterialInstance )

	

	MaterialInstance::MaterialInstance()
	: mUseTangentSpace( false )
	, mUseSkeletonMesh( false )
	, mBlendMode( BLEND_Opaque )
	, mWireframe( false )
	, mTwoSided( false )
	, mEnableDepthWrite( true )
	{

	}

	MaterialInstance::~MaterialInstance()
	{
		for ( MaterialParameterArray::iterator it = mShaderParameterArray.begin();
			it != mShaderParameterArray.end();
			++it )
		{
			MaterialParameter* Parameter = *it;
			delete Parameter;
		}

		mShaderParameterArray.clear();
	}


	MaterialParameter* MaterialInstance::GetShaderParameter( dword UniformIndex )
	{
		if ( UniformIndex >= mShaderParameterArray.size() )
			return null;

		return mShaderParameterArray[ UniformIndex ];
	}

	NekoEngine::dword MaterialInstance::GetShaderParameterCount()
	{
		return mShaderParameterArray.size();
	}

	void MaterialInstance::Serialize( BinarySerializer& Ser )
	{
		Ser << mMaterialName
			<< mShaderParameterArray;

		Ser << mUseTangentSpace
			<< mUseSkeletonMesh
			<< mBlendMode
			<< mWireframe
			<< mTwoSided
			<< mEnableDepthWrite;
	}

	void MaterialInstance::PostLoad()
	{
		for ( dword i = 0; i< mShaderParameterArray.size() ; i++ )
		{
			MaterialParameter* Parameter = mShaderParameterArray[i];
			Parameter->PrepareResource( );
		}

		mParamterResourceFence.BeginFence();
	}

	bool MaterialInstance::IsManualResourceReady( )
	{
		return mParamterResourceFence.GetNumPendingFences() == 0;
	}



	void MaterialInstance::CommitShader( bool VertexShaderOnly )
	{
		mShaderCollection.mVertexShader->Commit();

		if ( !VertexShaderOnly )
		{
			// 
			mShaderCollection.mPixelShader->CommitMaterialParameter( this );
			mShaderCollection.mPixelShader->Commit();
		}
	}

	extern void SetBlendState(eBlendMode BlendMode);

	void SetFillState( bool TwoSided, bool Wireframe )
	{
		if ( TwoSided && Wireframe )
		{
			TStaticRasterizerState<FM_Wireframe,CM_None>::Commit();
		}
		else if ( TwoSided == false && Wireframe == false )
		{
			TStaticRasterizerState<FM_Solid,CM_CW>::Commit();
		}
		else if ( TwoSided == true && Wireframe == false )
		{
			TStaticRasterizerState<FM_Solid,CM_None>::Commit();
		}
		else if ( TwoSided == false && Wireframe == true )
		{
			TStaticRasterizerState<FM_Wireframe,CM_CW>::Commit();
		}
	}

	void SetDepthState( bool EnableDepthWrite )
	{
		if ( EnableDepthWrite )
		{
			TStaticDepthState<true, CF_LessEqual>::Commit();
		}
		else
		{
			TStaticDepthState<false, CF_Always>::Commit();
		}
	}

	void MaterialInstance::CommitRenderState( )
	{
		TStaticDepthState<>::Commit();

		SetBlendState( mBlendMode );

		SetFillState( mTwoSided, mWireframe );

		SetDepthState( mEnableDepthWrite );
	}

	void MaterialInstance::LoadShader( RTTIClass* VertexShaderProvider )
	{
		MaterialShaderFactory* Factory = GShaderMaster->GetFactory<MaterialShaderFactory>();

		// 这里只需要一个材质名称即可重新编译Shader文件/从缓冲读取Shader，和Material没有任何依赖
		mShaderCollection = Factory->GetShader( this, mMaterialName.c_str(), VertexShaderProvider );
	}

	MaterialParameter* MaterialInstance::SearchParameter( const wchar* Name )
	{
		for ( MaterialParameterArray::iterator it = mShaderParameterArray.begin();
			it != mShaderParameterArray.end();
			++it )
		{
			MaterialParameter* Parameter = *it;
			if ( StringHelper::Compare( Parameter->GetName(), Name) )
				return Parameter;
		}

		return null;
	}


	void MaterialInstance::SetVector4( const wchar* Name, const Vector4& Value )
	{
		MaterialParameterVector* Parameter = RTTI_CAST(MaterialParameterVector, SearchParameter( Name ) );
		if ( Parameter == null )
		{
			Parameter = new MaterialParameterVector;
			Parameter->SetName( Name );
		}

		Parameter->mValue = Value;
	}

	void MaterialInstance::SetColor( const wchar* Name, const Color& C )
	{
		SetVector4(Name, Vector4(C.r, C.g, C.b, C.a ) );
	}


	void MaterialInstance::LinkMaterial( MaterialDescriptor* Mat )
	{
		Assert( Mat != null );

		MaterialParameterArray& MParameterList = Mat->GetCompiler()->GetParameterList();

		mShaderParameterArray.clear();

		for ( MaterialParameterArray::iterator it = MParameterList.begin();
			it != MParameterList.end();
			++it )
		{
			MaterialParameter* Parameter = *it;
			mShaderParameterArray.push_back( Parameter->Clone() );
		}

		// 拷贝参数列表
		
		// 拷贝引用的材质名
		mMaterialName = Mat->GetName();
	}

	void MaterialInstance::StaticInitProperty()
	{
		PropertyCollection* PC = RTTI_CLASS_INFO(MaterialInstance)->GetPropertyCollection( );
		PC->Add( PROPERTY_MEMBER_VAR( MaterialInstance,bool,  mUseTangentSpace ) );
		PC->Add( PROPERTY_MEMBER_VAR( MaterialInstance,bool,  mUseSkeletonMesh ) );		
		PC->Add( PROPERTY_MEMBER_VAR( MaterialInstance,dword, mBlendMode ) );
		PC->Add( PROPERTY_MEMBER_VAR( MaterialInstance,bool,  mWireframe ) );
		PC->Add( PROPERTY_MEMBER_VAR( MaterialInstance,bool,  mTwoSided ) );
		PC->Add( PROPERTY_MEMBER_VAR( MaterialInstance,bool,  mEnableDepthWrite ) );
	}






}