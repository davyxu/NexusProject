#include "EnginePCH.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// Hight Level Texture Object
	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_NEOBJECT( NeTexture )

	NeTexture::NeTexture()
	: mReady( false )
	{

	}

	// @TODO 从Package系统读取一些信息
	void NeTexture::CreateResource( const wchar* ResourceName )
	{
		WString SourceFile = ResourceName;

		mTextureInstance	= GResourceMaster->Get<TextureRes>( (SourceFile + L".dds").c_str(), this );
	}


	NeTexture::~NeTexture()
	{
		
	}

	void NeTexture::OnResourceStageChanged( ResourceInstance* Loader, eResourceStage Stage )
	{
		if ( Stage == RS_Ready && Loader->IsTypeOf( RTTI_TYPE_ID(TextureRes) ) )
			mReady = true;
	}

}