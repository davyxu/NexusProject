#pragma once

namespace NekoEngine
{
	#pragma warning( disable:4291 )

	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Render
	*  @{
	*/


	/** 
		渲染指令
	*/
	class RenderCommand
	{
	public:
		
		/** 
			使用分配器分配一个渲染指令
			@param
				Size 需要分配的大小
			@param
				Allocation 分配器
			@remark
				例子:new(ringbuffer::alloc_context(GRenderThread.mRenderCommandBuffer,sizeof(TypeName))) TypeName Params;
		*/		
		void* operator new(size_t Size,const ringbuffer::alloc_context& Allocation)
		{
			return Allocation.get_alloc();
		}

		virtual ~RenderCommand() {}

		/** 
			渲染线程执行指令
		*/
		virtual dword Execute() = 0;

		/** 
			指令描述
		*/
		virtual const char* DescribeCommand() = 0;
	};

	/** 
		渲染指令同步量
	*/
	class RenderCommandFence
	{
	public:
		RenderCommandFence():
			NumPendingFences(0)
		{}

		/** 
			添加一个同步量到渲染线程
			一个变量将被自加一
			在渲染线程执行完这条指令后,变量将被自减一
		*/
		void BeginFence();


		/** 
			等待渲染同步指令被执行
			@param
				NumFencesLeft 指定等待剩下多少指令后停止
			@remark
				本函数会一直阻塞直到前方指令全部被执行完毕
		*/
		void Wait( dword NumFencesLeft=0 ) const;

		/** 
			获得还剩下多少指令
		*/
		dword GetNumPendingFences() const { return NumPendingFences; }

	private:
		volatile dword NumPendingFences;
	};

	/** 
		渲染线程
	*/
	class RenderThread
	{
	public:
		RenderThread( );

		/** 
			刷新线程中所有的渲染指令
		*/
		float Flush();

		/** 
			开启渲染线程
		*/
		void Start( );

		/** 
			停止渲染线程
		*/
		void Stop( );

		/**
			通知线程开始处理
		*/
		void NotifyNewTask( );

		/// 是否开启
		bool IsEnable( ){ return mEnable; }

	public:
		/// 环状缓冲用于线程间任务交换
		ringbuffer	mRenderCommandBuffer;

		bool		mEnable;

		Thread		mRenderThread;

		EventObject mRenderEvent;
	};

	extern RenderThread GRenderThread;


	#define ENQUEUE_RENDER_COMMAND(TypeName,Params) \
	{ \
		if(GRenderThread.mEnable) \
		{ \
			new(ringbuffer::alloc_context(GRenderThread.mRenderCommandBuffer,sizeof(TypeName))) TypeName Params; \
			GRenderThread.NotifyNewTask();\
		} \
		else \
		{ \
			TypeName Command Params; \
			Command.Execute(); \
		} \
	}

	/**
	 * Declares a rendering command type with 0 parameters.
	 */
	#define ENQUEUE_UNIQUE_RENDER_COMMAND(TypeName,Code) \
		class TypeName : public RenderCommand \
		{ \
		public: \
			virtual dword Execute() \
			{ \
				Code; \
				return sizeof(*this); \
			} \
			virtual const char* DescribeCommand() \
			{ \
				return #TypeName; \
			} \
		}; \
		ENQUEUE_RENDER_COMMAND(TypeName,);



	#define ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,ParamValue1,Code) \
	class TypeName : public RenderCommand \
	{ \
	public: \
		typedef ParamType1 _ParamType1; \
		TypeName(const _ParamType1& In##ParamName1): \
		  ParamName1(In##ParamName1) \
		{} \
		virtual dword Execute() \
		{ \
			Code; \
			return sizeof(*this); \
		} \
		virtual const char* DescribeCommand() \
		{ \
		return #TypeName; \
		} \
	private: \
		ParamType1 ParamName1; \
	}; \
	ENQUEUE_RENDER_COMMAND(TypeName,(ParamValue1));

/**
 * Declares a rendering command type with 2 parameters.
 */
#define ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,ParamValue1,ParamType2,ParamName2,ParamValue2,Code) \
	class TypeName : public RenderCommand \
	{ \
	public: \
		typedef ParamType1 _ParamType1; \
		typedef ParamType2 _ParamType2; \
		TypeName(const _ParamType1& In##ParamName1,const _ParamType2& In##ParamName2): \
		  ParamName1(In##ParamName1), \
		  ParamName2(In##ParamName2) \
		{} \
		virtual dword Execute() \
		{ \
			Code; \
			return sizeof(*this); \
		} \
		virtual const char* DescribeCommand() \
		{ \
		return #TypeName; \
		} \
	private: \
		ParamType1 ParamName1; \
		ParamType2 ParamName2; \
	}; \
	ENQUEUE_RENDER_COMMAND(TypeName,(ParamValue1,ParamValue2));

	/** @} */
	/** @} */
}
