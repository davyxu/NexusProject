#include "EnginePCH.h"
#include "NekoEngine_Model.h"

namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS( StaticMeshVertexFactory)

	IMPLEMENT_RTTI_CLASS( StaticMeshResource)

	StaticMeshResource::StaticMeshResource()
	{

	}

	StaticMeshResource::~StaticMeshResource()
	{

	}

	void StaticMeshResource::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );
	}

}