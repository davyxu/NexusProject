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
		临界区对象
		@remark
			用于线程同步
	*/
	class LockObject
	{
	private:
		/// 为win32 api 保留
		_byte	mReserved[24];
		bool	mEnable;

	public:
		/// 构造函数
		LockObject( );
		
		/// 析构函数
		~LockObject( );

		/// 启用
		void Enable( bool enable );

		/// 进入临界区
		void Enter( ) const;
		
		/// 离开临界区
		void Leave( ) const;

		/** 
			尝试进入临界区
			@return
				true表示调用线程拥有临界区，false表示没有
		*/
		bool TryEnter( ) const;

		static int IncrementLock( volatile int* Value);

		static int DecrementLock( volatile int* Value);
	};

	/** 
		自动临界区
	*/
	class LockOwner
	{
	private:
		const LockObject&	mLock;

	public:
		/// 构造时自动进入临界区
		LockOwner( const LockObject& lock );
		
		/// 析构时自动离开临界区
		~LockOwner( );
	};

	/** @} */
	/** @} */
};