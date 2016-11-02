#include "EnginePCH.h"
#include "NekoEngine_Model.h"

namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS( StaticMeshComponent)

	void StaticMeshComponent::StaticInitProperty()
	{
		PropertyCollection* PC = RTTI_CLASS_INFO(StaticMeshComponent)->GetPropertyCollection( );
		PC->Add( PROPERTY_DELEGATE( StaticMeshComponent,WString,L"Mesh", &StaticMeshComponent::OnPropertyMesh ) );

	}

	void StaticMeshComponent::OnPropertyMesh( WString* Value, bool IsGet )
	{
		if ( IsGet )
		{
			if ( mMesh.Empty() )
			{
				*Value = L"";
			}
			else
			{
				*Value = mMesh.GetResource()->GetName();
			}
		}
		else
		{
			SetMesh( GObjectManager->LoadObject<StaticMeshResource>( *Value ) );
		}
	}


	void StaticMeshComponent::SetMesh( const StaticMeshResourcePtr& Mesh )
	{
		if ( Mesh.Empty( ) )
			return;

		// 不能在正在更换时又换
		if ( mMesh.IsDirty() )
			return;

		mMesh = Mesh;
		
		
		mOwner->SetDebugName( StringHelper::FormatString( L"%s#%s", mOwner->GetName().c_str(), Mesh->GetName().c_str() ) );
	}

	void StaticMeshComponent::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		if ( Ser.IsLoading() )
		{

			WString MeshName;

			Ser << MeshName;

			SetObjectPropertyByString( L"Mesh", MeshName.c_str() );
		}
		else
		{
			WString MeshName;

			GetObjectPropertyAsString( L"Mesh", MeshName );

			Ser << MeshName;
		}
	}

	void StaticMeshComponent::Tick( float elapse )
	{
		if ( !IsResourceReady() )
		{
			CheckResourceStage();
		}

	}

	void StaticMeshComponent::CheckResourceStage( )
	{
		Super::CheckResourceStage();

		if ( mMesh.CheckReadyToUpdate( ) )
		{
			// 计算绑定盒
			mOwner->SetBoundingBox( mMesh.GetResource()->GetBoundingBox(  ) );

			mMesh.ClearFlag( );
		}
	}

	bool StaticMeshComponent::IsResourceReady()
	{
#ifdef _DEBUG
		bool MatReady = mMaterialInstance.IsResourceReady();

		bool MeshReady = mMesh.IsResourceReady();

		return MatReady && MeshReady;
#else
		return mMaterialInstance.IsResourceReady() && mMesh.IsResourceReady() ;
#endif
		
	}


	MeshResource* StaticMeshComponent::GetMeshResource()
	{
		return mMesh.GetResource();
	}

	bool StaticMeshComponent::Pick( const Ray& PickRay, float& Distance )
	{
		if ( !IsResourceReady() )
			return false;

		MeshResource* Res = mMesh.GetResource();
		for ( dword i = 0; i< Res->GetVertexFactoryCount();i ++)
		{
			MeshVertexFactory* Factory = Res->GetVertexFactory( i );

			dword VertexSize = Factory->GetFormatData()->GetTotalStride(0);

			
			if ( Intersection::RayVSMesh(	PickRay, 
											Distance,
											VertexSize, 
											Intersection::TriangleList, 
											Factory->GetIndexCount() / 3, 
											(_byte*)Factory->GetVertexStream()->GetBuffer(),
											(_byte*)Factory->GetIndexStream()->GetBuffer()
											 ) )
			{
				return true;
			}
		}

		return false;
	}

	bool StaticMeshComponent::Pick( SceneView* RV, const FRect& ScreenArea )
	{
		if ( !IsResourceReady() )
			return false;



		MeshResource* Res = mMesh.GetResource();
		for ( dword i = 0; i< Res->GetVertexFactoryCount();i ++)
		{
			MeshVertexFactory* Factory = Res->GetVertexFactory( i );

			dword VertexSize = Factory->GetFormatData()->GetTotalStride(0);

			if ( Intersection::RectVSMesh(	ScreenArea,
				RV->GetCamera(), 
				mOwner->GetFullTransform(),
				0.0f,
				VertexSize, 
				Intersection::TriangleList, 
				Factory->GetIndexCount() / 3, 
				(_byte*)Factory->GetVertexStream()->GetBuffer(),
				(_byte*)Factory->GetIndexStream()->GetBuffer()
				) )
			{
				return true;
			}
			
		}


		return false;
	}

}