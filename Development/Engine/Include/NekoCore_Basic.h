#pragma once

//#ifdef CORE_EXPORTS		// 作为DLL导出
//	#define  __declspec(dllexport)
//#else						// 使用者					// exe
//	#define  __declspec(dllimport)
//	#ifdef _DEBUG
//		#pragma comment( lib, "NekoCored" )
//	#else
//		#pragma comment( lib, "NekoCore" )
//	#endif
//#endif


#pragma warning (disable:4996) // 'wcscpy' was declared deprecated
#pragma warning (disable:4251) // 'nxEngine::LogDevice::mRegionNameArray' : class 'nxEngine::Array<Type>' needs to have dll-interface to be used by clients of class 'nxEngine::LogDevice'
#pragma warning (disable:4312) // warning C4312: 'type cast' : conversion from 'int' to 'nxEngine::handle' of greater size
#pragma warning (disable:4311) // warning C4311: 'type cast' : pointer truncation from 'nxEngine::WinDialog *const ' to 'LONG'
#pragma warning (disable:4275) // warning C4275: non dll-interface class 'NekoEngine::VertexStream' used as base for dll-interface class 'NekoEngine::Sprite'
#pragma warning (disable:4267) // warning C4267: 'argument' : conversion from 'size_t' to 'int', possible loss of data

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif


#include <stdlib.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

#define ENABLE_ASSERT

// Types and define
#include "Core/EngineType.h"
#include "Core/Diagnose.h"
#include "Core/Log.h"
#include "Core/Console.h"

// Helper

#include "Core/StringConv.h"
#include "Core/StringHelper.h"
#include "Core/StringHelperA.h"
#include "Core/CheckSum.h"

// Container
#include "Core/GrowableBuffer.h"
#include "Core/RingBuffer.h"
#include "Core/RefCounting.h"
#include "Core/LinkedList.h"
#include "Core/Link.h"
#include "Core/Pool.h"
#include "Core/StaticHash.h"
#include "Core/DynamicHash.h"

#include "Core/CoreInitializer.h"

