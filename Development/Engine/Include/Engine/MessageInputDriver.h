#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup System
	*  @{
	*/

	enum eMessageKeyState
	{
		MKS_None,
		MKS_Down,
		MKS_Up,
	};

	/** 
		模拟DirectInput的Windows消息映射
	*/
	class MessageInputDriver : public IInputDriver
	{
	public:
		/**
			构造函数
			@param
				window 窗口句柄
		*/
		MessageInputDriver( handle window );

		virtual void		Tick( float elapse );

		/// 是否有鼠标按键按下
		virtual bool		IsMouseDown(eMouseButton Button);

		/// 是否有键盘按键按下
		virtual bool		IsKeyDown(eKeyButton Button);

		/// 是否有鼠标按键按过
		virtual bool		IsMouseUp(eMouseButton Button);

		/// 是否有键盘按键按过
		virtual bool		IsKeyUp(eKeyButton Button);

		/// 获取鼠标偏移
		virtual Point		GetMouseOffset( );

		/// 获取鼠标滚轮
		virtual int			GetMouseScroller( );

		/// 获取鼠标垫
		virtual Point		GetMousePoint( );

		/// 注入按键状态
		void NotifyKeyState( eKeyButton Button , eMessageKeyState State );

		/// 注入鼠标位置消息
		void NotifyMousePos( const Point& Pos );

		/// 注入鼠标抬起消息
		void NotifyMouseState( eMouseButton Button, eMessageKeyState State );


		/// 注入鼠标滚轮消息
		void NotifyMouseScroll( int Deta );

		/// 设置是否在窗口激活时才获取消息
		void SetReceiveOnActive( bool Enable ){ mReceiveOnActive = Enable; }
		
		/// 获取是否在窗口激活时才获取消息
		bool GetReceiveOnActive( ){ return mReceiveOnActive;}

		/// 清除标记
		void Cleanup( bool Force );

	private:
		Point mLastMousePt;
		enum{ MAX_VIRUTAL_KEY = 0xFF };
		eMessageKeyState	mKeyState[MAX_VIRUTAL_KEY];// 这里不使用WM_KEYDOWN记录，避免重复按键效果不好
		eMessageKeyState	mMouseState[3];
		Point			mMousePos;
		int				mMouseScroll;
		handle			mWindowHandle;

		bool			mReceiveOnActive;
	};

	/** @} */
	/** @} */
}
