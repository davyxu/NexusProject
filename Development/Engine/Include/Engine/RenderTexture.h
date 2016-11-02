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
		用于保存多层纹理的缓冲,以便实现MipMap
	*/
	class TextureMipMap
	{
	public:
		TextureMipMap( );

		/** 
			计算图片大小所占用的内存
			@param
				SizeX,SizeY 尺寸大小
			@param
				PixelFormat 像素格式
			@return
				图片所占用大小
		*/
		static dword CalculateImageBytes( dword SizeX, dword SizeY, ePixelFormat PixelFormat );

		/** 
			分配缓冲
			@param
				DataSize 可以使用CalculateImageBytes计算出来,DDS压缩格式慎用
			@param
				MipCount 纹理有多少层,至少有1层
			@return
				分配好的内存
			@remark
				分配完成后,必须使用SetMip设置每层的信息
		*/
		_byte* Alloc( dword DataSize, dword MipCount = 1 );

		
		/** 
			获得不同Mip层的数据
			@param
				MipIndex 层次(base 0)
			@param
				Bytes 返回本层占用的Bytes数
			@return
				本层开始的内存指针
		*/
		_byte* GetMip( dword MipIndex, dword& Bytes );

		/** 
			设置不同Mip层的数据
			@param
				MipIndex 层次(base 0)
			@param
				Data 数据来源
			@param
				Bytes 本层占用的Bytes数
		*/
		void SetMip( dword MipIndex, _byte* Data, dword Bytes  );

		/// 释放内存
		void Free( );	

		/** 
			获取纹理层次
		*/
		dword GetMipCount( ){ return mMipCount; }

	private:		
		_byte*			mData;
		dword			mDataSize;
		dword			mMipCount;

		struct MipLevelData 
		{
			_byte*		mDataRef;
			dword		Size;
		};

		enum{ MAX_MIP_LEVEL = 8 };

		MipLevelData	mMips[MAX_MIP_LEVEL];
	};


	/** 
		渲染纹理基类
		@remark
			封装RHI的渲染线程异步初始化过程,需要全程手动管理生命期
	*/
	class RenderTexture: public RenderResource
	{
	public:
		RenderTexture( );

		virtual ~RenderTexture( );

		/** 
			渲染线程初始化采样器
			@remark
				可重载加入自己需要初始化的东西
		*/
		virtual void InitRHI( );

		/** 
			渲染线程卸载采样器及纹理
			@remark
				可重载加入自己需要卸载的东西
		*/
		virtual void ReleaseRHI();

		/// 开始用渲染线程卸载D3D资源
		virtual void BeginReleaseResource( );

		/// 能否删除本类实例
		bool ReadyForDestory( );

		/// 纹理是否有效
		bool Valid( );

		/// 提交纹理及采样器
		void Commit( dword SamplerIndex );

		/// 阻塞等待释放完毕
		void WaitRelease( );

		/// 返回纹理宽度
		dword GetWidth( );

		/// 返回纹理高度
		dword GetHeight( );

		/// 获得RHI层纹理
		RHITextureRef GetTextureRHI(){ return mTextureRHI; }

		/// 获得RHI层的采样器状态
		RHISamplerStateRef GetSamplerStateRHI( ){ return mSamplerStateRHI;}

	protected:
	
		RHITextureRef		mTextureRHI;

		RHISamplerStateRef	mSamplerStateRHI;

	protected:
		void InitSampler( );
		void ReleaseSamplerTexture( );		
		RenderCommandFence	mReleaseFence;	// 控制析构同步量
		
	};



	class TextureResource;
	class RHITexture2D;

	/** 
		二维纹理,带有一个内存数据缓冲
	*/
	class RenderTexture2D: public RenderTexture
	{
	public:
		RenderTexture2D( );

		virtual ~RenderTexture2D( );

		/** 
			开始初始化纹理
			@param
				SizeX,SizeY 纹理大小
			@param
				PixelFormat 像素格式
			@remark
				在调用前应使用GetMips填充MipMapBuffer
			@par
				调用后,开始异步渲染线程初始化,将数据发到GPU
		*/
		void BeginInitResource( dword SizeX, dword SizeY, ePixelFormat PixelFormat );

		virtual void InitRHI( );

		TextureMipMap& GetMips( ){ return mDataSrc;}

	private:
		dword			mSizeX;
		dword			mSizeY;
		ePixelFormat	mPixelFormat;
		TextureMipMap	mDataSrc;// 纹理来源		
	};



	/** 
		空白纹理实现
	*/
	class WhiteTexture: public RenderTexture
	{
	public:
		static void StaticInit( );

		static void StaticExit( );

		virtual void InitRHI( );
		
	};

	/// 空白纹理
	extern WhiteTexture* GWhiteTexture;

	/** 
		蓝白马赛克	
	*/
	class CheckerTexture: public RenderTexture
	{
	public:
		CheckerTexture(  );

		static void StaticInit( );

		static void StaticExit( );

		virtual void InitRHI( );
	private:
		dword mCheckerSize;
	};

	/// 蓝白马赛克
	extern CheckerTexture* GCheckerTexture;

	/** @} */
	/** @} */
}
