#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Render
	*  @{
	*/

	class RenderTarget;
	class RenderTargetableSurface;
	class SceneViewListener;

	/** 
		渲染视图
	*/
	class SceneView
	{
	public:
		/** 
			构造函数,只能由RenderTarget构造
			@param
				World 创建的SceneView归属的世界
			@param
				Owner 渲染目标
			@param
				Cam	创建的SceneView归属的摄像机
			@param
				ZOrder 处理多个View的绘制先后关系
			@param
				Left,Top,Right,Bottom SceneView在屏幕上的相对位置,分量范围(0~1)
		*/
		SceneView( GameWorld* World, RenderTarget* Owner, Camera* Cam, dword ZOrder = 0, float Left = 0.0f, float Top = 0.0f, float Width = 1.0f, float Height = 1.0f );

		/** 
			获得挂接的摄像机
		*/
		Camera* GetCamera( ){ return mCamera; }

		/** 
			设定绑定的摄像机
		*/
		void SetCamera( Camera* Cam ){ mCamera = Cam ;}

		/// 以VP为绘图区域，更新摄像机，并通知World开始绘制
		void Draw( SceneViewListener* Listener );

		/// 设置View对RT的比例
		void SetDimension( float Left = 0.0f, float Top = 0.0f, float Width = 1.0f, float Height = 1.0f );

		/// VP的区域
		const Viewport& GetViewport( ) const{ return mViewport; }

		/// 获取这个View归属的RT
		RenderTarget* GetRenderTarget( ){ return mOwner;}

		/// Z越小，越先绘制
		dword GetZOrder( ){ return mZOrder; }

		/// 绑定盒是否在视锥内可见
		bool IsInFrustum( const AABB& BoundingBox );

		/// 宽度
		dword GetWidth( ){ return mViewport.Width; }

		/// 高度
		dword GetHeight( ){ return mViewport.Height; }
		
		/// 开始查询View上的像素点,由于效率问题,已经废除
		void StartQueryPixel( int X, int Y );

		/// 获取像素点颜色
		bool GetPixel( dword& ColorResult, bool Wait /*= true */ );

		/// 屏幕坐标
		Ray CreateRay( int X, int Y );

		/// 屏幕相对位置(输入相对左上角为0)
		Ray CreateRay( const Vector2& ScreenPos );

		/** 
			获得View上的画布
			@return
				若不在绘制中时,将返回空
		*/
		Canvas* GetCanvas( ){ return mCanvas;}

	private:
		friend class RenderTarget;
		
		// 更新比例到Viewport
		void UpdateDimension( );
		
	protected:
		RenderTarget*	mOwner;
		Viewport		mViewport;
		dword			mZOrder;
		float			mRelativeLeft;
		float			mRelativeTop;
		float			mRelativeWidth;
		float			mRelativeHeight;
		Camera*			mCamera;
		Frustum			mFrustum;
		GameWorld*		mWorld;
		Canvas*			mCanvas;

	private:
		// HitProxy
		RenderTargetableSurface*	mHitProxyRT;
		struct QueryPixelParameter
		{
			int				X, Y;			
			SceneView* ClassPtr;
		};


		static void QueryPixel_RenderThread( QueryPixelParameter& Parameter );
		

	public:		

		volatile dword mColorQueryResult;
		volatile dword mNumPendingFences;
	};


	/** @} */
	/** @} */	
}


