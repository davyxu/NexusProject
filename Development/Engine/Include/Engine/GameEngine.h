#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup System
	*  @{
	*/


	class RHIVideoDevice;
	class FrameSpeedUnit;
	class CameraController;

	class PresentWindow;
	class RenderTarget;
	class RenderTargetableTexture2D;
	class GameWindow;
	class ExternWindow;

	/** 
		引擎事件侦听器
	*/
	class EngineEventListener
	{
	public:
		/// 引擎更新帧开始
		virtual void OnEngineFrameBegin( float Elapsed ){ }

		/// 引擎更新帧结束
		virtual void OnEngineFrameEnd( float Elapsed ){ }
	};

	/** 
		引擎
		@remark
			整合管理所有游戏管理器
	*/
	class GameEngine : public RTTIObject
	{
	public:
		DECLARE_RTTI_CLASS( GameEngine, RTTIObject, ClassFlag_Intrinsic )

		GameEngine( );

		virtual ~GameEngine( );

		/**
			初始化引擎
			@remark
				内有默认纹理初始化,因此必须在此之间创建RHI设备
		*/ 
		void Initialize( EngineEventListener* Listener );

		/// 停止引擎, 卸载资源
		void Finalize( );

		/// 更新引擎
		bool Tick( );
		
		/// 获取引擎日志类
		Logger* GetLogger( ){ return mLogger; }
		
		/**
			创建游戏窗口
			@param
				Title 标题
			@param
				Width 宽度
			@param
				Height 高度
			@param
				IsFullScreen 是否全屏窗口
			@param
				IconID 图标资源ID
			@return
				游戏窗口
		*/
		GameWindow* CreateGameWindow( const wchar* Title, dword Width, dword Height, bool IsFullScreen, dword IconID = 0 );

		/**
			创建游戏窗口
			@param
				WindowHandle 窗口句柄
			@param
				IsFullScreen 是否全屏窗口
			@return
				游戏窗口
		*/
		ExternWindow* CreateGameWindow( void* WindowHandle, bool IsFullScreen );

		/** 
			创建游戏世界
		*/
		GameWorld* CreateWorld( );

		/** 
			获取游戏世界
		*/
		GameWorld* GetWorld( dword Index );

		/// 设置事件侦听器
		void SetEventListener( EngineEventListener* Listener ){ mListener = Listener; }

		/// 引擎功能按键拾取
		void CheckEngineShortcut( );

		/// 更新各种RT
		void UpdateRenderTarget( );

		/// 获取RT数量
		dword GetRenderTargetCount( );

		/// 获取RT,窗口属于RT
		RenderTarget* GetRenderTarget( dword Index );

		/// 获取窗口
		PresentWindow* GetWindow( dword Index );


		static void StaticInitProperty( );

	private:

		void DrawRenderTarget( );

		void ClearRenderTarget( );

		void AttachRenderTarget( RenderTarget* RT );

		void DetachRenderTarget( RenderTarget* RT);

		void SaveConfig( );

		void LoadConfig( );

		void SerializeConfig( PropertySerializer& Ser );

		void ClearWorld( );

		friend class GameWorld;
		void NotifyDestoryCamera(Camera* Cam );

		void OnPropertyUseParallelRendering( bool* Value, bool IsGet );

		void OnPropertyUseAsyncTask( bool* Value, bool IsGet );
		
	public:		
		FrameSpeedUnit*	mFPSUnit;
		
		//Camera mCamera;
		Vector3 mTestPoint;

	private:
		Logger* mLogger;
		
		//////////////////////////////////////////////////////////////////////////
		// 属性绑定
		//////////////////////////////////////////////////////////////////////////
		bool mShowGrid;
		bool mUseThreadRendering;
		bool mUseAsyncTask;
		bool mUseShaderCache;


		bool mIsUpdate;		
		EngineEventListener* mListener;

		typedef std::vector<GameWorld*> GameWorldList;
		GameWorldList mGameWorldList;

		// RT管理
		typedef std::vector<RenderTarget*> RenderTargetArray;
		RenderTargetArray mRenderTargetArray;

		// RT卸载时，关闭Detach里的从列表删除
		bool mEnableDraw;
	};

	
	extern GameEngine*		GEngine;
	extern dword			GGameThreadID;

	/** @} */
	/** @} */
}
