#include "CorePCH.h"		
#include "NekoCore_Platform.h"

namespace NekoEngine
{	
	DynamicLib::DynamicLib() :
	mInstance(null)
	{

	}

	bool DynamicLib::Load(const wchar* libname)
	{
		mInstance = (handle)::LoadLibrary( libname );
		
		return mInstance != null;
	}

	void DynamicLib::Unload()
	{
		::FreeLibrary( (HMODULE)mInstance );
		mInstance = null;
	}

	void* DynamicLib::GetSymbol(const wchar* name)
	{
		return (void*)::GetProcAddress((HMODULE)mInstance, StringConverter().UnicodeToAnsi(name));
	}

	void* DynamicLib::GetSymbol(const char* name)
	{
		return (void*)::GetProcAddress((HMODULE)mInstance, name);
	}

}