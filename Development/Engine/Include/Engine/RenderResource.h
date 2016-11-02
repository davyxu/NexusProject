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
		渲染资源基类
	*/
	class RenderResource
	{
	public:

		/** 
			@return
				全局初始化后的资源列表
		*/
		static TLinkedList<RenderResource*>*& GetResourceList();

		RenderResource():
			mInitialized(false)
		{}

		virtual ~RenderResource(){}

		/** 
			初始化动态资源,使用Dynamic标志创建的资源
		*/
		virtual void InitDynamicRHI() {}

		/** 
			卸载动态资源
		*/
		virtual void ReleaseDynamicRHI() {}

		/** 
			初始化Managed资源
		*/
		virtual void InitRHI() {}

		/** 
			卸载Managed资源
		*/
		virtual void ReleaseRHI() {}

		/** 
			将资源放入渲染线程开始异步初始化
		*/
		virtual void BeginInitResource( );

		/** 
			将资源放入渲染线程开始异步卸载
		*/
		virtual void BeginReleaseResource( );

		/** 
			不允许资源拷贝
		*/
		void operator=(const RenderResource& OtherResource) {}

		/** 
			资源是否初始化完成
		*/
		bool IsInitialized() const { return mInitialized; }

	protected:
		/// 手动初始化资源
		void Init();

		/// 手动卸载资源
		void Release();

		void UpdateRHI();
	private:

		TLinkedList<RenderResource*> mResourceLink;

		bool mInitialized;
	};

	/** @} */
	/** @} */
}