#include "EnginePCH.h"
#include "NekoInput.h"
#include "NekoEngine_System.h"
#include "Core/Win32Header.h"

namespace NekoEngine
{
	MessageInputDriver::MessageInputDriver( handle window )
		: mMousePos( Point::cZero )
		, mMouseScroll( 0 )
		, mWindowHandle( null )
		, mReceiveOnActive( true )
	{
		mWindowHandle = window;

		::memset( mKeyState, 0, sizeof mKeyState );
		::memset( mMouseState, 0, sizeof mMouseState );

	}


	void MessageInputDriver::Cleanup( bool Force )
	{
		mMouseScroll = 0;
		mLastMousePt = GetMousePoint();

		for ( dword i = 0; i< ArraySize( mKeyState);i++)
		{
			if ( mKeyState[i] == MKS_Up || Force)
				mKeyState[i] = MKS_None;
		}

		for ( dword i = 0; i< ArraySize( mMouseState);i++)
		{
			if ( mMouseState[i] == MKS_Up || Force )
				mMouseState[i] = MKS_None;
		}
	}


	bool MessageInputDriver::IsMouseDown( eMouseButton Button )
	{
		switch ( Button )
		{
		case MB_Left:
			return mMouseState[0] == MKS_Down;
		case MB_Right:
			return mMouseState[1] == MKS_Down;
		case MB_Middle:
			return mMouseState[2] == MKS_Down;
		}


		return false;
	}

	bool MessageInputDriver::IsMouseUp( eMouseButton Button )
	{
		switch ( Button )
		{
		case MB_Left:
			return mMouseState[0] == MKS_Up;
		case MB_Right:
			return mMouseState[1] == MKS_Up;
		case MB_Middle:
			return mMouseState[2] == MKS_Up;
		}

		return false;
	}


	bool MessageInputDriver::IsKeyDown( eKeyButton Button )
	{
		Assert( Button < MAX_VIRUTAL_KEY );
		return mKeyState[ Button ] == MKS_Down;
	}

	bool MessageInputDriver::IsKeyUp( eKeyButton Button )
	{
		Assert( Button < MAX_VIRUTAL_KEY );
		return mKeyState[ Button ] == MKS_Up;
	}

	Point MessageInputDriver::GetMouseOffset()
	{
		return GetMousePoint() - mLastMousePt;
	}

	int MessageInputDriver::GetMouseScroller()
	{
		return mMouseScroll;
	}

	Point MessageInputDriver::GetMousePoint()
	{
		return mMousePos;
	}


	void MessageInputDriver::NotifyKeyState( eKeyButton Button, eMessageKeyState State )
	{
		Assert( Button < MAX_VIRUTAL_KEY );
		mKeyState[ Button ] = State;
	}

	void MessageInputDriver::NotifyMousePos( const Point& Pos )
	{
		mMousePos = Pos;
	}


	void MessageInputDriver::NotifyMouseState( eMouseButton Button, eMessageKeyState State )
	{
		switch ( Button )
		{
		case MB_Left:
			mMouseState[0] = State;
			break;
		case MB_Right:
			mMouseState[1] = State;
			break;
		case MB_Middle:
			mMouseState[2] = State;
			break;
		}
	}


	void MessageInputDriver::NotifyMouseScroll( int Deta )
	{
		mMouseScroll = Deta;
	}

	void MessageInputDriver::Tick( float elapse )
	{
		Cleanup( false );
	}

}