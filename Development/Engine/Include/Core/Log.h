#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Basic
	*  @{
	*/

	class Logger;

	/** 
		日志输出设备
		@remark
			日志输出设备的抽象基类
	*/
	class LogDevice
	{
	public:
		virtual ~LogDevice( ){ }

		virtual void OnAttach( ){}
		/** 
			释放日志对象
			@remark
				跨DLL删除时，请使用Release
		*/
		virtual void Release();

		/** 
			手动调用初始化此函数被调用
			
		*/
		virtual void Initialize( ){ }

		/** 
			Logger类写入内容时调用
			@param
				prefix 日志前缀描述，例如:ShaderMaster, ObjectMaster
			@param
				level 日志级别, @enum Logger::eLogLevel
			@param
				content 格式化过的日志内容
		*/
		virtual void WriteLogContent( const wchar* prefix, dword level, const wchar* content ) = 0;

		/** 
			写入前的格式化内容操作
			@param
				prefix 日志前缀描述，例如:ShaderMaster, ObjectMaster
			@param
				level 日志级别, @enum Logger::eLogLevel
			@param
				content 格式化过的日志内容
			@return
				使用设备提供的mFormatBuffer返回格式化后的日志内容
		*/
		virtual const wchar* GetFormatString( const wchar* prefix, dword level, const wchar* content );

	protected:
		friend class Logger;
		enum{ MAX_FORMATBUFFERSIZE = 1024 };
		wchar	mFormatBuffer[MAX_FORMATBUFFERSIZE];
		Logger* mLogger;
	};

	/**
		日志文件类型
	*/
	enum eFileLogType
	{
		FLT_SingleFile,				///< 独立文件一直添加
		FLT_FilePerProcessLaunch,	///< 每次进程启动创建一个日期不同的日志
	};

	/** 
		普通文件日志设备
		@remark
			增量添加方式的文件日志设备
	*/
	class FileLog : public LogDevice
	{
	public:
		/** 
			构造函数
			@param logfilename 日志文件名
		*/
		FileLog( eFileLogType Type = FLT_SingleFile );

		/** 
			析构函数
			@remark
				关闭日志文件
		*/
		virtual ~FileLog( );

		/** 
			初始化
			@remark
				打开文件
		*/
		virtual void Initialize( );

		/**
			设置日志存放路径
			@remark
				例如: SetFolder(L"Log");
		*/
		void SetFolder( const wchar* FileName );		

		/**
			设置文件写入类型
		*/
		void SetFileLogType( eFileLogType Type ){ mFileLogType = Type; }


		virtual void WriteLogContent( const wchar* prefix, dword level, const wchar* content );

	protected:
		virtual const wchar* GetFormatString( const wchar* prefix, dword level, const wchar* content );

	protected:
		handle			mFile;
		WString			mFolder;
		eFileLogType	mFileLogType;
	};

	/** 
		VC调试输出
	*/
	class IDELog : public LogDevice
	{
	public:
		virtual void WriteLogContent( const wchar* prefix, dword level, const wchar* content );
	};

	/** 
		控制台输出
		@remark
			输出到默认命令行窗口
	*/
	class ConsoleLog : public LogDevice
	{
	public:
		virtual void WriteLogContent( const wchar* prefix, dword level, const wchar* content );
	};

	/**
		日志级别	
	*/
	enum eLogLevel
	{ 
		LogLevel_Debug = 0,		///< 调试级别
		LogLevel_Info  = 1,		///< 信息输出级别
		LogLevel_Alert = 2,		///< 警告级别
		LogLevel_Error = 3,		///< 错误级别
		LogLevel_Fatal = 4,		///< 致命级别
		LogLevel_Max,
	};

	class PropertySerializer;

	/** 
		日志类
		@remark
			提供日志层级写入，挂接日志设备服务
	*/
	class Logger
	{
	public:


		Logger( );

		virtual ~Logger( );

		void Initialize( );

		/** 
			挂接一个日志设备
			@param
				dev 日志设备
		*/
		void AttachDevice( LogDevice* dev );

		/** 
			断开一个日志设备
			@param
				dev 日志设备
		*/
		void DetachDevice( LogDevice* dev );

		/** 
			启用本日志
			@param
				enable 是否启用
		*/
		void SetEnable(bool enable) {mEnable = enable;}

		/** 
			本日志是否启用
			@return
				是否启用
		*/
		bool GetEnable( ){ return mEnable; }

		/** 
			设置日志等级过滤器
			@remark
				高于此等级的日志等级将被记录
			@param
				@enum Logger::eLogLevel
			@note
				初始值为Level_Debug
		*/
		void SetLevelFilter( dword level ) {mLeve = level; }

		/** 
			获取当前日志过滤器等级
			@return
				当前日志过滤器等级 @enum Logger::eLogLevel 
			
		*/
		dword GetLevelFilter( ){ return mLeve; }

		/// 设置是否在日志中显示TheadID
		void SetShowThreadID( bool Value ){ mShowThreadID = Value; }

		/// 是否在日志中显示TheadID
		bool GetShowThreadID( ){ return mShowThreadID; }

		/// 调试等级格式化输出日志
		void Debug( const wchar* format, ...  );

		/// 信息等级格式化输出日志
		void Info( const wchar* format, ...  );

		/// 警告等级格式化输出日志
		void Alert( const wchar* format, ...  );

		/// 错误等级格式化输出日志
		void Error( const wchar* format, ...  );

		/// 致命等级格式化输出日志
		void Fatal( const wchar* format, ...  );

		/** 
			写入一行日志
			@param
				level 日志等级， @enum Logger::eLogLevel
			@param
				format 格式化字符串
		*/
		void WriteLine(dword level, const wchar* format, ...  );

		static const wchar* GetLevelString(  dword level  );

		/** 
			设置日志前缀
			@remark
				通常代表本日志归属大类，例如ShaderMaster，ObjectMaster
		*/
		void SetPrefix( const wchar* prefix ){ mPrefix = prefix; }

		/// 获得日志前缀
		const wchar* GetPrefix( ){ return mPrefix.c_str(); }

		void SerializeConfig( PropertySerializer& Ser );

		/// 替代WriteDebugString童年
		static void WriteDebugInfo( const wchar* Format, ... );
		
		/**
			提供格式化参数的WriteLine版本
			@remark
				WriteLine的实现:
				WriteLine( dword level, const wchar* format, ... )
				{
					WriteLineV( mPrefix.c_str(), level, format, (char*)( &format ) + sizeof( format ) );
				}
		*/
		void WriteLineV( const wchar* prefix, dword level, const wchar* format, const void* arguments );
		
	private:
		bool		mEnable;
		typedef std::set<LogDevice*> DeviceSet;
		DeviceSet	mDeviceSet;
		dword		mLeve;
		WString		mPrefix;
		bool		mShowThreadID;
	};

	/** 
		简单日志类
		@remark
			输出到VC调试窗口
	*/
	class SimpleLog : public Logger
	{
	public:
		SimpleLog( );
	};

	/** 
		命令行工具用日志类
		@remark
			输出到VC调试窗口及命令行
	*/
	class CmdToolLog : public Logger
	{
	public:
		CmdToolLog( );
	};


	/** 
		服务器用日志类
		@remark
			输出到VC调试窗口，命令行及文件
	*/
	class ServerLog : public Logger
	{
	public:
		ServerLog( );
	};

	/** 
		客户端用日志类
		@remark
			输出到VC调试窗口及文件
	*/
	class ClientLog : public Logger
	{
	public:
		ClientLog( );
	};

	/** @} */
	/** @} */
}
