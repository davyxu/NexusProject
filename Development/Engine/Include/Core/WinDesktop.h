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
		桌面类
	*/
	class Desktop
	{
	public:
		/// 取桌面分辨率
		static Point GetResolution( );

		/// 取桌面句柄
		static handle GetHandle( );
	};

	/** @} */
	/** @} */
}