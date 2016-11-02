#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Platform
	*  @{
	*/

	enum eMouseButton
	{
		MB_Left					= 0x01,
		MB_Right				= 0x02,
		KeyMask_Shift			= 0x04,
		KeyMask_Control			= 0x08,
		MB_Middle				= 0x10,
		MB_ForceDWORD			= 0xffffffff,
	};


	enum eWindowState
	{
		WS_Normal	= 0,
		WS_Maxmized,
		WS_Minmized,
		WS_Hide,
	};

	enum eKeyButton
	{
		KB_Escape		 	=0x1B,
		KB_CapsLock		 	=0x14,
		KB_Menu			 	=0x12,

		KB_F1			 	=0x70,
		KB_F2            	=0x71,
		KB_F3            	=0x72,
		KB_F4            	=0x73,
		KB_F5            	=0x74,
		KB_F6            	=0x75,
		KB_F7            	=0x76,
		KB_F8            	=0x77,
		KB_F9            	=0x78,
		KB_F10           	=0x79,
		KB_F11			 	=0x7A,
		KB_F12			 	=0x7B,

		KB_0				=0x30,
		KB_1				=0x31,
		KB_2				=0x32,
		KB_3	            =0x33,
		KB_4	            =0x34,
		KB_5	            =0x35,
		KB_6	            =0x36,
		KB_7				=0x37,
		KB_8			    =0x38,
		KB_9		        =0x39,

		KB_Minus            =0xBD,    // For any country/region, the '-' key
		KB_Equals			=0xBB,    // For any country/region, the '+' key

		KB_Backspace		=0x08,    
		KB_Tab			    =0x09,
		/* 0A ~ 0B system reserved */
		KB_Return			=0x0D,
		/* 0E ~ 0F undefined */

		KB_A              	=0x41,
		KB_B              	=0x42,
		KB_C              	=0x43,
		KB_D              	=0x44,
		KB_E              	=0x45,
		KB_F              	=0x46,
		KB_G              	=0x47,
		KB_H              	=0x48,
		KB_I              	=0x49,
		KB_J              	=0x4A,
		KB_K              	=0x4B,
		KB_L              	=0x4C,
		KB_M              	=0x4D,
		KB_N              	=0x4E,
		KB_O              	=0x4F,
		KB_P              	=0x50,
		KB_Q              	=0x51,
		KB_R              	=0x52,
		KB_S              	=0x53,
		KB_T              	=0x54,
		KB_U              	=0x55,
		KB_V              	=0x56,
		KB_W              	=0x57,
		KB_X              	=0x58,
		KB_Y              	=0x59,
		KB_Z              	=0x5A,

		KB_LeftBracket      =0xDB,	  // left square brackets, For the US standard keyboard, the ']}' key
		KB_RightBracket     =0xDD,    // right square brackets
		KB_Backslash        =0xDC,    // 反斜线, For the US standard keyboard, the '\|' key

		KB_Semicolon        =0xBA,	  // 分号, For the US standard keyboard, the ';:' key 
		KB_SingleQuotes		=0xDE,	//	单引号, For the US standard keyboard, the 'single-quote/double-quote' key

		KB_Comma            =0xBC,	  // 逗号, For any country/region, the ',' key
		KB_Period           =0xBE,    // 主键盘的句点, For any country/region, the '.' key
		KB_Slash            =0xBF,    // 主键盘斜线,  For the US standard keyboard, the '/?' key 
		// For the US standard keyboard, the '`~' key 
		KB_Space            =0x20,

		KB_LeftShift        =0xA0,
		KB_RightShift       =0xA1,
		KB_LeftControl		=0xA2,
		KB_RightControl		=0xA3,
		KB_LeftWindow		=0x5B,
		KB_RightWindow		=0x5C,
		KB_LeftAtl			=0xA4,
		KB_RightAtl			=0xA5,

		KB_RightProperty    =0x5D,
		KB_PrintScreenSysRq =0x2C,
		KB_ScrollLock		=0x91,
		KB_PauseBreak		=0x13,

		KB_Insert			=0x2D,
		KB_Home				=0x24,
		KB_PageUp			=0x21,
		KB_Delete			=0x2E,
		KB_End				=0x23,
		KB_PageDown			=0x22,
		KB_ArrowUp			=0x26,    
		KB_ArrowDown		=0x28,    
		KB_ArrowLeft		=0x25,    
		KB_ArrowRight		=0x27,    

		//小键盘
		KB_NumLock			=0x90,

		KB_Divide			=0x6F,
		KB_Multiply			=0x6A,    // 小键盘乘号
		KB_Add				=0x6B,
		KB_Subtract			=0x6D,
		KB_Decimal			=0x6E,    // 小句点

		KB_Numpad7        	=0x67,
		KB_Numpad8        	=0x68,
		KB_Numpad9        	=0x69,
		KB_Numpad4        	=0x64,
		KB_Numpad5        	=0x65,
		KB_Numpad6        	=0x66,

		KB_Numpad1        	=0x61,
		KB_Numpad2        	=0x62,
		KB_Numpad3        	=0x63,
		KB_Numpad0        	=0x60,
		KB_ForceDWORD		=0xffffffff,
	};

	/** 
		窗口事件通知
	*/
	class WinFormListener
	{
	public:
		/// 窗口创建时
		virtual bool	OnFormCreate( ){return true;}

		/// 消息循环进行时
		virtual bool	OnFormLoop( ){return true;}

		/// 窗口关闭时
		virtual bool	OnFormClose( ){ return true; }

		/// 窗口删除时
		virtual void	OnFormDestroy( ){};

		/// 切换显示模式
		virtual void	OnToggleDisplayMode(bool windowed){}

		/// 窗口状态发生变化时
		virtual	void	OnWindowStateChanged( ){}

		/// 窗口大小变化
		virtual void	OnWindowSizeChanged(Point beforeSize,Point afterSize){}

		/// 鼠标按下时
		virtual void	OnMouseDown(int x,int y,eMouseButton button){}

		/// 鼠标抬起时
		virtual void	OnMouseUp(int x,int y,eMouseButton button){}

		/// 鼠标移动时
		virtual void	OnMouseMove(int x,int y,eMouseButton button){}

		/// 鼠标滚轮变化时
		virtual void	OnMouseWheel(int z ,eMouseButton button){}

		/// 案件按下时
		virtual void	OnKeyDown(eKeyButton button){}

		/// 按键抬起时
		virtual void	OnKeyUp(eKeyButton button){}

		/// windows消息到来时
		virtual bool	OnMessage(dword msg, dword wParam, dword lParam){ return false;}

		/// 窗体绘制时
		virtual void	OnPaint( ){}
	};

	/** 
		Windows窗口
	*/
	class WinForm : public WinControl
	{
	public:
		WinForm( );

		virtual ~WinForm( );

		/** 
			创建窗口
			@param
				title 窗口标题
			@param
				size 窗口大小
			@param
				windowed 是否为窗口
			@param
				initPos 初始位置
			@param
				Parent 父窗口
			@param
				IconID 窗口Icon,来源于resource文件

		*/
		bool			Create(	const wchar* title,
								const Point& size,
								bool windowed = true,
								const Point* initPos = null, 
								handle Parent = null, 
								dword IconID = null );
	
		/// 删除窗口
		void			Destroy( );

		/// 无消息时调用
		bool			OnIdle( );

		/// 切换全屏
		bool			ToggleFullScreen(bool enable);

		/// 是否全屏
		bool			IsFullScreen( );
		
		/// 设置窗口大小
		void			SetSize(const Point& size);

		/// 取得全屏时大小
		Point			GetFullScreenSize( ){ return mFullScreenSize;}

		/// 设置全屏时大小
		void			SetFullScreenSize(const Point& size){ mFullScreenSize;}

		/// 设置事件句柄
		void			SetEventHandle(WinFormListener* handle){ mEventHandle = handle;}

		/// 取得窗口状态
		eWindowState	GetWindowState( );

		/// 清空窗口
		void			Clear( );

		/// 关闭窗口
		void			Close( );

		/// 取得案件状态
		static bool		FetchKeyState( eKeyButton key );

	private:
		static dword	WndProc(handle window,int msg,int wParam,int lParam);
		dword			NativeWndProc(handle window,int msg,int wParam,int lParam);
		

	private:
		WinFormListener*	mEventHandle;
		bool				mWindowed;
		dword				mWinStyle;
		bool				mIsPaused;
		bool				mIgnoreSizeChange;
		Rect				mSavedRect;
		bool				mIsActive;
		Point				mFullScreenSize;
		Point				mOldSize;

	};

	/** @} */
	/** @} */
}
