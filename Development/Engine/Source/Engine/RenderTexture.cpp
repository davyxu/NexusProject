#include "EnginePCH.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// Texture Mip Data
	//////////////////////////////////////////////////////////////////////////
	TextureMipMap::TextureMipMap() 
		: mData( null )
		, mDataSize( 0 )
		, mMipCount( 0 )
	{

	}

	dword TextureMipMap::CalculateImageBytes( dword SizeX, dword SizeY, ePixelFormat PixelFormat )
	{
		return RHI->GetBitsPerPixel( PixelFormat ) / 8 * SizeX * SizeY;
	}

	_byte* TextureMipMap::Alloc( dword DataSize, dword MipCount )
	{
		mData = (_byte*)malloc( DataSize );
		mDataSize = DataSize;
		mMipCount = MipCount;

		::memset( mMips, 0, sizeof mMips );

		return mData;
	}


	void TextureMipMap::Free()
	{
		free( mData );
		mDataSize = 0;
	}

	_byte* TextureMipMap::GetMip( dword MipIndex, dword& Size )
	{
		Assert( MipIndex < MAX_MIP_LEVEL );
		MipLevelData& MipData = mMips[ MipIndex ];

		Size = MipData.Size;

		return MipData.mDataRef;
	}

	void TextureMipMap::SetMip( dword MipIndex, _byte* Data, dword Size )
	{
		Assert( MipIndex < MAX_MIP_LEVEL );
		MipLevelData& MipData = mMips[ MipIndex ];

		MipData.Size = Size;
		MipData.mDataRef = Data;
	}

	//////////////////////////////////////////////////////////////////////////
	// RenderTexture
	//////////////////////////////////////////////////////////////////////////


	RenderTexture::RenderTexture()
		: mTextureRHI( null )
		, mSamplerStateRHI( null )
	{

	}

	void RenderTexture::BeginReleaseResource()
	{
		// 开始卸载设备对象
		__super::BeginReleaseResource();

		mReleaseFence.BeginFence();
	}

	bool RenderTexture::ReadyForDestory( )
	{
		return mReleaseFence.GetNumPendingFences() == 0;
	}


	void RenderTexture::WaitRelease( )
	{
		mReleaseFence.Wait( 0 );
	}

	void RenderTexture::InitRHI()
	{	
		InitSampler();
	}

	void RenderTexture::InitSampler( )
	{
		SamplerStateInitializer Initializer = { SF_Linear, AM_Wrap, AM_Wrap, AM_Wrap };

		mSamplerStateRHI = RHI->CreateSamplerState(Initializer);
	}

	void RenderTexture::ReleaseSamplerTexture( )
	{
		mTextureRHI.Release();

		mSamplerStateRHI.Release();
	}

	void RenderTexture::ReleaseRHI()
	{
		ReleaseSamplerTexture();
	}

	RenderTexture::~RenderTexture()
	{

	}

	bool RenderTexture::Valid()
	{
		return mTextureRHI.Valid();
	}

	void RenderTexture::Commit( dword SamplerIndex )
	{
		if ( mSamplerStateRHI.Valid() )
			mSamplerStateRHI->Commit( SamplerIndex );

		if ( mTextureRHI.Valid() )
			mTextureRHI->Commit( SamplerIndex );
	}

	NekoEngine::dword RenderTexture::GetWidth()
	{
		if ( mTextureRHI.Empty() )
			return 0;

		return mTextureRHI->GetWidth();
	}

	NekoEngine::dword RenderTexture::GetHeight()
	{
		if ( mTextureRHI.Empty() )
			return 0;

		return mTextureRHI->GetHeight();
	}




	//////////////////////////////////////////////////////////////////////////
	// RHI Thread Render Wrapper Class
	//////////////////////////////////////////////////////////////////////////


	RenderTexture2D::RenderTexture2D(  )
		: mSizeX( 0 )
		, mSizeY( 0 )
		, mPixelFormat( PF_Unknown )
	{

	}

	RenderTexture2D::~RenderTexture2D()
	{
		mDataSrc.Free();
	}


	void RenderTexture2D::InitRHI()
	{
		__super::InitRHI( );

		mTextureRHI = RHI->CreateTexture2D( mSizeX, 
			mSizeY,
			mDataSrc.GetMipCount(),
			mPixelFormat );		

		for ( dword MipIndex = 0; MipIndex < mDataSrc.GetMipCount() ; MipIndex++)
		{
			dword DestStride;

			_byte* DestMipData = (_byte*)mTextureRHI->Lock( MipIndex,  true, DestStride );

				dword MipBytes;
				_byte* MipData = mDataSrc.GetMip( MipIndex, MipBytes );

				Assert( MipData != null );

				::memcpy( DestMipData, MipData, MipBytes );
				DestMipData += MipBytes;

			mTextureRHI->Unlock( MipIndex );
		}

	}

	void RenderTexture2D::BeginInitResource( dword SizeX, dword SizeY, ePixelFormat PixelFormat )
	{
		mSizeX			= SizeX;
		mSizeY			= SizeY;
		mPixelFormat	= PixelFormat;

		RenderResource::BeginInitResource();
	}






	//////////////////////////////////////////////////////////////////////////
	// WhiteTexture
	//////////////////////////////////////////////////////////////////////////
	WhiteTexture* GWhiteTexture = null;


	void WhiteTexture::StaticInit( )
	{
		GWhiteTexture = new WhiteTexture;
		GWhiteTexture->BeginInitResource();
	}

	void WhiteTexture::StaticExit( )
	{
		GWhiteTexture->BeginReleaseResource();
		
		// 与渲染线程删除操作同步
		GWhiteTexture->WaitRelease();
		SafeDelete( GWhiteTexture );
	}


	void WhiteTexture::InitRHI()
	{
		mTextureRHI = RHI->CreateTexture2D( 1, 1, 1, PF_A8R8G8B8 );

		dword DestStride;

		dword* DestMipData = (dword*)
		mTextureRHI->Lock( 0, true, DestStride );

		DestMipData[0] = 0xFFFFFFFF;

		mTextureRHI->Unlock( 0 );

		SamplerStateInitializer Initializer = { SF_Nearest, AM_Wrap, AM_Wrap, AM_Wrap };

		mSamplerStateRHI = RHI->CreateSamplerState(Initializer);
	}



	//////////////////////////////////////////////////////////////////////////
	// CheckerTexture
	//////////////////////////////////////////////////////////////////////////
	CheckerTexture* GCheckerTexture = null;


	CheckerTexture::CheckerTexture()
		: mCheckerSize( 32 )
	{

	}

	void CheckerTexture::StaticInit( )
	{
		GCheckerTexture = new CheckerTexture;
		GCheckerTexture->BeginInitResource( );
	}

	void CheckerTexture::StaticExit( )
	{
		GCheckerTexture->BeginReleaseResource();
		GCheckerTexture->WaitRelease();
		SafeDelete( GCheckerTexture );
	}


	void CheckerTexture::InitRHI()
	{
		mTextureRHI = RHI->CreateTexture2D( mCheckerSize, mCheckerSize, 1, PF_A8R8G8B8 );

		dword DestStride;

		dword* DestMipData = (dword*)
			mTextureRHI->Lock( 0, true, DestStride );

		dword ColorA = 0xFFFFFFFF;
		dword ColorB = 0xFF4451f2;
		for ( dword j = 0;j<mCheckerSize;j++)
		{
			dword LineBase = j*mCheckerSize;
			for ( dword i = 0;i< mCheckerSize ;i+=2 )
				DestMipData[i+LineBase] = ColorA;

			for ( dword i = 1;i< mCheckerSize ;i+=2 )
				DestMipData[i+LineBase] = ColorB;

			Math::Swap( ColorA, ColorB );
		}
	
		
		mTextureRHI->Unlock( 0 );

		SamplerStateInitializer Initializer = { SF_Nearest, AM_Wrap, AM_Wrap, AM_Wrap };

		mSamplerStateRHI = RHI->CreateSamplerState(Initializer);
	}



}