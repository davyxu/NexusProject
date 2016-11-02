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
		文件查找事件
	*/
	class FindFileListener
	{
	public:
		/** 
			找到一个文件
			@param
				path 路径
			@param
				name 文件名
			@return
				true表示继续迭代，false停止迭代
		*/
		virtual bool OnFindFile( const wchar* path, const wchar* name ){ return true; }

		/** 
			找到一个文件夹
			@param
				path 路径
			@param
				recursive 是否递归
			@return
				true表示继续迭代，false停止迭代
		*/
		virtual bool OnFindDirectory( const wchar* path , bool& recursive ){ return true; }
	};


	/** 
		文件递归搜索器
	*/
	class FileFinder
	{	
	public:
		struct FileInfo
		{
			WString	mName;
			dword	mSize;
			dword	mAttributes;
		};

		enum FileAttributes
		{
			Attribute_ReadOnly		= 0x00000001,
			Attribute_Hiden			= 0x00000002,
			Attribute_System		= 0x00000004,
			Attribute_Directory		= 0x00000010,
			Attribute_Archive		= 0x00000020,
			Attribute_Normal		= 0x00000080,
		};

		FileFinder( );
		~FileFinder( );

		/** 
			查找第一个文件
			@param
				filename 要查找的文件名，一般使用例如： c:\folder\*
			@param
				fileinfo 查找到的文件信息
			@return
				是否找到文件
		*/
		bool FindFirst( const wchar* filename, FileInfo& fileinfo );

		/** 
			查找下一个文件
			@param
				fileinfo 查找到的文件信息
			@return
				是否找到文件
		*/
		bool FindNext( FileInfo& fileinfo );

		/** 
			搜索目录
			@param
				dir 文件夹名
			@param
				listener 搜索中的事件回调
		*/
		static void ScanDirectory( const wchar* dir, FindFileListener* listener );

	private:
		handle		mFinderHandle;

	};


	/** @} */
	/** @} */
}