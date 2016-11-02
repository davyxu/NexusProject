#include "EnginePCH.h"
#include "NekoEngine_Render.h"


namespace NekoEngine
{
	

	class BatchedPointVertexFormat : public RenderResource
	{
	public:

		virtual void InitRHI( )
		{
			VertexFormat Parameter;
			Parameter.Add( 0, VDT_Float3, VU_Position );
			Parameter.Add( 0, VDT_Float2, VU_TexCoord);
			Parameter.Add( 0, VDT_Color, VU_Diffuse);

			mVertexFormatRHI = RHI->CreateVertexFormat( Parameter );
		}

		virtual void ReleaseRHI( )
		{
			mVertexFormatRHI.Release();
		}

	public:
		RHIVertexFormatRef mVertexFormatRHI;

	};

	BatchedPointVertexFormat GBatchedPointVertexFormat;


	//////////////////////////////////////////////////////////////////////////
	// ElementVertexShader
	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_RTTI_CLASS( ElementVertexShader);

	void ElementVertexShader::BindShaderParameter( eShaderType ShaderType, const ShaderParameterMap& ParameterMap )
	{
		mTransform.Bind( ParameterMap, "Transform" );
	}

	void ElementVertexShader::SetParameter( const Matrix4& mat )
	{
		mShaderRHI->SetContantParameter( mTransform, mat );
	}

	void ElementVertexShader::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		Ser << mTransform;
	}


	//////////////////////////////////////////////////////////////////////////
	// ElementPixelShader
	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_RTTI_CLASS( ElementPixelShader);

	void ElementPixelShader::BindShaderParameter( eShaderType ShaderType, const ShaderParameterMap& ParameterMap )
	{
		mTexture.Bind( ParameterMap, "Texture" );
	}

	void ElementPixelShader::SetTexture( RenderTexture* NewTexture )
	{
		if ( mShaderRHI.Valid() )
			mShaderRHI->SetTextureParameter( mTexture, NewTexture );
	}

	void ElementPixelShader::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		Ser << mTexture;
	}

	//////////////////////////////////////////////////////////////////////////
	// BatchedElements
	//////////////////////////////////////////////////////////////////////////

	BatchedElements::BatchedElements(  )
	{

	}

	void BatchedElements::StaticInit( )
	{
		GBatchedPointVertexFormat.BeginInitResource( );
	}


	void SetBlendState(eBlendMode BlendMode)
	{
		switch(BlendMode)
		{
		case BLEND_Opaque:
			TStaticBlendState<>::Commit();
			break;
		case BLEND_Masked:
			TStaticBlendState<BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,CF_GreaterEqual,128>::Commit();
			break;
		case BLEND_Translucent:
			//TStaticBlendState<BO_Add,BF_SourceAlpha,BF_InverseSourceAlpha,BO_Add,BF_Zero,BF_One>::Commit();
			TStaticBlendState<BO_Add,BF_SourceAlpha,BF_InverseSourceAlpha,BO_Add,BF_Zero,BF_One>::Commit();
			break;
		case BLEND_Additive:
			TStaticBlendState<BO_Add,BF_One,BF_One,BO_Add,BF_Zero,BF_One>::Commit();
			break;
		case BLEND_Modulate:
			TStaticBlendState<BO_Add,BF_DestColor,BF_Zero,BO_Add,BF_Zero,BF_One>::Commit();
			break;
		}
	}


	void BatchedElements::AddLine( const Vector3& P1, const Vector3& P2, const Color& C )
	{
		mBatchedLineBuffer.push_advance( SimpleElementVertex( P1, C ) );
		mBatchedLineBuffer.push_advance( SimpleElementVertex( P2, C ) );	
	}

	void BatchedElements::AddLine( const Vector3& P1, const Vector3& P2, const Color& C1, const Color& C2 )
	{
		mBatchedLineBuffer.push_advance( SimpleElementVertex( P1, C1 ) );
		mBatchedLineBuffer.push_advance( SimpleElementVertex( P2, C2 ) );	
	}

	int BatchedElements::AddTriangleVertex( const Vector3& Pos,const Vector2& UV, const Color& C  )
	{
		int VertexIndex = mBatchedMeshVertexBuffer.size();
		mBatchedMeshVertexBuffer.push_advance( SimpleElementVertex( Pos,UV, C ) );
		return VertexIndex;
	}

	void BatchedElements::AddTriangleIndex( int V0, int V1, int V2, RenderTexture* TextureRes, eBlendMode BlendMode  )
	{
		dword MaxIndicesAllowed = Math::cMaxInt / sizeof(int);
		
		int MaxVerticesAllowed = Math::cMaxInt / sizeof(SimpleElementVertex);

		BatchedMeshElement* SelectedElements = null;

		for ( dword i = 0;i< mBatchedMeshIndexBuffer.size();i++)
		{
			BatchedMeshElement* CurElements = mBatchedMeshIndexBuffer.at( i );
			
			if ( CurElements->mTexture == TextureRes &&
				 CurElements->mBlendMode == BlendMode &&
				 CurElements->mIndexData.size() + 3 < MaxIndicesAllowed )
			{
				int DeltaV0 = (V0 - (int)CurElements->mMinVertex);
				int DeltaV1 = (V1 - (int)CurElements->mMinVertex);
				int DeltaV2 = (V2 - (int)CurElements->mMinVertex);
				if( DeltaV0 >= 0 && DeltaV0 < MaxVerticesAllowed &&
					DeltaV1 >= 0 && DeltaV1 < MaxVerticesAllowed &&
					DeltaV2 >= 0 && DeltaV2 < MaxVerticesAllowed )
				{
					SelectedElements = CurElements;
					break;
				}	
			}
		}


		if(!SelectedElements)
		{
			// make sure that vertex indices are close enough to fit within MaxVerticesAllowed
			if( Math::Abs(V0 - V1) >= MaxVerticesAllowed ||
				Math::Abs(V0 - V2) >= MaxVerticesAllowed )
			{
				Assert("Omitting BatchedElements::AddTriangle due to sparce vertices");
			}
			else
			{
				// Create a new mesh element for the texture if this is the first triangle encountered using it.
				SelectedElements = mBatchedMeshIndexBuffer.push_advance( );

				SelectedElements->mTexture = TextureRes;

				SelectedElements->mBlendMode = BlendMode;
				
				SelectedElements->mMaxVertex = V0;
				// keep track of the min vertex index used
				SelectedElements->mMinVertex = Math::Min(Math::Min(V0,V1),V2);
			}
		}

		if( SelectedElements )
		{
			// Add the triangle's indices to the mesh element's index array.
			SelectedElements->mIndexData.push_advance(V0 - SelectedElements->mMinVertex);
			SelectedElements->mIndexData.push_advance(V1 - SelectedElements->mMinVertex);
			SelectedElements->mIndexData.push_advance(V2 - SelectedElements->mMinVertex);

			// keep track of max vertex used in this mesh batch
			SelectedElements->mMaxVertex = Math::Max(Math::Max(Math::Max(V0,(int)SelectedElements->mMaxVertex),V1),V2);
		}
	}

	void BatchedElements::Draw( Matrix4& Transform )
	{
		TStaticRasterizerState<FM_Solid,CM_None>::Commit();
		

		GlobalShaderRef<ElementVertexShader> VertexShaderRef;
		VertexShaderRef->SetParameter( Transform.Transpose() );

		GBatchedPointVertexFormat.mVertexFormatRHI->Commit();

		VertexShaderRef->Commit();

		GlobalShaderRef<ElementPixelShader> PixelShaderRef;

		if ( mBatchedLineBuffer.size() > 0 )
		{
			// 线段如果带Alpha层，可以绘制正确
			SetBlendState( BLEND_Translucent );

			PixelShaderRef->SetTexture( GWhiteTexture );
			PixelShaderRef->Commit();

			RHI->DrawUserBuffer( PT_LineList, mBatchedLineBuffer.size()/2, mBatchedLineBuffer.getdata(), sizeof( SimpleElementVertex ) );

			mBatchedLineBuffer.reset();
		}

		for ( dword i = 0;i< mBatchedMeshIndexBuffer.size();i++)
		{
			BatchedMeshElement* CurElements = mBatchedMeshIndexBuffer.at( i );

			PixelShaderRef->SetTexture( CurElements->mTexture);
			PixelShaderRef->Commit();
			SetBlendState( CurElements->mBlendMode );

			RHI->DrawIndexedUserBuffer(	PT_TriangleList, 
				CurElements->mIndexData.size() / 3,
				CurElements->mMaxVertex - CurElements->mMinVertex + 1,
				mBatchedMeshVertexBuffer.at(CurElements->mMinVertex), 
				sizeof( SimpleElementVertex ),
				CurElements->mIndexData.getdata(), 
				sizeof( int ) );
		}

		mBatchedMeshIndexBuffer.reset( );
		mBatchedMeshVertexBuffer.reset();

	}

}