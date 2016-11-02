#include "CorePCH.h"
#include "NekoCore_Platform.h"

namespace NekoEngine
{
	WinControl::WinControl( ):
	mID(-1),
	mHandle(null),
	mParent(null)
	{

	}

	void WinControl::RawAttach( dword id, handle parent )
	{
		mID = id;
		mParent = parent;
	}


	handle WinControl::GetHandle()
	{
		return mHandle;
	}

	void WinControl::SetText(const wchar* text)
	{	
		::SetWindowText((HWND)mHandle,text);
	}

	WString WinControl::GetText( )
	{
		int len = ::GetWindowTextLength((HWND)mHandle);

		WString text;
		text.resize( len + 1);
		::GetWindowText((HWND)mHandle,(LPWSTR)text.data(),len + 1);	
		return text;
	}

	void WinControl::SetPosition(const Point& pt)
	{
		SetPosition(pt.x,pt.y);
	}

	void WinControl::SetPosition(int x,int y)
	{
		Rect rcThis;
		::GetClientRect((HWND)mHandle,(RECT*)&rcThis);
		::MoveWindow((HWND)mHandle,x,y,rcThis.Width(),rcThis.Height(),true);
	}

	Rect WinControl::GetRect()
	{
		Rect rc;
		::GetWindowRect((HWND)mHandle,(RECT*)&rc);

		if ( mParent )
		{
			Rect rcParent;
			::GetWindowRect((HWND)mParent,(RECT*)&rcParent);

			rc -= rcParent.LeftTop();
		}

		return rc;
	}

	void WinControl::Move(int x, int y, int width, int height)
	{
		::MoveWindow((HWND)mHandle,x, y, width, height ,true);
	}

	Point WinControl::GetSize( )
	{
		Rect rc;
		::GetClientRect((HWND)mHandle,(RECT*)&rc);
		
		return Point(rc.right - rc.left,rc.bottom - rc.top);
	}

	Point WinControl::GetPosition(bool relateToParent)
	{
		Rect rc;
		::GetWindowRect((HWND)mHandle,(RECT*)&rc);

		if (relateToParent)
		{
			Rect rcParent;
			::GetWindowRect((HWND)mParent,(RECT*)&rcParent);
			return Point(rc.left - rcParent.left,rc.top - rcParent.top);
		}
		
		return Point(rc.left,rc.top);
	}

	void WinControl::SetFocus()
	{
		Assert(mHandle != null);

		::SetFocus((HWND)mHandle);
	}

	dword WinControl::GetFocusControlID( )
	{
		return ::GetDlgCtrlID(::GetFocus());
	}

	void WinControl::KillFocus()
	{
		Assert(mHandle != null);

		::SendMessage((HWND)mHandle,WM_KILLFOCUS,0,0);
	}

	void WinControl::Show( )
	{
		Assert(mHandle != null);

		::ShowWindow((HWND)mHandle,true);
	}

	void WinControl::Hide( )
	{
		Assert(mHandle != null);

		::ShowWindow((HWND)mHandle,false);
	}

	void WinControl::Dispose()
	{
		mID		= 0;
		mParent = 0;
		mHandle = null;
	}
}