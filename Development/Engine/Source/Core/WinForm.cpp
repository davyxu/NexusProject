#include "CorePCH.h"
#include "NekoCore_Math.h"
#include "NekoCore_Platform.h"

namespace NekoEngine
{

	WinForm::WinForm( ) :
	mEventHandle(null),
	mIsPaused(false),
	mIgnoreSizeChange(false),
	mIsActive(false),
	mWindowed(true),
	mFullScreenSize(-1,-1)
	{
		
	}

	WinForm::~WinForm()
	{

	}

	bool WinForm::OnIdle()
	{
		if (mEventHandle)
			 return mEventHandle->OnFormLoop();

		return true;
	}

	bool WinForm::Create(const wchar* title,const Point& size,bool windowed,const Point* initPos, handle Parent, dword IconID )
	{
		WNDCLASSEX			winClass;
		winClass.cbSize			= sizeof(WNDCLASSEX); 
		winClass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
		winClass.lpfnWndProc	= (WNDPROC)WndProc; // 消息回调
		winClass.cbClsExtra		= 0;
		winClass.cbWndExtra		= 0; 
		winClass.hInstance		= GetModuleHandle(0);  //GetModuleHandle(0)

		if ( IconID == null )
			winClass.hIcon	= ::LoadIcon( winClass.hInstance, IDI_APPLICATION );
		else
			winClass.hIcon = ::LoadIcon( winClass.hInstance, MAKEINTRESOURCE(IconID) );

		winClass.hCursor		= LoadCursor(null, IDC_ARROW); 
		winClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE); 
		winClass.lpszMenuName	= null; 

		wchar className[256];

		StringHelper::FormatBuffer(className,256,L"nEngine:%dx%d:%s",	size.width,
			size.height,
			title);

		winClass.lpszClassName = className;
		winClass.hIconSm = null;

		if (RegisterClassEx(&winClass) == null)
			return false;

		//获得桌面分辨率
		Point desktopres = Desktop::GetResolution();

		HWND hwnd = null;
		if (windowed)
		{
			RECT rc = {0,0,	size.width,
				size.height};

			//mWinStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
			mWinStyle = WS_OVERLAPPEDWINDOW| WS_VISIBLE;

			::AdjustWindowRect(&rc, mWinStyle,FALSE);

			Point pt;
			if (initPos == null)
			{
				
				pt.x = ( desktopres.x  - size.width ) / 2;
				pt.y = ( desktopres.y - size.height ) / 2;
			}
			else
				pt = *initPos;
			

			hwnd = ::CreateWindowEx( null,
				className,
				title,
				mWinStyle,
				pt.x,
				pt.y,
				rc.right - rc.left,
				rc.bottom - rc.top,
				Parent == null ? ::GetDesktopWindow(): ((HWND)Parent),
				null,
				winClass.hInstance,
				null);

		}
		else
		{
			// 全屏窗口模式
			hwnd = ::CreateWindowEx(	null,
				className,
				title,
				WS_POPUP,
				0,
				0,
				desktopres.x,
				desktopres.y,
				null,
				null,
				winClass.hInstance,
				null);
		}

		mWindowed = windowed;

		mHandle = hwnd;

		if (hwnd == null)
			return false;

		::SetWindowLong(hwnd, GWL_USERDATA, (LONG)this);

		if (mEventHandle)
			mEventHandle->OnFormCreate( );

		return true;

	}

	dword WinForm::WndProc(handle window,int msg,int wParam,int lParam)
	{
		WinForm* form = (WinForm*)::GetWindowLong((HWND)window,GWL_USERDATA);

		if (form == null)
			return (dword)::DefWindowProc((HWND)window, msg, wParam, lParam);

		if (!form->NativeWndProc(window,msg,wParam,lParam))
			return false;

		return (dword)::DefWindowProc((HWND)window, msg, wParam, lParam);
	}

	bool WinForm::FetchKeyState( eKeyButton key )
	{
		return( ::GetKeyState( key ) & 0x8000) != 0;
	}

	dword WinForm::NativeWndProc(handle window,int msg,int wParam,int lParam)
	{
		if (mEventHandle && mEventHandle->OnMessage(msg,(dword)wParam,(dword)lParam))
			return true;

		switch (msg)
		{
		case WM_ACTIVATEAPP:

			if (wParam)
				mIsActive = true;
			else
				mIsActive = false;

			break;
		case WM_DESTROY:
			if (mEventHandle)
			{
				mEventHandle->OnFormDestroy();
			}
			::PostQuitMessage(0);	
			break;
		case WM_CLOSE:
			{
				if (mEventHandle)
				{
					if ( mEventHandle->OnFormClose())
					{
						Destroy();

						return true;
					}
				}
			}
			break;
		case WM_PAINT:
			{
				if ( mEventHandle )
					mEventHandle->OnPaint();
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			{
				int xPos = GET_X_LPARAM(lParam); 
				int yPos = GET_Y_LPARAM(lParam);

				if (mEventHandle)
					mEventHandle->OnMouseDown(xPos,yPos,(eMouseButton)wParam);
				break;
			}

		case WM_LBUTTONUP:
			{
				int xPos = GET_X_LPARAM(lParam); 
				int yPos = GET_Y_LPARAM(lParam); 

				if (mEventHandle)
					mEventHandle->OnMouseUp(xPos,yPos,MB_Left);
				break;
			}
		case WM_RBUTTONUP:
			{
				int xPos = GET_X_LPARAM(lParam); 
				int yPos = GET_Y_LPARAM(lParam); 
				
				if (mEventHandle)
					mEventHandle->OnMouseUp(xPos,yPos,MB_Right);
				break;
			}
		case WM_MBUTTONUP:
			{
				int xPos = GET_X_LPARAM(lParam); 
				int yPos = GET_Y_LPARAM(lParam);

				if (mEventHandle)
					mEventHandle->OnMouseUp(xPos,yPos,MB_Middle);
				break;
			}
		case WM_MOUSEWHEEL:
			{
				short delta = GET_WHEEL_DELTA_WPARAM(wParam);
				if (mEventHandle)
					mEventHandle->OnMouseWheel(delta,(eMouseButton)wParam);

				break;
			}
		case WM_MOUSEMOVE:
			{
				int xPos = GET_X_LPARAM(lParam); 
				int yPos = GET_Y_LPARAM(lParam); 
				if (mEventHandle)
					mEventHandle->OnMouseMove(xPos,yPos,(eMouseButton)wParam);
			}
			break;
		case WM_SYSKEYDOWN://会将Alt+F4关闭截断
		case WM_KEYDOWN:
			if (wParam!= 0 && mEventHandle)
				mEventHandle->OnKeyDown(eKeyButton(wParam));

			return true;// 让默认函数继续工作
		case WM_SYSKEYUP:
		case WM_KEYUP:
			{
				if (wParam!= 0 && mEventHandle)
					mEventHandle->OnKeyUp(eKeyButton(wParam));
			}

			return true;// 让默认函数继续工作

		case WM_ENTERSIZEMOVE:
			mOldSize = GetSize();
			mIsPaused = true;
			break;
		case WM_EXITSIZEMOVE:
			mIsPaused = false;

			if (mEventHandle && mOldSize != GetSize() )
				mEventHandle->OnWindowSizeChanged(mOldSize,GetSize());
			break;
		case WM_ENTERMENULOOP:
			// Pause the app when menus are displayed
			mIsPaused = true;
			break;

		case WM_EXITMENULOOP:
			mIsPaused = false;
			break;
		case WM_SYSCHAR:
			{
				switch( wParam )
				{
				case VK_RETURN:
					{
						// Toggle full screen upon alt-enter 
						DWORD dwMask = (1 << 29);
						if( (lParam & dwMask) != 0 )
						{
							// Toggle the full screen/window mode
							if (mEventHandle)
								mEventHandle->OnToggleDisplayMode(!mWindowed);

							return false;
						}
					}
				}
				break;
			}
		case WM_SIZE:
			{
				if (mIgnoreSizeChange)
					return false;

				if( SIZE_MINIMIZED == wParam )
				{
					mIsPaused = true;
					
					if (mEventHandle)
						mEventHandle->OnWindowStateChanged( );
				}
				else if( SIZE_MAXIMIZED == wParam )
				{
					mIsPaused = false;
				
					if (mEventHandle)
						mEventHandle->OnWindowStateChanged();
				}
				else if( SIZE_RESTORED == wParam )
				{

					eWindowState state = GetWindowState();

					if( state == WS_Maxmized || 
						state == WS_Normal)
					{
						if (mEventHandle)
							mEventHandle->OnWindowStateChanged( );
					}

				}

				break;
			}

			break;
		} 


		return true;
	}

	void WinForm::Close( )
	{
		::SendMessage( (HWND)mHandle, WM_CLOSE, 0, 0 );
	}

	bool WinForm::ToggleFullScreen(bool enable)
	{
		if (mWindowed == !enable)
			return false;		

		mIsPaused = true;
		mIgnoreSizeChange = true;

		mWindowed = !enable;

		if (mWindowed)
		{
			Point thisSize = GetSize();

			//RECT rc = {0,0,thisSize.width,thisSize.height};			
			Rect rc = mSavedRect;
			::AdjustWindowRect((LPRECT)&rc,mWinStyle,false);

			::SetWindowLong( (HWND)mHandle, GWL_STYLE, mWinStyle );

			::SetWindowPos( (HWND)mHandle, HWND_NOTOPMOST,
							rc.left, rc.top,
							rc.Width(),
							rc.Height(),
							SWP_SHOWWINDOW );
		}
		else 
		{
			// 保存全屏前的窗口位置
			::GetWindowRect((HWND)mHandle,(RECT*)&mSavedRect);

		}

		mIgnoreSizeChange = false;
		mIsPaused =false;

		return true;
	}

	void WinForm::SetSize(const Point& size)
	{
		Assert(mHandle != null);

		// 全屏时不能切换
		if (!mWindowed)
			return;

		Point thisSize = GetSize();

		// 窗口改变方可设置
		if (size.width != thisSize.width ||
			size.height != thisSize.height)
		{
			if (mEventHandle)
				mEventHandle->OnWindowSizeChanged(thisSize,size);
		}
	}

	eWindowState WinForm::GetWindowState()
	{
		dword style =::GetWindowLong((HWND)mHandle,GWL_STYLE);
		if ((style & WS_MAXIMIZE) == WS_MAXIMIZE)
			return WS_Maxmized;
		else if ((style & WS_MINIMIZE) == WS_MINIMIZE)
			return WS_Minmized;
		else if ((style & WS_VISIBLE) != WS_VISIBLE)
			return WS_Hide;

		return WS_Normal;
	}

	void WinForm::Clear( )
	{
		RECT rc;
		::GetClientRect((HWND)mHandle,(LPRECT)&rc);
		::InvalidateRect((HWND)mHandle,&rc,true);
	}


	void WinForm::Destroy(void)
	{
		::DestroyWindow(HWND(mHandle));
		mHandle = null;
	}
}