#include "ExportPCH.h"
#include "PluginInclude.h"


MaxModel::MaxModel(  )
{

}

MaxModel::~MaxModel( )
{

}


void MaxModel::AddMesh( INode* Node, int Index )
{
	mMeshArray.push_back( new MaxMesh( GMaxCollector->mMaxSkeleton, Node, Index ) );
}


bool MaxModel::Save( const wchar_t* FileName, VertexExportOption& veo )
{
	if ( mMeshArray.size() == 0 )
		return true;

	using namespace NekoEngine;

	MeshResource* MeshRes;

	if ( veo.mSkin )
	{
		MeshRes = new SkeletonMeshResource;
		GMaxCollector->mMaxSkeleton->Serialize( (SkeletonMeshResource*)MeshRes );
	}
	else
	{
		MeshRes = new StaticMeshResource;
	}

	VertexFormat& VFormat = MeshRes->GetVertexFormat();

	VFormat.Add( 0, VDT_Float3, VU_Position );
	VFormat.Add( 0, VDT_Float3, VU_Normal );

	VFormat.Add( 0, VDT_Float2, VU_TexCoord );
	

	if ( veo.mSkin )
	{
		VFormat.Add( 0, VDT_Float4, VU_Blend_Weights );
		VFormat.Add( 0, VDT_Float4, VU_Blend_Indices );
	}

	if ( veo.mTangent )
	{
		VFormat.Add( 0, VDT_Float3, VU_Tangent );
		VFormat.Add( 0, VDT_Float3, VU_Binormal );
	}		


	for ( DWORD m = 0; m < mMeshArray.size( ); m ++ )
	{	
		MaxMesh* MMesh = mMeshArray[m];
		NekoEngine::MeshVertexFactory* Factory;
		
		if ( veo.mSkin )
			Factory = new NekoEngine::SkeletonMeshVertexFactory;
		else
			Factory = new NekoEngine::StaticMeshVertexFactory;

		MMesh->Prepare();
		MMesh->Serialize( Factory, veo );
		MeshRes->AddVertexFactory( Factory );
	}

	NekoEngine::FileStream Stream( FileName ,FAM_Write );
	BinarySerializer Ser( &Stream, false );
	Ser.SerializerFileHeader( veo.mSkin ? FILE_VER_SKIN : FILE_VER_MESH );
	Ser << MeshRes;

	delete MeshRes;

	return true;
}

DWORD MaxModel::GetMeshCount()
{
	return mMeshArray.size();
}


