#include "CorePCH.h"
#include "NekoCore_Stream.h"
#include "NekoCore_Pack.h"

namespace NekoEngine
{
	const wchar ChunkListName[] = L"_chunklist_";

	// 文件头的标示
	const char PackFileTag[] = "NGP";
	const dword PackVersion = 2;

	//////////////////////////////////////////////////////////////////////////
	// PackStdHeader
	//////////////////////////////////////////////////////////////////////////
	void PackStdHeader::Init()
	{
		::strcpy(mTag,PackFileTag);
		mVersion = PackVersion;
	}


	bool PackStdHeader::Valid( )
	{
		return (!strcmp(mTag, PackFileTag) ) && ( mVersion == PackVersion );
	}

	//////////////////////////////////////////////////////////////////////////
	// ChunkName
	//////////////////////////////////////////////////////////////////////////

	NameSet::NameSet( dword A, dword B, dword C )
		: mSetA( A )
		, mSetB( B )
		, mSetC( C )
	{

	}

	NameSet::NameSet( const wchar* name )
	{
		GenHash( name );
	}

	void NameSet::GenHash( const wchar* name )
	{
		mSetA = CheckSum::BLZStrHash( name, 0 );
		mSetB = CheckSum::BLZStrHash( name, 1 );
		mSetC = CheckSum::BLZStrHash( name, 2 );
	}

	bool NameSet::operator==( const NameSet& other ) const
	{
		return ::memcmp( this, &other,sizeof( other ) ) == 0;
	}



	PackMaster::PackMaster()
		: mPackMasterStatus( PMS_Invalid )
		, mDataSource( null )
		, mSourceAutoDelete(false)
	{

	}

	PackMaster::~PackMaster()
	{
		Close();
	}


	bool PackMaster::Create( const wchar* packfile )
	{
		return ProcessCreate( new FileStream( packfile,FAM_Write ), true );
	}

	bool PackMaster::Create( DataStream* mem )
	{
		return ProcessCreate( mem, false );
	}

	bool PackMaster::ProcessCreate( DataStream* mem, bool autodelete )
	{
		Assert( mDataSource == null );

		mDataSource = mem;
		mSourceAutoDelete = autodelete;

		if ( !mDataSource->IsValid() )
			return false;

		PackStdHeader header;

		header.Init();

		mDataSource->WriteBuffer( &header, sizeof( PackStdHeader ) );

		mPackMasterStatus = PMS_WritePack;

		return true;
	}

	bool PackMaster::WriteChunk( const wchar* name, DataStream* stream, bool writeNameTable )
	{
		// 记录真实名字，便于遍历
		if ( writeNameTable )
		{
			NameSet Name( name );
			mNameTableList.push_back( ChunkName( Name, name ) );
			return WriteChunk( Name, stream );
		}

		return WriteChunk( NameSet( name ), stream );
	}

	bool PackMaster::WriteChunk( const NameSet& name, DataStream* stream )
	{
		Assert( mPackMasterStatus = PMS_WritePack );
		if ( mPackMasterStatus != PMS_WritePack )
			return false;

		ChunkSet set;
		set.mName = name;
		set.mSize = stream->GetSize();
		set.mOffset = mDataSource->GetPosition();
		set.mEncode = 0;

		Assert( set.mSize > 0 );


		return	mDataSource->WriteBuffer( &set, sizeof( ChunkSet ) ) &&
				mDataSource->WriteBuffer( stream );
	}

	const wchar* PackNameTableName = L"ngp_nametable";

	void PackMaster::WriteNameTable( )
	{
		HeapStream ss;

		for ( NameTableList::iterator it = mNameTableList.begin();
			it != mNameTableList.end();
			++it )
		{
			ChunkName& chunkname = *it;	
			ss.WriteBuffer( &chunkname.mNameSet, sizeof(NameSet));
			ss.WriteUNICODEString( chunkname.mRealName.c_str() );
		}

		WriteChunk( PackNameTableName , &ss, false );
	}

	bool PackMaster::ReadNameTable( )
	{
		if ( mNameTableList.size() > 0 )
			return true;

		HeapStream hs;
		if ( !ReadChunkByName( PackNameTableName, &hs ) )
			return false;

		while ( hs.GetRemainBytes() > 0 )
		{
			ChunkName chunkname;
			if ( !hs.ReadBuffer( &chunkname.mNameSet, sizeof(NameSet) ) )
				return false;

			chunkname.mRealName = hs.ReadUNICODEString();

			mNameTableList.push_back(chunkname);
		}

		return true;
	}

	bool PackMaster::Open( DataStream* mem )
	{
		return ProcessOpen( mem , false );
	}

	bool PackMaster::Open( const wchar* packfile )
	{
		return ProcessOpen( new FileStream( packfile,FAM_Read ), true );
	}

	bool PackMaster::ProcessOpen( DataStream* mem, bool autodelete )
	{
		Assert( mDataSource == null );

		mDataSource = mem;
		mSourceAutoDelete = autodelete;

		if ( !mDataSource->IsValid() )
			return false;

		PackStdHeader header;
		if ( !mDataSource->ReadBuffer( &header, sizeof( PackStdHeader ) ) )
			return false;

		if ( !header.Valid() )
			return false;

		mChunkMap.clear();

		// 缓冲chunset列表
		while( mDataSource->GetRemainBytes() > 0 )
		{
			ChunkSet set;
			if ( !mDataSource->ReadBuffer( &set, sizeof( set ) ) )
				return false;

			mChunkMap.set( set.mName, set );

			// 跨越到下一个chunkset
			mDataSource->Seek( set.mOffset + set.mSize + sizeof( ChunkSet ), MSO_Begin );
		}

		ReadNameTable();

		mPackMasterStatus = PMS_ReadPack;

		return true;
	}

	bool PackMaster::ReadChunk( const NameSet& hashname , DataStream* stream )
	{
		return ReadChunkRaw( mChunkMap.find( hashname ), stream );
	}

	bool PackMaster::ReadChunkByName( const wchar* name, DataStream* stream )
	{
		return ReadChunkRaw( mChunkMap.find( NameSet( name ) ), stream );
	}

	bool PackMaster::ChunkExist( const wchar* name )
	{
		return mChunkMap.find( NameSet( name ) ) != null;
	}

	bool PackMaster::ChunkExist( const NameSet& name )
	{
		return mChunkMap.find( name ) != null;
	}

	bool PackMaster::ReadChunkRaw( ChunkSet* chunk, DataStream* stream )
	{
		Assert( mPackMasterStatus = PMS_ReadPack );
		if ( mPackMasterStatus != PMS_ReadPack )
			return false;

		if ( chunk == null )
			return false;

		if ( stream == null )
			return false;

		// 偏移到数据，跨过chunkset段
		if ( !mDataSource->Seek( chunk->mOffset + sizeof(ChunkSet), MSO_Begin ) )
			return false;

		// stream预分配内存, 为字符串结尾预留空间
		char* buffer = stream->AllocBuffer( chunk->mSize + 8 );

		if ( buffer == null )
			return false;

		// 填充数据
		if ( !mDataSource->ReadBuffer( buffer, chunk->mSize ) )
			return false;

		// 写入偏移
		stream->SetLength( chunk->mSize );

		return true;
	}

	bool PackMaster::ResetChunkIterator(  )
	{
		if ( !ReadNameTable() )
			return false;

		mIterator = mNameTableList.begin();

		return true;
	}

	bool PackMaster::IteratorChunk( WString& name, DataStream* bs )
	{
		if ( mIterator == mNameTableList.end() )
			return false;

		ChunkName& chunkname = *mIterator;
		name = chunkname.mRealName;

		if ( !ReadChunk( chunkname.mNameSet, bs ) )
			return false;

		++mIterator;

		return true;
	}

	void PackMaster::Close( )
	{
		mChunkMap.clear();
		mNameTableList.clear( );
		mPackMasterStatus = PMS_Invalid;
		if ( mSourceAutoDelete )
		{
			SafeDelete(mDataSource);
		}
		else
			mDataSource = null;
	}

	bool PackMaster::CanWrite()
	{
		return mPackMasterStatus == PMS_WritePack;
	}

	bool PackMaster::CanRead()
	{
		return mPackMasterStatus == PMS_ReadPack;
	}

	bool PackMaster::Valid()
	{
		return mPackMasterStatus != PMS_Invalid;
	}

}