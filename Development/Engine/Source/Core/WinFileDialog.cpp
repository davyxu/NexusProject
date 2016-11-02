#include "CorePCH.h"
#include <shlobj.h>
#include <CommCtrl.h>
#include <CommDlg.h>
#include "NekoCore_Stream.h"
#include "NekoCore_Platform.h"

namespace NekoEngine
{
	const wchar* WinFileDialog::GetFileName(void)
	{
		return mFileName.c_str();
	}

	void WinFileDialog::SetDefaultExt(const wchar* ext)
	{
		mDefaultExt = ext;
	}


	bool WinFileDialog::OpenFileDialog(const wchar* pFilter, handle parentwindow )
	{
		wchar buffer[1024]=L"\0";

		DirectoryStack dirstack;

		dirstack.Caputure();


		OPENFILENAME ofn = { sizeof(OPENFILENAME), (HWND)parentwindow, null,
			pFilter, 
			null, 0, 1, buffer, 1024, null, 0, 
			mInitPath.c_str(), null, 
			OFN_FILEMUSTEXIST, 0, 1, null, 0, null, null };

		// 存盘时默认的扩展名
		if (mDefaultExt.length() >0)
		{
			ofn.lpstrDefExt = mDefaultExt.c_str();
		}

		ofn.Flags = OFN_FILEMUSTEXIST;

		if (::GetOpenFileName(&ofn))
		{
			mLastBrowserPath = Directory::GetCurrent();

			mFileName = buffer;
			
			return TRUE;
		}

		dword error = CommDlgExtendedError();
		//CDERR_DIALOGFAILURE

		return FALSE;
	}


	bool WinFileDialog::SaveFileDialog(const wchar* pFilter, handle parentwindow )
	{
		wchar buffer[1024]=L"\0";

		DirectoryStack dirstack;

		dirstack.Caputure();


		OPENFILENAME ofn = { sizeof(OPENFILENAME), (HWND)parentwindow, null,
			pFilter, 
			null, 0, 1, buffer, 1024, null, 0, 
			mInitPath.c_str(), null, 
			OFN_FILEMUSTEXIST, 0, 1, null, 0, null, null };

		// 存盘时默认的扩展名
		if (mDefaultExt.length() >0)
		{
			ofn.lpstrDefExt = mDefaultExt.c_str();
		}

		ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT; 

		if (::GetSaveFileName(&ofn))
		{
			mLastBrowserPath = Directory::GetCurrent();

			mFileName = buffer;
			return TRUE;
		}

		dword error = CommDlgExtendedError();
		//CDERR_DIALOGFAILURE

		return FALSE;
	}

	bool WinFileDialog::GetFolderDialog( const wchar* root, handle parentwindow, WString& path )
	{
		BROWSEINFOW param;
		param.hwndOwner = (HWND)parentwindow;
		param.iImage = null;
		param.lParam =  null;
		param.lpfn = null;
		param.lpszTitle = L"Select folder";
		param.pidlRoot = null;
		param.pszDisplayName = null;
		param.ulFlags = BIF_RETURNONLYFSDIRS|BIF_RETURNFSANCESTORS;
		LPITEMIDLIST pidl = SHBrowseForFolder( &param );

		if ( pidl == null )
			return false;

		wchar buffer[MAX_PATH];
		if (!SHGetPathFromIDList( pidl, buffer) )
			return false;

		path = buffer;

		return true;
	}

	void WinFileDialog::SetInitDir( const wchar* path )
	{
		mInitPath = path;
	}

	WinFileDialog::WinFileDialog()
	{
		mInitPath = Directory::GetCurrent( );
	}

	WinFileDialog::WinFileDialog( const wchar* initpath )
	{
		mInitPath = initpath;
	}

	const wchar*  WinFileDialog::GetLastBrowserPath( )
	{
		return mLastBrowserPath.c_str();
	}
}