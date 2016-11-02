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
		文件夹操作封装
	*/
	class Directory
	{
	public:
		/** 
			递归创建文件夹
		*/
		static bool			Create( const wchar* pathname );

		/** 
			取得当前文件夹
		*/
		static const wchar*	GetCurrent(void);

		/** 
			设置当前文件夹
		*/
		static void			SetCurrent(const wchar* path);

		/** 
			取得文件名的完整路径
		*/
		static const wchar* GetFullPath(const wchar* file);

		/** 
			判断一个文件夹是否存在
		*/
		static bool			Exists(const wchar* foldername);

		/** 
			判断一个文件夹是否可写
			@remark
				不使用任何api，直接写一个临时文件夹
		*/
		static bool			IsWritable( const wchar* PathName );

		/** 
			检查一个文件夹是否为隐藏文件夹
		*/
		static bool			IsHidden( const wchar* FileName );
	};

	/** 
		文件夹设置栈
	*/
	class DirectoryStack
	{
	public:
		DirectoryStack(const wchar* newPath);

		DirectoryStack( );

		void Caputure( );

		void Push( const wchar* newPath );

		~DirectoryStack( );
		
	private:
		WString mOldDirectory;
	};
}