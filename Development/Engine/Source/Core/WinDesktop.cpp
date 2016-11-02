#include "CorePCH.h"
#include "NekoCore_Platform.h"

namespace NekoEngine
{
	Point Desktop::GetResolution()
	{
		return Point(::GetSystemMetrics( SM_CXSCREEN ),::GetSystemMetrics( SM_CYSCREEN ));
	}

	handle Desktop::GetHandle()
	{
		return ::GetDesktopWindow();
	}




}
