#include "EnginePCH.h"
#include "NekoEngine_Render.h"
#include "Engine/RenderFont.h"

namespace NekoEngine
{




	Canvas::Canvas( const Viewport& vp )
		: mBatchedElements( null )
		, mTexture( null )
		, mBlendMode( BLEND_MAX )
		, mElementType( ET_MAX )
	{
		mViewportWidth = (float)vp.Width;
		mViewportHeight = (float)vp.Height;

		mTransformStack.push_back( Matrix4().Scale( 1.0f, -1.0f, 1.0f ) * 
			Matrix4().Translation(  -0.5f, mViewportHeight +0.5f, 0.0f ) *
			Matrix4::OrthoOffCenterLH( 0.0f, mViewportWidth, 0.0f, mViewportHeight, vp.NearZ, vp.FarZ ) );
	}




	Canvas::~Canvas()
	{
		delete mBatchedElements;

		mTransformStack.clear();
	}

	BatchedElements* Canvas::GetBatcher( eElementType ElementType, RenderTexture* InTexture, eBlendMode BlendMode )
	{
		if ( mBatchedElements )
		{
			bool NeedFlush = false;
			if ( ElementType != ET_MAX && ElementType != mElementType )
			{
				NeedFlush = true;
			}
			else if ( mTexture && InTexture && mTexture != InTexture )
			{
				NeedFlush = true;
			}
			else if ( BlendMode != BLEND_MAX && mBlendMode!= BLEND_MAX && mBlendMode != BlendMode )
			{
				NeedFlush = true;
			}

			if ( NeedFlush )
			{
				Flush();

				mBatchedElements = new BatchedElements;
			}
		}
		else
		{
			mBatchedElements = new BatchedElements;
		}


		mTexture = InTexture;
		mBlendMode = BlendMode;
		mElementType = ElementType;

		return mBatchedElements;
	}

	void Canvas::DrawLine2D( const Vector2& P1, const Vector2& P2, const Color& C )
	{
		GetBatcher( ET_Line )->AddLine( Vector3( P1.x, P1.y, 0.5f), Vector3( P2.x, P2.y, 0.5f ), C );
	}

	void Canvas::DrawRect2D( const Vector2& P1, const Vector2& P2, const Color& C )
	{
		DrawLine2D( Vector2( P1.x, P1.y ), Vector2( P2.x, P1.y ), C );
		DrawLine2D( Vector2( P2.x, P1.y ), Vector2( P2.x, P2.y ), C );
		DrawLine2D( Vector2( P2.x, P2.y ), Vector2( P1.x, P2.y ), C );
		DrawLine2D( Vector2( P1.x, P2.y ), Vector2( P1.x- 1.0f, P1.y - 1.0f  ), C );
	}

	void Canvas::DrawLine( const Vector3& P1, const Vector3& P2, const Color& C )
	{
		GetBatcher( ET_Line )->AddLine( P1, P2 , C );
	}

	void Canvas::DrawLine( const Vector3& P1, const Vector3& P2, const Color& C1, const Color& C2 )
	{
		GetBatcher( ET_Line )->AddLine( P1, P2 , C1, C2 );
	}

	void Canvas::DrawTile( 
		const Vector2& Pos, 
		const Vector2& Size, 
		const Vector2& UV, 
		const Vector2& SizeUV, 
		const Color& C, 
		RenderTexture* InTexture, 
		eBlendMode Mode )
	{
		BatchedElements* Batcher = GetBatcher( ET_Triangle, InTexture, Mode );
		

		float U = 0.0f;
		float V = 0.0f;
		float SizeU = 1.0f;
		float SizeV = 1.0f;

		int V00 = Batcher->AddTriangleVertex(Vector3(Pos.x,				Pos.y,				0.5f)	,Vector2(UV.x,				UV.y			),	C);
		int V10 = Batcher->AddTriangleVertex(Vector3(Pos.x + Size.x,	Pos.y,				0.5f)	,Vector2(UV.x + SizeUV.x,	UV.y			),	C);
		int V01 = Batcher->AddTriangleVertex(Vector3(Pos.x,				Pos.y + Size.y,		0.5f)	,Vector2(UV.x,				UV.y + SizeUV.y	),	C);
		int V11 = Batcher->AddTriangleVertex(Vector3(Pos.x + Size.x,	Pos.y + Size.y,		0.5f)	,Vector2(UV.x + SizeUV.x,	UV.y + SizeUV.y	),	C);


		Batcher->AddTriangleIndex(V00,V10,V11,InTexture, Mode );
		Batcher->AddTriangleIndex(V00,V11,V01,InTexture, Mode );
	}

	void Canvas::DrawString( RenderFont* Font, const Vector2& Pos, const Color& C, const wchar* Text )
	{
		if ( Font == null || Text == null )
			return;

		

		dword LineX = 0;

		for ( dword i = 0; Text[i]; i++)
		{
			wchar Char = Text[i];

			Vector2 uv, uvsize, charsize;
			RenderFontTexture* Texture = Font->GetFontTexture( Char, uv, uvsize, charsize );
			if ( Texture == null )
				continue;

			Vector2 FinalPos( LineX + Pos.x , Pos.y );
			

			DrawTile(	FinalPos, 
						charsize, 
						uv, 
						uvsize,
						C,
						Texture,
						BLEND_Translucent );

			LineX += (dword)charsize.x;
		}
	}


	//void Canvas::DrawText( int X, int Y, dword FontStyle,const wchar* Format,... )
	//{
	//	struct DrawTextParameter 
	//	{
	//		int X, Y;
	//		dword Color;
	//		wchar CombinedString[ 256 ];
	//	};

	//	DrawTextParameter thisParameter = {
	//		X, Y,
	//		FontStyle,
	//	};

	//	StringHelper::FormatBufferV( thisParameter.CombinedString, 256, Format, (char*)( &Format ) + sizeof( Format ) );

	//	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(DrawTextFaked, DrawTextParameter,mTextParameter, thisParameter, { 

	//		RHI->DrawDebugText( mTextParameter.X, mTextParameter.Y, mTextParameter.Color, mTextParameter.CombinedString );

	//	} );


	//}

	void Canvas::PushTransform( const Matrix4& Transform )
	{
		Flush();

		mTransformStack.push_back( Transform );
	}

	const Matrix4& Canvas::GetFullTransform()
	{
		return mTransformStack.back();
	}

	void Canvas::PopTransform( )
	{
		Flush();

		mTransformStack.pop_back();
	}

	void Canvas::Flush( )
	{
		if ( mBatchedElements )
		{
			ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(BatchedDraw, 
				BatchedElements*,mBatchedElements, mBatchedElements,
				Matrix4, mTransform, GetFullTransform(),
			{ 

				mBatchedElements->Draw( mTransform );
				delete mBatchedElements;
			} );
		}

	
		mBatchedElements = null;
		mTexture = null;
		mBlendMode = BLEND_MAX;
		mElementType = ET_MAX;
	}



}