#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Model
	*  @{
	*/


	/** 
		静态模型顶点格式实现
	*/
	class StaticMeshVertexFactory : public MeshVertexFactory
	{
		DECLARE_RTTI_CLASS( StaticMeshVertexFactory, MeshVertexFactory, ClassFlag_Intrinsic )

	public:
		
	};

	/** 
		静态模型资源实现
	*/
	class StaticMeshResource : public MeshResource
	{
		DECLARE_RTTI_CLASS( StaticMeshResource, MeshResource, ClassFlag_Intrinsic|ClassFlag_Serializable )
	public:
		StaticMeshResource( );

		virtual ~StaticMeshResource( );

		virtual void Serialize( BinarySerializer& Ser );
	};

	typedef TRefPtr<StaticMeshResource> StaticMeshResourcePtr;

	/** @} */
	/** @} */
}
