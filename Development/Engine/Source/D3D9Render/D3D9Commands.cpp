#include "D3D9PCH.h"
#pragma warning( disable: 4748 )

namespace NekoEngine
{
	int GDrawUPVertexCheckCount = MAXINT;	

	void D3D9RHI::Clear(bool bClearColor,dword c,bool bClearDepth,float Depth,bool bClearStencil,dword Stencil)
	{
		Assert( IsInRenderingThread() );

		// Determine the clear flags.
		DWORD Flags = 0;
		if(bClearColor)
		{
			Flags |= D3DCLEAR_TARGET;
		}
		if(bClearDepth)
		{
			Flags |= D3DCLEAR_ZBUFFER;
		}
		if(bClearStencil)
		{
			Flags |= D3DCLEAR_STENCIL;
		}

		// Clear the render target/depth-stencil surfaces based on the flags.
		GDirect3DDevice->Clear(0, NULL, Flags, c, Depth, Stencil);
	}

	ID3DXFont* GDebugFont = null;

	void D3D9RHI::DrawDebugText(int x, int y, dword color,const wchar* format,...)
	{
		Assert( IsInRenderingThread() );

		if ( !GDebugFont )
		{
			const int FontSize = 9;
			const bool IsBold = false;
			const wchar* FontName = L"Arial";

			HRESULT hr = D3DXCreateFont( GDirect3DDevice
										, - MulDiv( FontSize, 96, 72 )
										, 0
										, IsBold ? FW_BOLD:FW_NORMAL
										, 1
										, FALSE
										, DEFAULT_CHARSET
										, OUT_DEFAULT_PRECIS
										, DEFAULT_QUALITY
										, DEFAULT_PITCH | FF_DONTCARE
										, FontName , &GDebugFont );
		}

		if ( !GDebugFont )
			return;

		Rect rcText;

		rcText.right	= 0;
		rcText.bottom	= 0;
		rcText.left		= x;
		rcText.top		= y;

		wchar string[ 1024 ];
		StringHelper::FormatBufferV( string, 1024, format, (char*)( &format ) + sizeof( format ) );

		GDebugFont->DrawText( null, string, -1, (RECT*)&rcText, DT_NOCLIP ,color );
	}

	RenderStatus GRenderStatus;

	
	static dword GetPrimitiveCount( ePrimitiveTopology pt, dword vertexcount )
	{
		switch( pt )
		{
		case PT_PointList:
			return vertexcount;
		case PT_LineList:
			return vertexcount /2;
		case PT_LineStrip:
			return vertexcount - 1;
		case PT_TriangleList:
			return vertexcount / 3;
		case PT_TriangleStrip:
			return vertexcount - 2;			
		}

		return 0;
	}

	static dword GetIndexCount( ePrimitiveTopology pt, dword NumPrimitives )
	{
		switch( pt )
		{
		case PT_PointList:
			return NumPrimitives;
		case PT_LineList:
			return NumPrimitives * 2;
		case PT_LineStrip:
			return NumPrimitives + 1;
		case PT_TriangleList:
			return NumPrimitives * 3;
		case PT_TriangleStrip:
			return NumPrimitives + 2;			
		}

		return 0;
	}

	
	void CalcRenderResult( ePrimitiveTopology pt, dword NumPrimitives )
	{
		switch( pt )
		{
		case PT_PointList:
			GRenderStatus.mNumVertices += NumPrimitives *3;
			break;
		case PT_LineList:
			GRenderStatus.mNumVertices += NumPrimitives *3;
			break;
		case PT_LineStrip:
			GRenderStatus.mNumVertices += NumPrimitives + 1;
			break;
		case PT_TriangleList:
			GRenderStatus.mNumPrimitives += NumPrimitives;
			GRenderStatus.mNumVertices += NumPrimitives *3;
			break;
		case PT_TriangleStrip:
			GRenderStatus.mNumPrimitives += NumPrimitives;
			GRenderStatus.mNumVertices += NumPrimitives + 2;
			break;
		}
	}


	void D3D9RHI::Draw( ePrimitiveTopology PrimitiveType, RHIIndexBuffer* IndexBuffer, dword NumVertices )
	{
		Assert( IsInRenderingThread() );

		dword NumPrimitives;

		if( IndexBuffer != null )
		{
			NumPrimitives = GetPrimitiveCount( PrimitiveType, IndexBuffer->GetSize() / sizeof( word ) );
			VD3DHR(GDirect3DDevice->DrawIndexedPrimitive( 
				(D3DPRIMITIVETYPE)PrimitiveType,
				0,
				0,
				NumVertices,
				0,
				NumPrimitives
				) );
		}
		else
		{
			NumPrimitives = GetPrimitiveCount( PrimitiveType, NumVertices );
			VD3DHR(GDirect3DDevice->DrawPrimitive(
				(D3DPRIMITIVETYPE)PrimitiveType,
				0,
				NumPrimitives
				) );
		}


		CalcRenderResult( PrimitiveType, NumPrimitives );
	}

	void D3D9RHI::DrawIndexedUserBuffer( 
							ePrimitiveTopology PrimitiveType, 
							dword NumPrimitives,
							dword NumVertices,
							const void* UserVertexStream,
							dword VertexStreamStride, 
							const void* UserIndexBuffer,
							dword IndexStreamStride )
	{

		Assert( IsInRenderingThread() );

		VD3DHR(GDirect3DDevice->DrawIndexedPrimitiveUP( 
			(D3DPRIMITIVETYPE)PrimitiveType,
			0,	
			NumVertices,   // NumVertices
			NumPrimitives,  // PrimitiveCount
			UserIndexBuffer,
			(IndexStreamStride == sizeof(dword) )? D3DFMT_INDEX32: D3DFMT_INDEX16,
			UserVertexStream,
			VertexStreamStride
			) );

		CalcRenderResult( PrimitiveType, NumPrimitives );
	}

	void D3D9RHI::DrawUserBuffer( 
		ePrimitiveTopology PrimitiveType, 
		dword NumPrimitives,
		const void* UserVertexStream,
		dword VertexStreamStride )
	{
		Assert( IsInRenderingThread() );

		VD3DHR(GDirect3DDevice->DrawPrimitiveUP(
			(D3DPRIMITIVETYPE)PrimitiveType,
			NumPrimitives,
			UserVertexStream,
			VertexStreamStride
			) );

		CalcRenderResult( PrimitiveType, NumPrimitives );
	}


	void D3D9RHI::SetDrawArea( dword X, dword Y, dword Width, dword Height )
	{
		Assert( IsInRenderingThread() );
		D3DVIEWPORT9 VP = { X, Y, Width, Height, 0.0f, 1.0f };// D3D的Viewport不使用这里的NearZ,FarZ，恒定0到1
		
		GDirect3DDevice->SetViewport( &VP );
	}

	void D3D9RHI::RestoreRenderTarget( dword Index )
	{
		Assert( IsInRenderingThread() );

		GDirect3DDevice->SetRenderTarget( Index, GD3DBackBuffer );		
	}

	void D3D9RHI::BeginDraw( )
	{
		GRenderStatus.mNumPrimitives = 0;
		GRenderStatus.mNumVertices = 0;

		Assert( IsInRenderingThread() );

		VD3DHR(GDirect3DDevice->BeginScene());
	}

	void D3D9RHI::EndDraw( )
	{
		Assert( IsInRenderingThread() );

		for(dword TextureIndex = 0;TextureIndex < 16;TextureIndex++)
		{
			VD3DHR(GDirect3DDevice->SetTexture(TextureIndex,NULL));
		}

		VD3DHR(GDirect3DDevice->SetVertexShader(NULL));

		for(dword StreamIndex = 0;StreamIndex < 16;StreamIndex++)
		{
			VD3DHR(GDirect3DDevice->SetStreamSource(StreamIndex,NULL,0,0));
		}

		VD3DHR(GDirect3DDevice->SetIndices(NULL));
		VD3DHR(GDirect3DDevice->SetPixelShader(NULL));

		// Tell D3D we're done rendering.
		VD3DHR(GDirect3DDevice->EndScene());
	}


}