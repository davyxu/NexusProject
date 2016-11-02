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
		文件时间结构
	*/
	struct FileTime 
	{
		dword mLowDateTime;
		dword mHighDateTime;

		bool operator == ( const FileTime& Other )
		{
			return memcmp( this, &Other, sizeof( FileTime ) ) == 0;
		}

		bool operator != ( const FileTime& Other )
		{
			return memcmp( this, &Other, sizeof( FileTime ) ) != 0;
		}
	};

	/** 
		文件操作类
	*/
	class File
	{
	public:
		/** 
			判断一个文件是否存在
		*/
		static bool		Exists(const wchar* FileName );

		/** 
			获得文件大小
		*/
		static dword	GetFileSize( const wchar* FileName );

		/** 
			获得文件修改时间	
		*/
		static bool		GetFileModifyTime( const wchar* FileName, FileTime& Time );

		/** 
			删除文件	
		*/
		static bool		Delete( const wchar* FileName );

		/** 
			检查文件是否为隐藏文件	
		*/
		static bool		IsHidden( const wchar* FileName );
	};

	/** @} */
	/** @} */
}