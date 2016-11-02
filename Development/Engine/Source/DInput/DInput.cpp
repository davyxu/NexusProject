#include "DInputPCH.h"


namespace NekoEngine
{


	
	DInput::DInput(void) :
		mWindowHandle(null),
		mKeyboardEnable(false),
		mMouseEnable(false),
		mJoystickEnable(false),
		mMousePoint(Point::cZero),
		mDI(null),
		mDIJoyStick(null),
		mDIKeyboard(null),
		mDIMouse(null)
	{
		::memset(mKeyScanCode, 0, sizeof(mKeyScanCode));
		::memset(mKeyPressedTimes, 0, sizeof(mKeyPressedTimes));
		::memset(mMousePressedTimes, 0, sizeof(mMousePressedTimes));
		::memset(&mMouseState,0,sizeof(mMouseState));
	}

	DInput::~DInput(void)
	{

	}

	bool DInput::Create( handle window )
	{
		HRESULT rval;

		mWindowHandle = window;

		rval = ::DirectInput8Create(	GetModuleHandle(null), DIRECTINPUT_VERSION,
			IID_IDirectInput8,(void **) &mDI, null);

		if (rval != DI_OK)
			return false;

		rval = mDI->CreateDevice(GUID_SysKeyboard, &mDIKeyboard, null);

		if (rval == DI_OK)
		{
			rval = mDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
			rval = mDIKeyboard->SetCooperativeLevel((HWND)mWindowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			rval = mDIKeyboard->Acquire();
			mKeyboardEnable = true;
		}

		rval = mDI->CreateDevice(GUID_SysMouse, &mDIMouse, null);
		if (rval == DI_OK)
		{
			rval = mDIMouse->SetDataFormat(&c_dfDIMouse);
			rval = mDIMouse->SetCooperativeLevel((HWND)mWindowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			mMouseEnable = true;
		}
		
		BuildKeyTable();

		return true;
	}

	Point DInput::GetMouseOffset()
	{
		return Point( mMouseState.lX, mMouseState.lY );
	}


	int DInput::GetMouseScroller()
	{
		return mMouseState.lZ;
	}

	void DInput::BuildKeyTable(void)
	{
		for(int i = 0; i<MAX_KEY_COUNT; i++)
		{
			m_KeyTable[i] = i;
		}

		m_MouseTable[TMOUSE_LEFT]	= TMOUSE_LEFT;
		m_MouseTable[TMOUSE_RIGHT]	= TMOUSE_RIGHT;
		m_MouseTable[TMOUSE_MIDDLE] = TMOUSE_MIDDLE;
	}

	void DInput::Tick( float elapse )
	{
		POINT ptMouse;
		RECT rtClient;
		HRESULT hr;
		if (mMouseEnable)
		{
			mDIMouse->Acquire();
			hr = mDIMouse->GetDeviceState(sizeof(mMouseState), &mMouseState);
			
			while( hr == DIERR_INPUTLOST )
			{
				mDIMouse->Acquire();
			}

			::GetCursorPos(&ptMouse);
			::GetClientRect((HWND)mWindowHandle, &rtClient);
			::ScreenToClient((HWND)mWindowHandle, &ptMouse);

			mMousePoint = Point( ptMouse.x, ptMouse.y );

			( mMouseState.rgbButtons[2] > 0 );
		}

		if (mKeyboardEnable)
		{
			mDIKeyboard->Acquire();
			hr = mDIKeyboard->GetDeviceState(sizeof(mKeyScanCode), &mKeyScanCode);
			while( hr == DIERR_INPUTLOST )
			{
				hr = mDIKeyboard->Acquire();
			}
		}
	}

	bool DInput::IsMouseDown(eMouseScanCode code)
	{
		Assert(code >= 0 && code <= MAX_MOUSEKEY_COUNT);
		int scanCode = m_MouseTable[code];
		return ( mMouseState.rgbButtons[scanCode] > 0 );
	}

	bool DInput::IsKeyDown(eKeyScanCode code)
	{
		int scanCode = m_KeyTable[code];
		return (mKeyScanCode[scanCode] > 0 );
	}

	void DInput::Release(void)
	{
		if (mMouseEnable)
		{
			mDIMouse->Unacquire();
		}

		if (mKeyboardEnable)
		{
			mDIKeyboard->Unacquire();
		}

		SafeRelease(mDIKeyboard);
		SafeRelease(mDIMouse);
		SafeRelease(mDI);

		delete this;
	}

	IInputDriver* CreateDInput( )
	{
		return new DInput;
	}
}