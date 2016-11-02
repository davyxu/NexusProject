#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Basic
	*  @{
	*/


	/**
		路径字符串操作类
	*/
	class Path
	{
	public:
		/** 
			获取路径中的文件名部分
			@param
				fullpath 完整路径
			@return
				返回文件名
			@note
				c:\folder\c.bmp -> c.bmp
		*/
		static WString		GetFileName( const wchar* fullpath );

		/** 
			获取路径中文件夹名称
			@param
				fullpath 完整路径
			@return
				返回最后一层文件夹名称
			@note
				c:\folder\c.bmp -> c:\folder
		*/
		static WString		GetDirectoryName( const wchar* fullpath, bool wholepath = true );

		/** 
			获取路径中的扩展名
			@param
				fullpath 完整路径
			@return
				返回扩展名
			@note
				c:\folder\c.bmp -> .bmp
		*/

		static const wchar*	GetExtention( const wchar* fullpath );

		/** 
			检查路径是否是完整路径
			@param
				fullpath 完整路径
			@return
				是否是完整路径
			@note
				相对路径返回false
		*/
		static bool			IsFullpath( const wchar* fullpath );

		/** 
			检查路径是否是文件夹
			@param
				fullpath 完整路径
			@return
				是否是文件夹
		*/

		static bool			IsDirecty( const wchar* fullpath );

		/** 
			检查路径是否是相对路径
			@param
				fullpath 万盏路径
			@return
				是否是相对路径
		*/

		static bool			IsRelative( const wchar* fullpath );

		/** 
			合并路径及文件名
			@param
				path 路径
			@param
				file 文件名
			@return
				合并后的新文件名
		*/
		static WString		Combine( const wchar* path, const wchar* file );

		/** 
			将绝对路径中的相对路径部分去掉
			@param
				from 相对路径部分
			@param
				to 绝对路径部分
			@param
				fromisdir 相对路径是文件夹
			@param
				toisdir	绝对路径部分是文件夹
			@note
				例子：from = c:\a\ to = c:\a\b.bmp  result = RelativePathTo( from, to, true, false ) = "b.bmp"
		*/
		static WString		RelativePathTo( const wchar* from, const wchar* to, bool fromisdir, bool toisdir );

		/** 
			合并两个路径
			@param
				path 路径1
			@param
				anotherpath 路径2
			@return
				合并后的路径
		*/
		static WString		Append( const wchar* path, const wchar* anotherpath );

		/** 
			去除路径中的扩展名
			@param
				fullpath 完整路径
			@return
				去除扩展名后的路径
		*/
		static WString		RemoveExtention( const wchar* fullpath );

		/** 
			标准化路径
			@remark
				路径去掉右斜杠， 统一斜杠为Windows标准
			@param
				path[in,out] 路径输入及修改后结果
		*/
		static void			NormalizePath( WString& path );

		/** 
			获取没有扩展名的文件名
			@param
				fullpath 完整路径
			@return
				没有扩展名的文件名
			@note
				c:\folder\c.bmp -> c
		*/
		static WString		GetFileNameWithoutExtension( const wchar* fullpath );

		/** 
			重命名文件名中的扩展名
			@param
				fullpath 完整路径
			@param
				NewExtName 新扩展名
			@return
				重命名后的路径名
		*/
		static WString		RenameExtension( const wchar* fullpath, const wchar* NewExtName );
	};


	/** @} */
	/** @} */
}