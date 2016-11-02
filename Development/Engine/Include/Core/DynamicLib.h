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
		动态连接库读取类
	*/
	class DynamicLib
	{
	public:
		DynamicLib( );

		/** 
			从磁盘读取动态链接库
		*/
		bool		Load(const wchar* libname);

		/** 
			卸载加载的动态链接库
		*/
		void		Unload();

		/** 
			取得一个Symbol的句柄	
		*/
		void*		GetSymbol(const wchar* name);

		/** 
			取得一个Symbol的句柄
		*/
		void*		GetSymbol(const char* name);

		/** 
			从DLL创建一个实例
			@param
				dllname dll路径
			@param
				entryname 导出的创建函数名
			@param
				userdata 创建时用到的数据
			@return
				返回创建好的类
		*/
		template <typename T>
		T CreateInstance( const wchar* dllname, const wchar* entryname, void* userdata = null )
		{
			typedef T	(*CreateFunc)( void* u );

			if ( !Load(dllname) )
				return null;

			CreateFunc func = (CreateFunc)GetSymbol( entryname );
			if ( func == null )
				return null;

			return func( userdata );
		}

	private:
		handle mInstance;
	};
}
