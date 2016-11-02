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
		操作系统核心对象
	*/
	class KernelObject
	{
	protected:
		
		handle		mObjectHandle;

	public:
		/// 构造函数
		KernelObject( );
		
		/// 赋值构造函数
		KernelObject( handle objecthandle );
		
		/// 拷贝构造函数
		KernelObject( const KernelObject& kernelobject );
		
		/// 析构函数
		~KernelObject( );

		/// 赋值操作
		KernelObject& operator = ( const KernelObject& kernelobject );

		/// 类型转换
		inline operator handle ( ) const;

		/// 关闭对象
		void Close( );

		/// 赋值一个句柄
		handle CloneHandle( ) const;

		/// 验证句柄是否有效
		inline bool Valid( ) const;
	};



	KernelObject::operator handle ( ) const
	{
		return mObjectHandle;
	}

	bool KernelObject::Valid( ) const
	{
		return mObjectHandle != null;
	}

	/** @} */
	/** @} */
};