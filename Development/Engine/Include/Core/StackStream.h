#pragma once
 
namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Stream
	*  @{
	*/

	/** 
		栈内存流
	*/
	class StackStream : public DataStream
	{
	public:
		enum{
			DEFAULT_PREALLOC_SIZE = 4096, ///< 默认分配的栈大小
		};

		StackStream( );

		/** 
			将内存写入到栈中
		*/
		StackStream( const void* data, dword size );
		
		virtual char*	GetBuffer( );

		virtual char*	AllocBuffer( dword size );

	protected:
		char mStackData[DEFAULT_PREALLOC_SIZE];
	};

	/** @} */
	/** @} */
}