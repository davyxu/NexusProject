#pragma once

namespace NekoEngine
{	
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Render
	*  @{
	*/

	/** 
		纹理资源
		@remark
			拥有自我回收机制
	*/
	class TextureResource :	public ManagedObject
	{
		DECLARE_RTTI_CLASS( TextureResource, ManagedObject, ClassFlag_Intrinsic )

	public:
		TextureResource( );

		virtual ~TextureResource( );

		/** 
			纹理宽度
		*/
		dword GetWidth( ){ return mRenderTexture->GetWidth(); }

		/** 
			纹理高度
		*/
		dword GetHeight( ){ return mRenderTexture->GetHeight(); }

		/** 
			获得渲染纹理类
		*/
		RenderTexture* GetRenderTexture( );
		
		virtual void Serialize( BinarySerializer& Ser );

	private:
		virtual bool CanDestroyNow();

		/// 将纹理删除提交到渲染线程
		virtual void BeginDispose( );

		/// 检查纹理是否加载好
		virtual bool IsManualResourceReady();

		/// 游戏线程
		virtual void PostLoad( );

		

	private:
		friend class RenderTexture2D;
		AsyncTaskFence			mStreamingFence;
		RenderCommandFence		mReadyFence;
		RenderTexture2D*		mRenderTexture;
		DDSTextureParameter		mDDSParamter;
	};

	typedef TRefPtr<TextureResource> TextureResourcePtr;

	/** @} */
	/** @} */
}
