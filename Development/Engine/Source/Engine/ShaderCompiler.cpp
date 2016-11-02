#include "EnginePCH.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{
	bool ShaderParameterMap::FindParameterAllocation(const char* ParameterName, word& OutBaseRegisterIndex, word& OutNumRegisters) const
	{
		const ParameterAllocation* Allocation = ParameterMap.find(ParameterName);
		if(Allocation)
		{
			OutBaseRegisterIndex = Allocation->BaseRegisterIndex;
			OutNumRegisters = Allocation->NumRegisters;
			return true;
		}
		else
		{
			return false;
		}
	}

	void ShaderParameterMap::AddParameterAllocation(const char* ParameterName,word BaseRegisterIndex,word NumRegisters)
	{
		ParameterAllocation Allocation;
		Allocation.BaseRegisterIndex = BaseRegisterIndex;
		Allocation.NumRegisters = NumRegisters;
		ParameterMap.set(ParameterName,Allocation);
	}


	ShaderParameter::ShaderParameter() 
		: mNumRegisters( 0 )
		, mBaseRegisterIndex( -1 )
	{

	}

	void ShaderParameter::Bind( const ShaderParameterMap& ParameterMap,const char* ParameterName )
	{
		if(!ParameterMap.FindParameterAllocation(ParameterName,mBaseRegisterIndex,mNumRegisters))
		{
			//Assert( "Shader Parameter Not Found" == 0);
		}
	}



}