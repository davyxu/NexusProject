#include "EnginePCH.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{
	TLinkedList<RenderResource*>*& RenderResource::GetResourceList()
	{
		static TLinkedList<RenderResource*>* FirstResourceLink = null;
		return FirstResourceLink;
	}

	void RenderResource::Init()
	{
		if(!mInitialized)
		{
			mResourceLink = TLinkedList<RenderResource*>(this);
			mResourceLink.Link(GetResourceList());

			InitDynamicRHI();
			InitRHI();

			mInitialized = true;
		}
	}

	void RenderResource::Release()
	{
		if(mInitialized)
		{
			ReleaseDynamicRHI();
			ReleaseRHI();
			mResourceLink.Unlink();
			mInitialized = false;
		}
	}

	void RenderResource::UpdateRHI()
	{
		if(mInitialized )
		{
			ReleaseRHI();
			ReleaseDynamicRHI();
			InitDynamicRHI();
			InitRHI();
		}
	}


	void RenderResource::BeginInitResource( )
	{
		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
			InitCommand,
			RenderResource*,Resource,this,
		{
			Resource->Init();
		});
	}

	void RenderResource::BeginReleaseResource( )
	{
		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
			ReleaseCommand,
			RenderResource*,Resource,this,
		{
			Resource->Release();
		});
	}



}
