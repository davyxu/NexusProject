#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Platform
	*  @{
	*/

	/** 
		Windows控件基类
	*/
	class WinControl 
	{
	public:
		WinControl( );

		void	RawAttach( dword id, handle parent );

		handle	GetHandle();

		void	SetHandle( handle h ){ mHandle = h;}

		dword	GetID( ){ return mID;}

		handle	GetParent( ){ return mParent; }

		void	SetText(const wchar* text);

		WString	GetText( );

		void	SetPosition(int x,int y);

		void	SetPosition(const Point& pt);// 相对parent

		void	Move(int x, int y, int width, int height);

		Rect	GetRect();

		Point	GetSize( );

		Point	GetPosition(bool relateToParent = false);

		void	SetFocus();

		dword	GetFocusControlID( );

		void	KillFocus();

		void	Show( );

		void	Hide( );

		virtual bool	OnMessage(int msg,int wParam,int lParam){return false;}

		virtual void Dispose( );

	protected:
		dword				mID;
		handle				mParent;
		handle				mHandle;
	};

	/** @} */
	/** @} */
}