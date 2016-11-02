#include "EnginePCH.h"
#include "NekoEngine_Model.h"

namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS( SkeletonMeshComponent)

	SkeletonMeshComponent::SkeletonMeshComponent()
		: mSkeletonInstance( null )
		, mSkeletonAnimController( null )
	{

	}

	SkeletonMeshComponent::~SkeletonMeshComponent()
	{
		delete mSkeletonInstance;
		delete mSkeletonAnimController;
	}

	void SkeletonMeshComponent::StaticInitProperty()
	{
		PropertyCollection* PC = RTTI_CLASS_INFO(SkeletonMeshComponent)->GetPropertyCollection( );
		PC->Add( PROPERTY_DELEGATE( SkeletonMeshComponent,WString,L"Skin", &SkeletonMeshComponent::OnPropertySkin ) );
		PC->Add( PROPERTY_DELEGATE( SkeletonMeshComponent,WString,L"Animation", &SkeletonMeshComponent::OnPropertyAnimation ) );	
	}

	void SkeletonMeshComponent::OnPropertySkin( WString* Value, bool IsGet )
	{
		if ( IsGet )
		{
			if ( mSkin.Empty() )
			{
				*Value = L"";
			}
			else
			{
				*Value = mSkin.GetResource()->GetName();
			}
		}
		else
		{
			SetSkin( GObjectManager->LoadObject<SkeletonMeshResource>( *Value ) );
		}
	}

	void SkeletonMeshComponent::OnPropertyAnimation( WString* Value, bool IsGet )
	{
		if ( IsGet )
		{
			if ( mAnimation.Empty() )
			{
				*Value = L"";
			}
			else
			{
				*Value = mAnimation.GetResource()->GetName();
			}
		}
		else
		{
			SetAnimation( GObjectManager->LoadObject<SkeletonAnimResource>( *Value ) );
		}
	}



	void SkeletonMeshComponent::SetSkin( const SkeletonMeshResourcePtr& Skin )
	{
		if ( Skin.Empty() )
			return;

		// 不能在正在更换时又换
		if ( mSkin.IsDirty() )
			return;

		mSkin = Skin;

		mOwner->SetDebugName( StringHelper::FormatString( L"%s#%s", mOwner->GetName().c_str(), Skin->GetName().c_str() ));
	}

	void SkeletonMeshComponent::SetAnimation( const SkeletonAnimResourcePtr& Anim )
	{
		// 不能在正在更换时又换
		if ( mAnimation.IsDirty() )
			return;

		mAnimation = Anim;
	}

	void SkeletonMeshComponent::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		if ( Ser.IsLoading() )
		{
			WString SkinName, AnimName;

			Ser << SkinName << AnimName;

			SetObjectPropertyByString( L"Skin", SkinName.c_str() );
			SetObjectPropertyByString( L"Animation", AnimName.c_str() );
		}
		else
		{
			WString SkinName, AnimName;

			GetObjectPropertyAsString( L"Skin", SkinName );
			GetObjectPropertyAsString( L"Animation", AnimName );

			Ser << SkinName << AnimName;
		}
	}


	void SkeletonMeshComponent::Tick( float elapse )
	{
		if ( IsResourceReady() )
		{
			if ( mSkeletonInstance != null )
			{
				// 更新骨架
				mSkeletonInstance->Tick( elapse );

				// 根据骨架计算绑定盒
				mOwner->SetBoundingBox( mSkeletonInstance->CaculateSkeletonBoundingBox(  ) );			
			}

			
			// 动画更新
			if ( mSkeletonAnimController != null )
			{
				mSkeletonAnimController->Update( elapse);
			}
		}
		else
		{
			CheckResourceStage();
		}

	}

	bool SkeletonMeshComponent::IsResourceReady()
	{
		return  mMaterialInstance.IsResourceReady() && 
				mSkin.IsResourceReady();
	}

	void SkeletonMeshComponent::CheckResourceStage( )
	{
		Super::CheckResourceStage();

		// 骨骼和模型加载完毕
		if ( mSkin.CheckReadyToUpdate() )
		{
			SkeletonMeshResource* Skin = mSkin.GetResource();

			// 从资源拷贝一份骨骼做实例 
			delete mSkeletonInstance;
			mSkeletonInstance = new SkeletonInstance( Skin );

			mSkin.ClearFlag();
		}

		// 动画加载完毕
		if ( mAnimation.CheckReadyToUpdate() && mSkeletonInstance != null )
		{
			SkeletonAnimResource* Anim = mAnimation.GetResource();

			delete mSkeletonAnimController;
			mSkeletonAnimController = new SkeletonAnimController( Anim, mSkeletonInstance );

			mAnimation.ClearFlag();
		}
	}


	MeshResource* SkeletonMeshComponent::GetMeshResource()
	{
		return mSkin.GetResource();
	}


	void SkeletonMeshComponent::OnRenderParameter( dword Index, MeshRenderParameter& Parameter )
	{
		SkeletonMeshResource* Skin = mSkin.GetResource();
		Parameter.mBoneData = Skin->GenBoneIndexMatrix( mBoneShaderBuffer, mSkeletonInstance, Index );
	}

	bool SkeletonMeshComponent::Pick( const Ray& PickRay, float& Distance )
	{
		if ( !IsResourceReady() )
			return false;

		MeshResource* Res = mSkin.GetResource();
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


	bool SkeletonMeshComponent::Pick( SceneView* RV, const FRect& ScreenArea )
	{
		if ( !IsResourceReady() )
			return false;



		MeshResource* Res = mSkin.GetResource();
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






