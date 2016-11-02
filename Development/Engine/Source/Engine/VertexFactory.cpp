#include "EnginePCH.h"
#include "NekoEngine_Model.h"


namespace NekoEngine
{
	VertexFactory::VertexFactory( )
		: mVertexFormatRHI( null )
		, mVertexBufferRHI( null )
		, mIndexStreamRHI( null )
		, mStreamCount( 0 )
	{

	}


	void VertexFactory::InitRHI()
	{	
		__super::InitRHI( );

		{
			DataStream* SrcData = GetVertexStream();
			mVertexBufferRHI = RHI->CreateVertexBuffer( SrcData->GetSize(), false );

			void* Data = mVertexBufferRHI->Lock( 0, SrcData->GetSize() );

			::memcpy( Data, SrcData->GetBuffer(), SrcData->GetSize() );

			mVertexBufferRHI->Unlock();
		}



		// 创建顶点定义
		mVertexFormatRHI = RHI->CreateVertexFormat( *GetFormatData() );
		
		{
			DataStream* SrcData = GetIndexStream();
			// 创建索引缓冲
			mIndexStreamRHI = RHI->CreateIndexBuffer( SrcData->GetSize(), sizeof(word) , false );
			void* Data = mIndexStreamRHI->Lock( 0, SrcData->GetSize() );
			::memcpy( Data, SrcData->GetBuffer(), SrcData->GetSize() );
			mIndexStreamRHI->Unlock();
		}

	}


	void VertexFactory::Commit( )
	{
		mVertexFormatRHI->Commit();
		mVertexBufferRHI->Commit( 0, GetFormatData()->GetTotalStride( 0 ) );
		mIndexStreamRHI->Commit();
	}



	void VertexFactory::ReleaseRHI()
	{
		mIndexStreamRHI.Release( );

		mVertexFormatRHI.Release();

		mVertexBufferRHI.Release();
		
		__super::ReleaseRHI();
	}

	VertexFactory::~VertexFactory()
	{

	}
}