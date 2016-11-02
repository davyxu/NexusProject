#include "CorePCH.h"
#include "Core/Win32Header.h"
#include "Core/TimeSource.h"

namespace NekoEngine
{

	Logger::Logger( )
		: mEnable( true )
		, mLeve( LogLevel_Debug )
		, mShowThreadID( false )
	{
		
	}

	void Logger::WriteDebugInfo( const wchar* Format,  ... )
	{	
		wchar buffer[1024];
		StringHelper::FormatBufferV( buffer,1024, Format, (char*)( &Format ) + sizeof( Format )  );

		OutputDebugString( buffer );
	}

	Logger::~Logger( )
	{
		for ( DeviceSet::iterator it = mDeviceSet.begin();
			it != mDeviceSet.end();
			++it )
		{
			LogDevice* Device = *it;
			Device->Release();
		}

		mDeviceSet.clear();
	}

	void Logger::AttachDevice( LogDevice* dev )
	{
		if ( dev == null )
			return;

		dev->mLogger = this;
		dev->OnAttach();

		mDeviceSet.insert( dev );
	}


	void Logger::DetachDevice( LogDevice* dev )
	{
		DeviceSet::iterator it = mDeviceSet.find( dev );
		if ( it == mDeviceSet.end() )
			return;

		dev->Release();

		mDeviceSet.erase( it );
	}


	void Logger::WriteLineV( const wchar* prefix, dword level, const wchar* format, const void* arguments )
	{
		if (!mEnable)
			return;

		// 过滤器
		if ( level < mLeve )
			return;

		wchar buffer[1024];
		StringHelper::FormatBufferV( buffer,1024, format, arguments );


		for ( DeviceSet::iterator it = mDeviceSet.begin();
			it != mDeviceSet.end();
			++it )
		{
			LogDevice* dev = *it;
			dev->WriteLogContent( prefix, level, buffer );
		}
	}



	void Logger::Initialize()
	{
		for ( DeviceSet::iterator it = mDeviceSet.begin();
			it != mDeviceSet.end();
			++it )
		{
			LogDevice* dev = *it;
			dev->Initialize();
		}
	}


	void Logger::Debug( const wchar* format, ... )
	{
		WriteLineV( mPrefix.c_str(), LogLevel_Debug, format, (char*)( &format ) + sizeof( format ) );
	}

	void Logger::Info( const wchar* format, ... )
	{
		WriteLineV( mPrefix.c_str(), LogLevel_Info, format, (char*)( &format ) + sizeof( format ) );
	}

	void Logger::Alert( const wchar* format, ... )
	{
		WriteLineV( mPrefix.c_str(), LogLevel_Alert, format, (char*)( &format ) + sizeof( format ) );
	}

	void Logger::Error( const wchar* format, ... )
	{
		WriteLineV( mPrefix.c_str(), LogLevel_Error, format, (char*)( &format ) + sizeof( format ) );
	}

	void Logger::Fatal( const wchar* format, ... )
	{
		WriteLineV( mPrefix.c_str(), LogLevel_Fatal, format, (char*)( &format ) + sizeof( format ) );
	}

	const wchar* Logger::GetLevelString( dword level )
	{
		static const wchar* LevelString[] = {
			L"DEBUG",
			L"INFO",
			L"***ALERT",
			L"!!!ERROR",
			L"!!!FATAL",
		};

		return LevelString[level];
	}


	void Logger::WriteLine( dword level, const wchar* format, ... )
	{
		WriteLineV( mPrefix.c_str(), level, format, (char*)( &format ) + sizeof( format ) );
	}

	void Logger::SerializeConfig( PropertySerializer& Ser )
	{
		Ser << L"Enable" << mEnable
			<< L"Prefix" << mPrefix
			<< L"Level" << mLeve;
	}

	const wchar* LogDevice::GetFormatString(  const wchar* prefix,  dword level, const wchar* content )
	{
		TimeData Data;

		TimeSource::GetLocalTime( Data );

		if ( mLogger->GetShowThreadID() )
		{
			return StringHelper::FormatBuffer(mFormatBuffer, 
				MAX_FORMATBUFFERSIZE, 
				L"[%s] %02d:%02d:%02d:%03d |%x| %s>> %s\r\n",
				Logger::GetLevelString(level),
				Data.mHour,
				Data.mMinute,
				Data.mSecond, 
				Data.mMilliSeconds, 
				Thread::GetCurrentThreadID(),
				prefix,
				content );
		}

		return StringHelper::FormatBuffer(mFormatBuffer, 
			MAX_FORMATBUFFERSIZE, 
			L"[%s] %02d:%02d:%02d:%03d %s>> %s\r\n",
			Logger::GetLevelString(level),
			Data.mHour,
			Data.mMinute,
			Data.mSecond, 
			Data.mMilliSeconds, 
			prefix,
			content );
	}

	void LogDevice::Release()
	{
		delete this;
	}


	FileLog::FileLog( eFileLogType Type )
		: mFile( null )
		, mFileLogType( Type )
		, mFolder( L"." )
	{
	
	}

	FileLog::~FileLog( )
	{
		if ( mFile != NULL )
			::CloseHandle( mFile );
	}

	void FileLog::Initialize()
	{
		WString LogFileName = Path::Combine( mFolder.c_str(), Application::GetExecutableName().c_str() );

		switch ( mFileLogType )
		{
		case FLT_SingleFile:
			{
				wchar FileName[260];

				StringHelper::FormatBuffer( FileName, 260,L"%s.log",LogFileName.c_str());

				mFile = ::CreateFile( FileName, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE , NULL , OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
			}
			break;

		case FLT_FilePerProcessLaunch:
			{
				TimeData Data;

				TimeSource::GetLocalTime( Data );

				// 创建一个带日期的文件
				wchar FileName[260];
				StringHelper::FormatBuffer( FileName, 260,L"%s%d%02d%02d%02d%02d%02d%03d.log",
					LogFileName.c_str(),
					Data.mYear, 
					Data.mMonth, 
					Data.mDay,
					Data.mHour,
					Data.mMinute,
					Data.mSecond, 
					Data.mMilliSeconds
					);

				mFile = ::CreateFile( FileName, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE , NULL , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
			}
			break;
		}
		
	}

	const wchar* FileLog::GetFormatString(  const wchar* prefix,  dword level, const wchar* content )
	{
		TimeData Data;

		TimeSource::GetLocalTime( Data );

		if ( mLogger->GetShowThreadID() )
		{
			return StringHelper::FormatBuffer(mFormatBuffer, 
				MAX_FORMATBUFFERSIZE, 
				L"[%s] %d-%02d-%02d %02d:%02d:%02d:%03d |%x| %s>> %s\r\n",
				Logger::GetLevelString(level),
				Data.mYear, 
				Data.mMonth, 
				Data.mDay,
				Data.mHour,
				Data.mMinute,
				Data.mSecond, 
				Data.mMilliSeconds, 
				Thread::GetCurrentThreadID(),
				prefix,
				content );
		}
		

		return StringHelper::FormatBuffer(mFormatBuffer, 
			MAX_FORMATBUFFERSIZE, 
			L"[%s] %d-%02d-%02d %02d:%02d:%02d:%03d %s>> %s\r\n",
			Logger::GetLevelString(level),
			Data.mYear, 
			Data.mMonth, 
			Data.mDay,
			Data.mHour,
			Data.mMinute,
			Data.mSecond, 
			Data.mMilliSeconds, 
			prefix,
			content );

		
	}

	void FileLog::WriteLogContent( const wchar* prefix,  dword level, const wchar* content )
	{
		if ( mFile == NULL )
			return;

		StringConverter conv;
		char* ansilog = conv.UnicodeToAnsi( GetFormatString( prefix, level, content ) );

		::SetFilePointer(mFile , 0 , NULL , FILE_END);    //移动到文件末尾
		DWORD dwWriteLen;
		BOOL bSuccess = ::WriteFile(mFile , ansilog , (DWORD)strlen( ansilog ) , &dwWriteLen , NULL);
	}

	void FileLog::SetFolder( const wchar* FileName )
	{
		mFolder = FileName;
		Directory::Create( mFolder.c_str() );
	}


	void IDELog::WriteLogContent( const wchar* prefix,  dword level, const wchar* content )
	{
		::OutputDebugString( GetFormatString( prefix, level, content ) );
	}

	void ConsoleLog::WriteLogContent( const wchar* prefix,  dword level, const wchar* content )
	{
		Console::Write( GetFormatString( prefix, level, content ) );
	}


	SimpleLog::SimpleLog()
	{
		AttachDevice( new IDELog );
	}

	CmdToolLog::CmdToolLog( )
	{
		AttachDevice( new IDELog );
		AttachDevice( new ConsoleLog );
	}

	ServerLog::ServerLog( )
	{
		AttachDevice( new FileLog );
		AttachDevice( new IDELog );
		AttachDevice( new ConsoleLog );
	}

	ClientLog::ClientLog( )
	{
		AttachDevice( new FileLog );
		AttachDevice( new IDELog );
	}
}
