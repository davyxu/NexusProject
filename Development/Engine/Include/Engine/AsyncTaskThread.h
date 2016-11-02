#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup System
	*  @{
	*/


	/** 
		异步任务事件类
	*/
	class AsyncTask
	{
	public:
		AsyncTask( ) : mAsyncTaskFinished( false ){ }
		virtual ~AsyncTask() {}

		/// 加载线程,任务处理
		virtual void OnAsyncTaskProcess() = 0;

		/// 游戏线程, 任务完成
		virtual void OnAsyncTaskFinished( ){}

		/// 异步任务是否完成
		bool IsAsyncTaskFinished( ){ return mAsyncTaskFinished; }

	protected:
		friend class AsyncTaskThread;
		bool mAsyncTaskFinished;
	};

	/** 
		异步任务同步量
	*/
	class AsyncTaskFence : public AsyncTask
	{
	public:
		AsyncTaskFence( bool AutoDeleteSelf = false );

		/** 
			添加一个同步量到加载线程
			一个变量将被自加一
			在加载线程执行完这条指令后,变量将被自减一
		*/
		void BeginFence( );

		/** 
			等待同步任务指令被执行
			@remark
				本函数会一直阻塞直到前方指令全部被执行完毕
		*/
		void Wait( );

		/** 
			获得还剩下多少指令
		*/
		dword GetNumPendingFences() const { return mNumFence; }

	private:
		virtual void OnAsyncTaskProcess();

		virtual void OnAsyncTaskFinished( );

	private:
		volatile dword mNumFence;
		bool mAutoDeleteSelf;
	};


	/** 
		异步任务线程
	*/
	class AsyncTaskThread
	{
	public:
		AsyncTaskThread( );

		/// 添加异步任务
		void Add( AsyncTask* task );

		/// 打开异步任务处理
		void Start( );

		/// 停止异步任务处理
		void Stop( );

		/// 通知异步任务完成
		void NotifyFinishedTask( );

		/// 处理所有剩下的异步任务
		void Flush( );

		/// 异步任务是否可以进行处理
		bool IsEnable( );

		/// 判断当前是否在异步线程
		bool IsInAsyncTaskThread( );

	public:

		void FinishLoading( AsyncTask* task );

	public:
		Thread		mThread;
		bool		mEnable;

		ringbuffer	mInQueue;
		ringbuffer	mOutQueue;

		AsyncTaskFence mFlushFence;
		
		EventObject		mAsyncTaskEvent;
	};

	extern AsyncTaskThread GAsyncTaskThread;

	/** @} */
	/** @} */
}
