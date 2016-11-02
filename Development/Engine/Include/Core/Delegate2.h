#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Reflection
	*  @{
	*/

	/** 
		代理抽象句柄
	*/
	template<typename TSender, typename TArg> 
	class delegate2_handler 
	{ 
	public: 
		virtual void operator()( TSender sender, TArg args ) = 0;
		virtual delegate2_handler* clone( ) = 0;
	};

	/** 
		代理静态函数句柄
	*/
	template<typename functype, typename TSender, typename TArg> 
	class delegate2_statichandler : public delegate2_handler<typename TSender, typename TArg>
	{ 
	public: 
		delegate2_statichandler(){} 
		delegate2_statichandler(functype &func) 
			: m_func(func) 
		{ 
		}

		virtual void operator()(TSender sender, TArg args) 
		{ 
			m_func( sender, args ); 
		}

		virtual delegate2_handler* clone( )
		{
			return new delegate2_statichandler<functype>(m_func);
		}

	protected: 
		functype    m_func; 
	};

	/** 
		代理成员函数句柄
	*/
	template<typename callee, typename functype, typename TSender, typename TArg> 
	class delegate2_memberhander : public delegate2_handler <typename TSender, typename TArg>
	{ 
	public: 
		delegate2_memberhander(){} 
		delegate2_memberhander(callee* obj, functype func) 
			: m_callee(obj) 
			, m_func(func) 
		{ 
		}

		virtual void operator()(TSender sender, TArg args) 
		{ 
			(m_callee->*m_func)(sender, args); 
		}

		virtual delegate2_handler* clone( )
		{
			return new delegate2_memberhander<callee, functype, TSender, TArg>(m_callee, m_func);
		}

	protected: 
		functype    m_func; 
		callee*    m_callee; 
	};

	/** 
		简单代理2代
		@remark
			支持C/C++ 函数,静态函数,类成员函数作为回调
		@param
			TSender 回调中sender类型
		@param
			TArg 回调中Arg类型
	*/
	template<typename TSender, typename TArg> 
	class delegate2 
	{ 
	public: 
		/** 
			构造函数
		*/
		delegate2() 
			: m_handler(0) 
		{ 
		}

		/** 
			拷贝构造函数
		*/
		delegate2( const delegate2& other )
			: m_handler(0) 
		{
			assign( other );
		}

		/** 
			赋值函数
		*/
		delegate2& operator = ( const delegate2& other)
		{
			return assign( other );
		}

		/** 
			绑定一个C函数或静态成员函数
			@param
				func 需要绑定的函数
			@remark
				例子: xx.bind( class::staticfunc );
		*/
		template<typename functype> 
		delegate2<TSender, TArg>& bind( functype func ) 
		{ 
			m_handler = new delegate2_statichandler<functype, TSender, TArg>(func);

			return *this;
		}

		/** 
			绑定一个C++类成员函数
			@param
				obj 对象实例
			@param
				func 需要绑定的函数
			@remark
				例子: xx.bind( classptr, &class::memberfunc );
		*/
		template<typename callee, typename functype> 
		delegate2<TSender, TArg>& bind( callee* obj, functype func ) 
		{ 
			m_handler = new delegate2_memberhander<callee, functype, TSender, TArg>(obj, func); 

			return *this;
		}

		/** 
			赋值
		*/
		delegate2& assign( const delegate2& other )
		{
			clear();

			if ( other.m_handler )
				m_handler = other.m_handler->clone();

			return *this;
		}

		/** 
			取消绑定
		*/
		void clear( )
		{
			if ( m_handler )
				delete m_handler;
		}

		virtual ~delegate2() 
		{ 
			clear();
		}

		/** 
			绑定是否有效
		*/
		bool valid( )
		{
			return m_handler != 0;
		}

		/** 
			调用绑定的函数
		*/
		void invoke(TSender sender, TArg args)
		{
			if (!m_handler)
				return;

			(*m_handler)(sender, args); 
		}


	private: 
		delegate2_handler<TSender, TArg>     *m_handler; 
	};

	/** @} */
	/** @} */
}


//class MyClass
//{
//public:
//	void foo(int sender, MemoryStream* args)
//	{
//		Console::WriteLine( args.ReadUNICODEString().c_str() );
//	}
//};
//
//
//
//int wmain( )
//{
//	ByteStream s;
//	s.WriteUNICODEString( L"hello");
//
//	MyClass b;
//	delegate2<int, MemoryStream*> a( &b, &MyClass::foo );
//	a.invoke( 0, s );
//}