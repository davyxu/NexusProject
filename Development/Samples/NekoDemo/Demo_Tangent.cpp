#include "EngineDemoPCH.h"
#include "PCH_Scene.h"

IMPLEMENT_RTTI_CLASS(Demo_Tangent)

Demo_Tangent::Demo_Tangent()
{

}

void Demo_Tangent::OnCreate()
{
	
}

void Demo_Tangent::OnEnter()
{
	mModel = GWorld->SpawnActor( RTTI_CLASS_INFO(ActorFactoryStaticMesh) );
	//mModel->CreateResource( L"model/teapot", false );
	//mModel->CreateResource( L"model/toy", true );
}

void Demo_Tangent::OnLeave()
{
	//mModel.Release();
}

void Demo_Tangent::OnDestroy()
{

}

void Demo_Tangent::OnUpdate( float elapse )
{
	//MeshRes* Loader = mModel->GetMeshLoader();

	//Canvas LineBrush( GGameEngine->GetViewportRHI() );

	//const VertexFormat& VF = Loader->GetVertexFormat();
	//dword TangentOffset = VF.GetElementOffset( VU_Tangent );
	//dword BinormalOffset = VF.GetElementOffset( VU_Binormal );
	//dword NormalOffset = VF.GetElementOffset( VU_Normal);
	//dword VertexStride = VF.GetTotalStride( 0 );

	//Assert( TangentOffset != -1 && "No Tangent Found In Mesh");

	//MeshVertexFactory* Content = Loader->GetMeshContent( 0 );

	//MemoryStream* VB = Content->mVertexStream;
	//
	//for ( dword VertexIndex = 0;VertexIndex< Content->mVertexCount; VertexIndex++ )
	//{
	//	Vector3* PositionPtr	= (Vector3*)(VB->GetBuffer() + 0				+ VertexStride* VertexIndex);
	//	Vector3* TangentPtr		= (Vector3*)(VB->GetBuffer() + TangentOffset	+ VertexStride* VertexIndex);
	//	Vector3* BinormalPtr	= (Vector3*)(VB->GetBuffer() + BinormalOffset	+ VertexStride* VertexIndex);
	//	Vector3* NormalPtr		= (Vector3*)(VB->GetBuffer() + NormalOffset		+ VertexStride* VertexIndex);

	//	LineBrush.DrawLine( *PositionPtr, *PositionPtr + (*TangentPtr) *0.1f, Color::cGreen );
	//	LineBrush.DrawLine( *PositionPtr, *PositionPtr + (*BinormalPtr) *0.5f, Color::cRed );
	//	LineBrush.DrawLine( *PositionPtr, *PositionPtr + (*NormalPtr) *0.5f, Color::cBlue );
	//}

	//
	//LineBrush.PushTransform( GGameEngine->mCamera.mViewMatrix * GGameEngine->mCamera.mProjectMatrix );


	//mModel->Draw( );

	//LineBrush.Flush();
}
