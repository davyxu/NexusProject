#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Platform
	*  @{
	*/


	// filter = "ONF格式动画(*.onf)\0*.onf\0\0")

	/** 
		打开对话框
	*/
	class WinFileDialog
	{
	public:
		/** 
			构造函数
			@param
				初始路径
		*/
		WinFileDialog(  const wchar* initpath  );

		WinFileDialog( );

		/// 取对话框返回的文件名
		const wchar*	GetFileName(void);

		/// 设置默认文件名
		void			SetDefaultExt(const wchar* ext);

		/// 设置初始文件夹
		void			SetInitDir( const wchar* path );
	
		/// 打开文件对话框
		bool			OpenFileDialog(const wchar* pFilter, handle parentwindow);

		/// 保存文件对话框
		bool			SaveFileDialog(const wchar* pFilter, handle parentwindow );

		/// 打开选择文件夹对话框
		static bool	GetFolderDialog( const wchar* root, handle parentwindow, WString& path );

		/// 取上次打开对话框前的路径
		const wchar* GetLastBrowserPath( );

	private:
		WString		mFileName;
		WString		mDefaultExt;
		WString		mInitPath;
		WString		mLastBrowserPath;// 上次浏览路径
	};

	/** @} */
	/** @} */
}
