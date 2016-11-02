#include "ExportPCH.h"
#include "PluginInclude.h"

#include "Engine/EngineInitializer.h"
//----------------------------------------------------------------------------
// MaxInterface
//----------------------------------------------------------------------------
MaxCollector* GMaxCollector = null;

MaxCollector::MaxCollector(  ) 
: mMaxModel( null )
, mMaxSkeletonAnimation( null )
, mMaxSkeleton( null )
, mMaxMaterial( null )
, mExportSelected( false )
{
	
}

MaxCollector::~MaxCollector()
{
	Clear();
}

void MaxCollector::StaticInit( )
{
	NekoEngine::CoreInitializer::InitGlobalObject();
	NekoEngine::EngineInitializer::InitGlobalObject();
}

void MaxCollector::StaticExit( )
{
	NekoEngine::EngineInitializer::ExitGlobalObject();
	NekoEngine::CoreInitializer::ExitGlobalObject();
}

void MaxCollector::CollectData( bool ExportSelected )
{	
	Clear();

	mMaxModel = new MaxModel;

	mMaxSkeleton = new MaxSkeleton;

	mMaxSkeletonAnimation = new MaxSkeletonAnimation( mMaxSkeleton );

	mMaxMaterial = new MaxMaterial;


	INode* rootnode = GMaxInterface->GetRootNode();

	for(int i = 0 ; i  < rootnode->NumberOfChildren() ; i++)
	{	
		INode* ThisNode = rootnode->GetChildNode(i);

		// 只导出选中的物体
		if ( ExportSelected )
		{
			if ( !ThisNode->Selected() )
				continue;
		}

		ParseNode( ThisNode );
	}

}

void MaxCollector::Clear( )
{
	SafeDelete( mMaxModel );
	SafeDelete( mMaxSkeletonAnimation );
	SafeDelete( mMaxSkeleton );
	SafeDelete( mMaxMaterial );
}

void MaxCollector::ParseNode( INode* node )
{
	MaxNode maxnode( node );

	if ( !maxnode.IsVisible() )
		return;

	// 模型
	if ( maxnode.IsMesh( ) )
	{
		Mtl* mtl = node->GetMtl( );

		if ( mtl != null && mtl->IsMultiMtl( ) )
		{
			for ( int i = 0; i < mtl->NumSubMtls( ); i ++ )
			{
				Mtl* submtl = mtl->GetSubMtl( i );

				mMaxModel->AddMesh( node, i );
			}
		}
		else
		{
			mMaxModel->AddMesh( node, -1 );
		}


	}
	// 骨头
	else if ( maxnode.IsBone( ) && maxnode.IsRoot( ) )
	{
		mMaxSkeleton->AddBone( node, -1 );
	}
}






