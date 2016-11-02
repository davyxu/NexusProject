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
		应用程序相关函数
	*/
	class Application
	{
	public:
		static const dword cCpuResondTimeMS;

		/** 
			设置保存引用程序Instance
		*/
		static void			 SetHandle(handle hInstance);

		/// 获取应用程序Instance
		static handle		 GetHandle( );

		/** 
			获得应用程序的完整路径名
			@return
				应用程序被启动时的完整路径名
		*/
		static WString		 GetExecutablePath( );

		/** 
			获得应用程序的进程名称
			@return
				应用程序的进程名称
		*/
		static WString GetExecutableName( );

		/** 
			获得启动路径
			@return
				应用程序被启动时的路径
		*/
		static WString		 GetStartupPath( );

		/**
			设置消息泵在工作时，是否允许释放时间片元给CPU
		*/
		static void			 SetAutoSleep(bool enable);

		/**
			获得消息泵在工作时，是否允许释放时间片元给CPU
		*/
		static bool			GetAutoSleep( );

		/// 获得进程关联的命令行
		static const wchar*	 GetSystemCommandLine( );

		/// 释放时间片元
		static void			 Sleep( dword milliseconds );

		/** 
			处理消息
			@return
				是否遇到进程退出消息
		*/
		static bool			 DoEvents( );
	
		/** 
			将工作目录设置为程序的启动目录
			@remark
				避免VC调试时将工程目录设为工作目录
		*/
		static void			SetStartupPathByExecutable();

		/** 
			阻塞允许窗口的消息，直到窗口关闭
		*/
		static bool			Run( WinForm* form,bool autoSleep);

		/** 
			获得应用程序数据存放位置
			@remark
				Vista以上的操作系统，在未获得管理员权限下，应用程序不能写自己的程序目录，使用本函数获得可写的目录位置
		*/
		static WString		GetExecutableDataPath( );

		/** 
			给定配置文件名，返回可写的完整配置文件目录
			@param
				ConfigFileName 配置文件名
			@param
				SuggestPath 建议路径
			@return
				可写的完整配置文件目录
		*/
		static WString		GetConfigCombinedPath( const wchar* ConfigFileName, const wchar* SuggestPath = null );
		
	private:
		static bool mAutoSleep;
		static handle mhInstance;

	};

	/** @} */
	/** @} */
}