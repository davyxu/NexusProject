#pragma once


namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Basic
	*  @{
	*/


	/** 命令行封装类
		
	*/

	class Console
	{
	public:
		/** 设
			置命令行语言，默认为中文
			@remarks
				命令行输出中文可能在有些机器上为乱码，需要通过此函数设置encoding类型
			@param
				localstr 默认为中文
		*/
		static void SetLocal(const char* localstr = null);

		/// 输出格式化文字到命令行
		static int Write( const wchar* format, ... );

		/// 输出格式化文字， 自动换行
		static int WriteLine( const wchar* format, ... );

		/// 从命令行接受格式化输入
		static int Input( const wchar* format, ... );

		/// 从键盘等待一个输入字符
		static wchar GetChar( );

		/** 
			控制台事件回调函数类型
			@remarks
				此函数用于接收命令行的各种事件
			@param
				CtrlType 事件类型，请查阅MSDN SetConsoleCtrlHandler
			@return
				1代表此事件已被处理， 0代表未处理
		*/
		typedef int (*ConsoleEvent)(dword CtrlType );
		
		/** 
			控制台事件回调设置
			@remarks
				用于侦测各种命令行事件
			@param
				handle 返回函数类型
			@return
				设置是否成功
			@remark
				注意: 
				1.回调会在另外的命令行线程
				2. 回调后,进程会被命令行控制器终止,时间不确定
				3. 不推荐在服务器关闭时使用
		*/
		static bool SetConsoleEventCallBack( ConsoleEvent handle );
		
	};

	/** @} */
	/** @} */
}