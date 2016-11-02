#include "EnginePCH.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// Hight Level Texture Object
	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_RTTI_CLASS( TextureResource)

	TextureResource::TextureResource()
	: mRenderTexture( null )
	, mStreamingFence( false )
	{
		mRenderTexture = new RenderTexture2D;
	}


	void TextureResource::BeginDispose()
	{
		mRenderTexture->BeginReleaseResource( );		
	}

	bool TextureResource::CanDestroyNow()
	{
		return	mRenderTexture->ReadyForDestory() && 
				mStreamingFence.GetNumPendingFences() == 0;
	}


	void TextureResource::Serialize( BinarySerializer& Ser )
	{
		DDSCodec_Load( Ser.GetStream(), mRenderTexture->GetMips(), mDDSParamter );
	}


	void TextureResource::PostLoad()
	{
		mRenderTexture->BeginInitResource( mDDSParamter.mSizeX, mDDSParamter.mSizeY, mDDSParamter.mPixelFormat );
		mReadyFence.BeginFence();
	}


	TextureResource::~TextureResource()
	{
		SafeDelete( mRenderTexture );
	}


	bool TextureResource::IsManualResourceReady( )
	{
		return mReadyFence.GetNumPendingFences() == 0;
	}

	RenderTexture* TextureResource::GetRenderTexture()
	{
		return mRenderTexture;
	}



}