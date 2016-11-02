#pragma once

namespace NekoEngine
{
	/** 
		核心自动构造对象
	*/
	class CoreInitializer
	{
	public:
		/// 构造时，初始化对象
		CoreInitializer( );

		/// 析构时，退出对象
		~CoreInitializer( );

		/// 手动初始化对象
		static void InitGlobalObject( );

		/// 手动析构对象
		static void ExitGlobalObject( );
	};

	/// 在程序入口加入此宏可以自动初始化
	#define NEKO_CORE_INIT NekoEngine::CoreInitializer nekocoreobj;
}
