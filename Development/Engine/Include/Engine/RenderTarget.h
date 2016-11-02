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
		SceneView操作侦听器
	*/
	class SceneViewListener
	{
	public:
		/** 
			SceneView绘制之前
		*/
		virtual void PreSceneViewDraw( SceneView* RV ){}

		/** 
			SceneView 绘制之后
		*/
		virtual void PostSceneViewDraw( SceneView* RV ){}
	};

	/** 
		渲染目标基类
	*/
	class RenderTarget
	{
	public:
		RenderTarget( );

		virtual ~RenderTarget( );

		/** 
			获得渲染目标宽度
		*/
		virtual dword GetWidth( ) = 0;

		/** 
			获得渲染目标高度
		*/
		virtual dword GetHeight( ) = 0;

		/** 
			渲染优先度
			@remark
				纹理总是比屏幕先绘制
		*/
		virtual dword GetPriority( ) = 0;

		/** 
			渲染目标的绘制操作
		*/
		virtual void Draw( ) = 0;

		// 摄像机为SceneManager创建的，应该是CameraActor对象，有实体，可被渲染出来

		/** 
			在渲染目标上添加一个SceneView,进而实现分屏
			@param
				World 创建的SceneView归属的世界
			@param
				Cam	创建的SceneView归属的摄像机
			@param
				ZOrder 处理多个View的绘制先后关系
			@param
				Left,Top,Right,Bottom SceneView在屏幕上的相对位置,分量范围(0~1)
		*/
		virtual SceneView* AddSceneView( GameWorld* World, Camera* Cam, dword ZOrder = 0, float Left = 0.0f, float Top = 0.0f, float Width = 1.0f, float Height = 1.0f );

		/// 获取每个View
		SceneView* GetView( dword Index );

		/// 清除所有的View
		void ClearView();

		/// RenderTarget的Size有变化时，调用这个更新所有View的大小
		void UpdateViewDimension( );

		/// 订阅每个View更新的通知
		void AddListener( SceneViewListener* Listener );

		/// 取得目标清除颜色
		dword GetClearColor( ){ return mClearColor; }

		/// 设置目标清除颜色
		void SetClearColor( const Color& C ){ mClearColor = C; }

		/// 是否开启绘制
		void SetEnableDraw( bool EnableDraw ) { mEnableDraw = EnableDraw; }

		/// 获得是否开启绘制
		bool GetEnableDraw( ){ return mEnableDraw; }

	private:
		friend class GameEngine;

		void NotifyDestoryCamera(Camera* Cam );

	protected:
		// 绘制Viewport
		void DrawSceneView( );

	private:
		typedef std::map<dword, SceneView*, std::less<dword> > ViewMap;
		ViewMap mViewMap;

	protected:
		SceneViewListener* mListener;
		dword				mClearColor;
		bool				mEnableDraw;
	};

	/** 
		用于渲染目标的资源基类
	*/
	class RenderTargetableResource : public RenderTarget
	{
	public:
		RenderTargetableResource( );

		virtual ~RenderTargetableResource( );

		virtual dword GetWidth( );

		virtual dword GetHeight( );

		virtual void Draw();

		virtual bool IsResourceReady( ) = 0;

		virtual void InitDynamicRHI();

		virtual RHISurfaceBase* GetSurfaceBase( ) = 0;

		virtual dword GetPriority( ){ return 1;}

		/** 
			设置缩放
			
		*/
		void SetScaleSize( SceneView* View , float ScaleX = 1.0f, float ScaleY = 1.0f);

		void SetSize( dword Width, dword Height );

	protected:
		dword mWidth, mHeight;

		SceneView* mView;
		float mScaleX;
		float mScaleY;
	};

	class RenderTargetableTexture2D : public RenderTexture
									, public RenderTargetableResource
	{
	public:
		virtual ~RenderTargetableTexture2D( ){ }

	public:
		// 关闭默认的初始化
		virtual void InitRHI( ){ }

		// 关闭默认的析构
		virtual void ReleaseRHI(){ }

		// 动态资源的处理
		virtual void InitDynamicRHI();

		virtual void ReleaseDynamicRHI();


		virtual bool IsResourceReady( );

		virtual RHISurfaceBase* GetSurfaceBase( );
	
	};

	class RenderTargetableSurface : public RenderResource
								  , public RenderTargetableResource
	{
	public:
		virtual ~RenderTargetableSurface( ){ }

	public:
		// 关闭默认的初始化
		virtual void InitRHI( ){ }

		// 关闭默认的析构
		virtual void ReleaseRHI(){ }

		// 动态资源的处理
		virtual void InitDynamicRHI();

		virtual void ReleaseDynamicRHI();


		virtual bool IsResourceReady( );

		virtual RHISurfaceBase* GetSurfaceBase( );

		virtual void BeginReleaseResource();

		bool ReadyForDestory( );
		
	public:
		RHISurfaceRef mRHISurface;
		RenderCommandFence	mReleaseFence;// 控制析构同步量
	};

	/** @} */
	/** @} */
}
