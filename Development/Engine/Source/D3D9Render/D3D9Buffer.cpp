#include "D3D9PCH.h"


namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// D3D9VertexBuffer
	//////////////////////////////////////////////////////////////////////////

	class D3D9VertexBuffer : public RHIVertexBuffer
	{
	public:
		D3D9VertexBuffer(  )
			: mDirect3DVertexBuffer9( null )
		{

		}

		virtual ~D3D9VertexBuffer( )
		{
			SafeRelease( mDirect3DVertexBuffer9 );
		}

		virtual void Commit( dword StreamIndex, dword Stride )
		{
			Assert( IsInRenderingThread() );

			VD3DHR(GDirect3DDevice->SetStreamSource( StreamIndex, mDirect3DVertexBuffer9, 0, Stride ) );
		}

		virtual void* Lock( dword Offset, dword Size )
		{
			void* Data = NULL;
			VD3DHR( mDirect3DVertexBuffer9->Lock( Offset, Size, &Data, mIsDynamic ? D3DLOCK_DISCARD : 0 ) );

			
			return Data;
		}

		virtual dword GetSize( )
		{
			D3DVERTEXBUFFER_DESC Desc;
			mDirect3DVertexBuffer9->GetDesc( &Desc );
			return Desc.Size;
		}

		virtual void Unlock( )
		{
			VD3DHR( mDirect3DVertexBuffer9->Unlock( ) );
		}

	public:
		IDirect3DVertexBuffer9* mDirect3DVertexBuffer9;
		bool mIsDynamic;
	};

	RHIVertexBufferRef D3D9RHI::CreateVertexBuffer(dword Size,bool IsDynamic)
	{
		Assert(Size > 0);

		Assert( IsInRenderingThread() );

		D3D9VertexBuffer* NewBuffer = new D3D9VertexBuffer;
		
		NewBuffer->mIsDynamic = IsDynamic;
		const DWORD Usage = IsDynamic ? (D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY) : 0;
		const D3DPOOL Pool = IsDynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;

		// Create the vertex buffer.
		VD3DHR(GDirect3DDevice->CreateVertexBuffer(Size,Usage,0,Pool,&NewBuffer->mDirect3DVertexBuffer9,NULL));

		return NewBuffer;
	}

	//////////////////////////////////////////////////////////////////////////
	// D3D9IndexBuffer
	//////////////////////////////////////////////////////////////////////////


	class D3D9IndexBuffer : public RHIIndexBuffer
	{
	public:
		D3D9IndexBuffer(  )
			: mDirect3DIndexBuffer9( null )
		{

		}

		virtual ~D3D9IndexBuffer( )
		{
			SafeRelease( mDirect3DIndexBuffer9 );
		}

		virtual void Commit( )
		{
			Assert( IsInRenderingThread() );

			VD3DHR(GDirect3DDevice->SetIndices( mDirect3DIndexBuffer9 ) );
		}

		virtual void* Lock( dword Offset, dword Size )
		{
			void* Data = NULL;
			VD3DHR( mDirect3DIndexBuffer9->Lock( Offset, Size, &Data, mIsDynamic ? D3DLOCK_DISCARD : 0 ) );

			return Data;
		}

		virtual dword GetSize( )
		{
			D3DINDEXBUFFER_DESC Desc;
			mDirect3DIndexBuffer9->GetDesc( &Desc );
			return Desc.Size;
		}

		virtual void Unlock( )
		{
			VD3DHR( mDirect3DIndexBuffer9->Unlock( ) );
		}

	public:
		IDirect3DIndexBuffer9* mDirect3DIndexBuffer9;
		bool mIsDynamic;
	};

	RHIIndexBufferRef D3D9RHI::CreateIndexBuffer(dword Size,dword Stride, bool IsDynamic)
	{
		Assert(Size > 0);

		Assert( IsInRenderingThread() );

		D3D9IndexBuffer* NewBuffer = new D3D9IndexBuffer;

		NewBuffer->mIsDynamic = IsDynamic;
		const DWORD Usage = IsDynamic ? (D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY) : 0;
		const D3DPOOL Pool = IsDynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;
		const D3DFORMAT Format = (Stride == sizeof(WORD) ? D3DFMT_INDEX16 : D3DFMT_INDEX32);

		// Create the vertex buffer.
		VD3DHR(GDirect3DDevice->CreateIndexBuffer(Size,Usage,Format,Pool,&NewBuffer->mDirect3DIndexBuffer9,NULL));

		return NewBuffer;
	}
}