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
		命令行解析器
	*/
	class CmdLineParser 
	{
	public:
		CmdLineParser ();

		~CmdLineParser();

		/** 
			取得参数
			@param
				index 索引，base 0
			@return
				返回索引对应的参数
		*/
		const wchar* GetArg( dword index );

		/// 取得参数个数
		dword GetArgCount( );

		/** 
			检查某个参数是否存在
		*/
		bool HasArg( dword index );

		/** 
			检查某个参数名称是否存在
		*/
		bool ArgExist( const wchar* ArgName );

		/// 获得参数名对应的索引
		dword GetArgIndex( const wchar* ArgName );

		/** 
			获得参数内容
			@remark
				-file a.dds, 获得a.dds
			@param
				ArgName 参数名
			@return
				参数内容
		*/
		const wchar* GetArgContent( const wchar* ArgName );
		
	private:
		wchar* m_cmdline; // the command line string

		////////////////////////////////////////////////////////////////////////////////
		// Parse m_cmdline into individual tokens, which are delimited by spaces. If a
		// token begins with a quote, then that token is terminated by the next quote
		// followed immediately by a space or terminator.  This allows tokens to contain
		// spaces.
		// This input string:     This "is" a ""test"" "of the parsing" alg"o"rithm.
		// Produces these tokens: This, is, a, "test", of the parsing, alg"o"rithm
		////////////////////////////////////////////////////////////////////////////////
		void ParseCmdLine (); // ParseCmdLine()
		



	private:
		typedef std::vector<wchar*> CharArray;
		CharArray mCharArray;

	};
}