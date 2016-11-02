#include "CorePCH.h"
#include <Shlwapi.h>
#include <shlobj.h>

#pragma comment(lib,"shlwapi.lib")

#include "NekoCore_Stream.h"
#include "NekoCore_Platform.h"

namespace NekoEngine
{
	const dword Application::cCpuResondTimeMS = 15; 

	static wchar path[1024];
	bool Application::mAutoSleep = false;
	handle Application::mhInstance = null;

	void Application::SetHandle(handle hInstance)
	{
		if (hInstance == null)
			mhInstance = ::GetModuleHandle(null);
		else
			mhInstance = hInstance;
	}

	handle Application::GetHandle( )
	{
		return mhInstance;
	}

	WString Application::GetExecutablePath( )
	{
		::GetModuleFileName(null,path,1024);
		return path;
	}

	WString Application::GetExecutableName( )
	{	
		::GetModuleFileName(null,path,1024);

		return Path::GetFileNameWithoutExtension(path);
	}

	WString Application::GetStartupPath()
	{		
		return Path::GetDirectoryName(GetExecutablePath().c_str()); 
	}

	void Application::SetAutoSleep(bool enable)
	{
		mAutoSleep = enable;
	}

	bool Application::GetAutoSleep( )
	{
		return mAutoSleep;
	}

	const wchar* Application::GetSystemCommandLine( )
	{
		return ::GetCommandLineW( );
	}

	void Application::SetStartupPathByExecutable()
	{
		wchar_t exename[MAX_PATH];
		::GetModuleFileName(NULL,exename,MAX_PATH);
		::PathRemoveFileSpec( exename );
		::SetCurrentDirectory( exename );
	}

	WString Application::GetExecutableDataPath( )
	{
		WString ExecutableName = Path::GetFileNameWithoutExtension(GetExecutablePath().c_str());
		wchar_t PathName[MAX_PATH];
		if ( SUCCEEDED( SHGetFolderPath(	NULL, 
											CSIDL_COMMON_APPDATA, 
											NULL, 
											SHGFP_TYPE_CURRENT, 
											PathName  ) ) 
		   )
		{
			WString AppDataDir = Path::Append( PathName, ExecutableName.c_str());

			if ( Directory::Exists( AppDataDir.c_str() ) )
				return AppDataDir;

			if (!Directory::Create( AppDataDir.c_str() ) )
				return L"";

			return AppDataDir;
		}

		return L"";
	}

	WString Application::GetConfigCombinedPath( const wchar* ConfigFileName, const wchar* SuggestPath )
	{
		// 有建议路径
		if ( SuggestPath != null )
		{
			// 尝试检测建议路径是否可写
			if ( Directory::IsWritable( SuggestPath ) )
				return Path::Append( SuggestPath, ConfigFileName );
		}

		// 建议路径无法写,改用appdate
		WString DataPath = GetExecutableDataPath( );

		if ( Directory::IsWritable( DataPath.c_str() )  )
		{
			return Path::Append( DataPath.c_str(), ConfigFileName );
		}

		// 下下策
		return ConfigFileName;
	}

	bool Application::DoEvents( )
	{
		MSG msg;
		while ( PeekMessage( &msg, null, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if (mAutoSleep)
		{
			// 当CPU空闲时就睡觉，降低CPU占用而不损失效率 降低 CPU占用代码
			float time = TimeSource::GetElapsedTime();
			if ( time < cCpuResondTimeMS)
				::Sleep((dword)(cCpuResondTimeMS - time));
		}



		if ( msg.message == WM_QUIT )
			return false;

		return true;
	}
	
	void Application::Sleep( dword milliseconds )
	{
		::Sleep( milliseconds );
	}



	bool Application::Run(WinForm* form,bool autoSleep)
	{
		Assert( form != null );

		mAutoSleep = autoSleep;

		if (!form)
			return false;

		MSG msg;

		for(;;)
		{
			if( PeekMessage( &msg, null, 0U, 0U, PM_REMOVE ) != 0 )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				if (mAutoSleep)
				{
					// 当CPU空闲时就睡觉，降低CPU占用而不损失效率 降低 CPU占用代码
					float time = TimeSource::GetElapsedTime();
					if ( time < cCpuResondTimeMS)
						::Sleep((dword)(cCpuResondTimeMS - time));
				}

				// 无消息时处理框架
				if (!form->OnIdle( ))
				{
					break;
				}
			}


			if (WM_QUIT == msg.message)
				break;
		}

		return true;

	}
}