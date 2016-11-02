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
		渲染窗口
	*/
	class PresentWindow : public RenderTarget
	{
	public:
		PresentWindow( );

		virtual ~PresentWindow( );

		virtual dword GetWidth( );

		virtual dword GetHeight( );

		virtual void Draw( );

		virtual dword GetPriority( ){ return 0;}

		/** 
			获取RHI设备
		*/
		RHIVideoDevice* GetRHIVideoDevice( ){ return mRHIVideoDevice;}

		/** 
			重新设置大小
		*/
		void Resize( dword Width, dword Height );

		/** 
			取得窗口句柄
		*/
		handle GetWindowHandle( );

		/// 获得输入设备
		IInputDriver* GetInputDriver( ){ return mInputDriver; }

	private:
		static void DrawDebugStatus_RenderThread( PresentWindow* RW );

	protected:
		RHIVideoDevice*	mRHIVideoDevice;
		MessageInputDriver* mInputDriver;
	};

	
	/** 
		提供窗口句柄的外部窗口绘制体
	*/
	class ExternWindow : public PresentWindow
	{
	protected:
		friend class GameEngine;

		/** 
			构造函数
			@param
				Window 窗口句柄
			@param
				IsFullScreen 是否全屏
		*/
		ExternWindow( void* Window, bool IsFullScreen );

		virtual ~ExternWindow( );
	};


	/** 
		自动创建的窗口
		@remark
			由于父类RenderTarget被GEngine管理,自动删除,因此new了不用管
	*/
	class GameWindow : public PresentWindow
					 , public WinFormListener
	{
	public:
		/** 
			进行窗口的消息循环,直到窗口关闭
			@remark
				由GameEngine创建窗口后
		*/
		void Go( );

	protected:
		friend class GameEngine;
		GameWindow( const wchar* Title, dword Width, dword Height, bool IsFullScreen , dword IconID  );

		virtual ~GameWindow( );

		virtual void OnMouseUp(int x,int y,eMouseButton button);

		virtual void OnMouseDown(int x,int y,eMouseButton button);

		virtual void OnKeyUp(eKeyButton button);

		virtual void OnKeyDown(eKeyButton button);

		virtual void OnMouseMove(int x,int y,eMouseButton button);

		virtual void OnWindowSizeChanged(Point beforeSize,Point afterSize);

		virtual	void OnWindowStateChanged( );

		virtual bool OnFormClose( );

		virtual void OnMouseWheel(int z ,eMouseButton button);

		bool IsWindowFocus( );

	private:
		WinForm*		mForm;
	};


	/** @} */
	/** @} */
}
