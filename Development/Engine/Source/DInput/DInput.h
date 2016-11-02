#pragma once

namespace NekoEngine
{
	class DInput :	public IInputDriver
	{
	public:
		DInput( );
		virtual ~DInput( );

		virtual bool		Create( handle window );

		virtual void		Release( );

		virtual void		Tick( float elapse );

		virtual bool		IsMouseDown(eMouseScanCode code);

		virtual bool		IsKeyDown(eKeyScanCode code);

		virtual Point		GetMouseOffset( );

		virtual int			GetMouseScroller( );

		virtual Point		GetMousePoint( ){ return mMousePoint; }
		

	private:
		void				BuildKeyTable(void);	
		
		handle	mWindowHandle;
		bool	mKeyboardEnable;
		bool	mMouseEnable;
		bool	mJoystickEnable;

		Point	mMousePoint;
		
		byte	mKeyScanCode[MAX_KEY_COUNT];
		bool	mKeyPressedTimes[MAX_KEY_COUNT];					//是否已经按过
		bool	mMousePressedTimes[MAX_MOUSEKEY_COUNT];
		int		m_KeyTable[MAX_KEY_COUNT];
		int		m_MouseTable[MAX_MOUSEKEY_COUNT];

		LPDIRECTINPUT8			mDI;
		LPDIRECTINPUTDEVICE8	mDIJoyStick;
		LPDIRECTINPUTDEVICE8	mDIKeyboard;
		LPDIRECTINPUTDEVICE8	mDIMouse;
		DIMOUSESTATE			mMouseState;

	};
}
