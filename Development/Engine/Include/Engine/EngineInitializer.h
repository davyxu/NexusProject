#pragma once

namespace NekoEngine
{
	class EngineInitializer
	{
	public:
		EngineInitializer( );

		~EngineInitializer( );

		static void InitGlobalObject( );

		static void ExitGlobalObject( );
	};

	#define NEKO_ENGINE_INIT NekoEngine::EngineInitializer nekoengineobj;
}
