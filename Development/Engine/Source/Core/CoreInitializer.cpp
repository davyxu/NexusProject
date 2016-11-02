#include "CorePCH.h"

#include "NekoCore_Reflection.h"
#include "NekoCore_Platform.h"

namespace NekoEngine
{

	CoreInitializer::CoreInitializer()
	{
		InitGlobalObject();
	}


	void CoreInitializer::InitGlobalObject() 
	{
		MathTable::StaticInit();
		CheckSum::StaticInit( );
		NameRef::StaticInit( );
		TimeSource::StaticInit();
		RTTIObject::StaticInit();	
		BinarySerializer::StaticInit();
	}

	void CoreInitializer::ExitGlobalObject()
	{
		BinarySerializer::StaticExit();
		RTTIObject::StaticExit();
		NameRef::StaticExit();
	}

	CoreInitializer::~CoreInitializer()
	{
		ExitGlobalObject();
	}




}