#include "EnginePCH.h"
#include "NekoEngine_Model.h"

namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS( MeshVertexFactory )

	MeshVertexFactory::MeshVertexFactory(   )
		: mMaterialIndex( -1 )
		, mVertexCount( 0 )
		, mIndexCount( 0 )
		, mVertexStream( null )
		, mIndexStream( null )
		, mFormatData( null )
		, mCachedAABB( AABB::cNullBox )
	{

	}

	MeshVertexFactory::~MeshVertexFactory()
	{
		delete mIndexStream;

		delete mVertexStream;
	}

	void MeshVertexFactory::Serialize( BinarySerializer& Ser )
	{
		Ser << mName 
			<< mMaterialIndex
			<< mVertexCount
			<< mIndexCount
			<< mIndexStream
			<< mVertexStream;


		//if ( Ser.IsLoading() )
		//{
		//	struct VertexStruct
		//	{
		//		Vector3 mPosition;
		//		Vector3 mNormal;
		//		Vector2 mTexCoord;
		//		Vector3 mTangent;
		//		Vector3 mBinormal;
		//	};

		//	VertexStruct* VS = (VertexStruct*)mVertexStream->GetBuffer();
		//	word* IS = (word*)mIndexStream->GetBuffer();
		//	int a = 1;
		//}
	}

	AABB MeshVertexFactory::GetStaticBoundingBox( )
	{
		if ( mCachedAABB != AABB::cNullBox )
			return mCachedAABB;

		dword Stride = mFormatData->GetTotalStride( 0 );

		dword PositionOffset = mFormatData->GetElementOffset( VU_Position );

		if ( PositionOffset == -1 )
			return mCachedAABB;

		mCachedAABB = AABB::cInitBox;

		dword ReadOffset = 0;
		while ( ReadOffset < mVertexStream->GetSize() )
		{
			Vector3* PositionData = (Vector3*)( mVertexStream->GetBuffer() + PositionOffset + ReadOffset );

			Assert( PositionData != null );

			mCachedAABB += *PositionData;

			ReadOffset+= Stride;
		}

		return mCachedAABB;
	}

	void MeshVertexFactory::InitData( const WString& Name, dword VertexCount, HeapStream* VertexStream, dword IndexCount, HeapStream* IndexStream, dword MaterialIndex )
	{
		mName = Name;
		mVertexCount = VertexCount;
		mVertexStream = VertexStream;
		mIndexStream = IndexStream;
		mIndexCount = IndexCount;
		mMaterialIndex = MaterialIndex;
	}

	IMPLEMENT_RTTI_CLASS(MeshResource)

	MeshResource::MeshResource()
	{

	}

	MeshResource::~MeshResource()
	{
		// 显示数据
		for ( dword i = 0;i < mVertexFactoryList.size(); i++ )
		{
			MeshVertexFactory* Factory = mVertexFactoryList[i];
			delete Factory;
		}

		mVertexFactoryList.clear();
	}

	void MeshResource::Serialize( BinarySerializer& Ser )
	{
		Ser << mVertexFormat
			<< mVertexFactoryList;

		// 绑定VF数据
		if ( Ser.IsLoading() )
		{
			for ( dword i = 0;i < mVertexFactoryList.size(); i++ )
			{
				MeshVertexFactory* Factory = mVertexFactoryList[i];
				Factory->mFormatData = &mVertexFormat;
			}
		}
	}


	void MeshResource::InitializeVertexFactory( )
	{
		for ( dword i = 0;i < mVertexFactoryList.size(); i++ )
		{
			mVertexFactoryList[i]->BeginInitResource( );
		}

		// 同步信号,等待这个渲染指令被执行后即完成
		mReadyFence.BeginFence();
	}

	void MeshResource::BeginDispose( )
	{
		for (VertexFactoryList::iterator it = mVertexFactoryList.begin();
			it != mVertexFactoryList.end();
			++it )
		{
			VertexFactory* Factory = *it;
			Factory->BeginReleaseResource( );
		}

		mReleaseFence.BeginFence();
	}


	dword MeshResource::AddVertexFactory( MeshVertexFactory* Content )
	{
		mVertexFactoryList.push_back( Content );

		return mVertexFactoryList.size() - 1;
	}


	dword MeshResource::GetVertexFactoryCount()
	{
		return mVertexFactoryList.size();
	}

	MeshVertexFactory* MeshResource::GetVertexFactory( dword Index )
	{
		return mVertexFactoryList[ Index ];
	}

	void MeshResource::PostLoad()
	{
		InitializeVertexFactory();
	}

	bool MeshResource::IsManualResourceReady()
	{
		return mReadyFence.GetNumPendingFences() == 0;
	}

	bool MeshResource::CanDestroyNow()
	{
		return mStreamingFence.GetNumPendingFences() == 0 &&
			   mReleaseFence.GetNumPendingFences() == 0;
	}

	AABB MeshResource::GetBoundingBox(  )
	{
		AABB BoundingBox = AABB::cInitBox;
		for ( dword i = 0; i< GetVertexFactoryCount();i ++)
		{
			MeshVertexFactory* Factory = GetVertexFactory( i );
			BoundingBox += Factory->GetStaticBoundingBox(  );
		}

		return BoundingBox;
	}

}