#include "EngineDemoPCH.h"

//=====================================
//TriangleVB
//=====================================

TriangleVB::TriangleVB()
{
	mVertexFormat.Append( VDT_Float4, VU_PositionRHW );
	mVertexFormat.Append( VDT_Color, VU_Diffuse );

	mVB.push_back( Vertex2D( Vector3( 150.0f,  50.0f,0.5f ), 0xffff0000  ) );
	mVB.push_back( Vertex2D( Vector3( 250.0f, 250.0f,0.5f),0xff00ff00  ) );
	mVB.push_back( Vertex2D( Vector3( 50.0f, 250.0f,0.5f), 0xff00ffff  ) );
}

NekoEngine::dword TriangleVB::GetStride()
{
	return sizeof( Vertex2D );
}

VertexFormat* TriangleVB::GetVertexFormat()
{
	return &mVertexFormat;
}

NekoEngine::dword TriangleVB::GetSize()
{
	return GetStride() * mVB.size();
}

void TriangleVB::GetData( void* data )
{
	::memcpy( data, &mVB[0], GetSize() );
}

void TriangleVB::Render( IGenericRender* render )
{
	render->Draw( PT_TriangleList, &mVertexFormat, this, null, null );
}



//=====================================
//QuadIB
//=====================================
NekoEngine::dword QuadIB::GetSize()
{
	return 6 * sizeof( word );
}

void QuadIB::GetData( void* data )
{
	static word ilist[] = { 0,1,2,0,2,3 };
	::memcpy(data, ilist, GetSize() );
}


//=====================================
//QuadVB
//=====================================
QuadVB::QuadVB()
{
	mVertexFormat.Append( VDT_Float4, VU_PositionRHW );
	mVertexFormat.Append( VDT_Color, VU_Diffuse );
	mVertexFormat.Append( VDT_Float2, VU_TexCoord );

	float size = 200.0f;

	float y = 100.0f;

	mTexture1.SetName(L"tex\\test.bmp");
	mTexture2.SetName(L"tex\\default.png");

	mEffect.mShaderTexture1.SetTexture( &mTexture1 );
	mEffect.mShaderTexture2.SetTexture( &mTexture2 );

	mVB.push_back( Vertex2D( Vector3( 50.5f			, y		,0.5f )	, -1	, Vector2(0,0)  ) );
	mVB.push_back( Vertex2D( Vector3( size + 50.5f	, y			,0.5f)	,-1		, Vector2(1,0)  ) );
	mVB.push_back( Vertex2D( Vector3( size + 50.5f	, size + y	,0.5f)	, -1	, Vector2(1,1)  ) );
	mVB.push_back( Vertex2D( Vector3( 50.5f			, size + y	,0.5f)	, -1	, Vector2(0,1)  ) );
}

VertexFormat* QuadVB::GetVertexFormat()
{
	return &mVertexFormat;
}

NekoEngine::dword QuadVB::GetStride()
{
	return sizeof( Vertex2D );
}

NekoEngine::dword QuadVB::GetSize()
{
	return GetStride() * mVB.size();
}

void QuadVB::GetData( void* data )
{
	::memcpy( data, &mVB[0], GetSize() );
}

void QuadVB::Render( IGenericRender* render )
{
	mEffect.mParameterColor.SetColor(Color(1.0f, 1.0f, 0.0f,1.0f));
	
	render->Draw( PT_TriangleList, &mVertexFormat, this, &mIB, &mEffect );
}


IMPLEMENT_SCENELISTENERBJECT(Demo_Primitive2D)


void Demo_Primitive2D::OnRender( RenderContext* rc  )
{
	mTriangleVB.Render( rc->GetGenericRender() );
	mQuadVB.Render( rc->GetGenericRender() );
}

void Demo_Primitive2D::OnEnter()
{
	
}

void Demo_Primitive2D::OnLeave()
{
	GRenderDriver->ManualUncache( &mTriangleVB.mVertexFormat );
	GRenderDriver->ManualUncache( &mTriangleVB );
	GRenderDriver->ManualUncache( &mQuadVB );
	GRenderDriver->ManualUncache( &mQuadVB.mIB );
	GRenderDriver->ManualUncache( &mQuadVB.mEffect );
	GRenderDriver->ManualUncache( &mQuadVB.mVertexFormat );
	GRenderDriver->ManualUncache( &mQuadVB.mTexture1 );
	GRenderDriver->ManualUncache( &mQuadVB.mTexture2 );
}