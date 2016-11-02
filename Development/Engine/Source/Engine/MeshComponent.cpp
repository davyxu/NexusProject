#include "EnginePCH.h"
#include "NekoEngine_Model.h"


namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// Hight Level Texture Object
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( MeshComponent)
	MeshComponent::MeshComponent()
	{

	}

	MeshComponent::~MeshComponent()
	{
		
	}

	void MeshComponent::StaticInitProperty()
	{
		PropertyCollection* PC = RTTI_CLASS_INFO(MeshComponent)->GetPropertyCollection( );
		PC->Add( PROPERTY_DELEGATE( MeshComponent,WString,L"Material", &MeshComponent::OnPropertyMaterial ) );
		
	}

	void MeshComponent::OnPropertyMaterial( WString* Value, bool IsGet )
	{
		if ( IsGet )
		{
			if ( mMaterialInstance.Empty() )
			{
				*Value = L"";
			}
			else
			{
				*Value = mMaterialInstance.GetResource()->GetName();
			}
		}
		else
		{
			SetMaterialInstance( GObjectManager->LoadObject<MaterialInstance>( *Value ) );
		}
	}

	void MeshComponent::SetMaterialInstance( MaterialInstance* MatIns )
	{
		if ( MatIns == null )
			return;

		// 不能在正在更换时又换
		if ( mMaterialInstance.IsDirty() )
			return;

		mMaterialInstance = MatIns;

		CheckResourceStage();
	}

	MaterialInstancePtr MeshComponent::GetMaterial( )
	{
		return mMaterialInstance.GetResource();
	}

	void MeshComponent::CheckResourceStage()
	{
		// 材质加载完毕
		if ( mMaterialInstance.CheckReadyToUpdate( ) )
		{
			mMaterialInstance.GetResource()->LoadShader( RTTI_CLASS_INFO( ModelMaterialVertexShader) );

			mMaterialInstance.ClearFlag();
		}
	}

	void MeshComponent::Draw( SceneView* RV )
	{
		if ( !IsResourceReady() )
			return;

		const Matrix4& FullTransform = mOwner->GetFullTransform();

		Camera* Cam = RV->GetCamera();


		MeshRenderParameter Parameter = {
			(FullTransform * Cam->GetViewMatrix() * Cam->GetProjectMatrix()).Transpose(),
			FullTransform.Transpose(),
			FullTransform.Inverse(),
			mMaterialInstance.GetResource(),
			null,
			null,
			mPickID,
			RV
		};


		MeshResource* MeshRes = GetMeshResource();

		for (dword Index = 0; Index < MeshRes->GetVertexFactoryCount() ;Index++)
		{
			Parameter.mVertexFactory = (VertexFactory*)MeshRes->GetVertexFactory( Index );

			// 骨骼动画需要填充骨骼数据
			OnRenderParameter( Index, Parameter );


			ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
				DrawModelCommand,
				MeshRenderParameter,Parameter, Parameter,
			{
				DrawMesh_RenderThread( Parameter );
			});
		}


		//Canvas* CV = RV->GetCanvas();
		//CV->PushTransform( RV->GetCamera()->GetViewMatrix() * RV->GetCamera()->GetProjectMatrix() );

		//Vector3 XAxis = mOwner->GetDerivedRotation().GetXAxis() * 50.0f;
		//Vector3 YAxis = mOwner->GetDerivedRotation().GetYAxis() * 50.0f;
		//Vector3 ZAxis = mOwner->GetDerivedRotation().GetZAxis() * 50.0f;

		//CV->DrawLine( mOwner->GetDerivedPosition(), XAxis + mOwner->GetDerivedPosition(), Color::cRed );
		//CV->DrawLine( mOwner->GetDerivedPosition(), YAxis + mOwner->GetDerivedPosition(), Color::cGreen );
		//CV->DrawLine( mOwner->GetDerivedPosition(), ZAxis + mOwner->GetDerivedPosition(), Color::cBlue );

		//CV->PopTransform();

	}


	void MeshComponent::DrawMesh_RenderThread( MeshRenderParameter& Parameter )
	{
		Parameter.mVertexFactory->Commit( );


		ModelMaterialVertexShader* VertexShader = Parameter.mMaterialInstance->GetVertexShader<ModelMaterialVertexShader>( );

		if ( VertexShader != null )
		{
			
			// 矩阵
			VertexShader->SetParameter( Parameter.mLocalToScreen, Parameter.mLocalToWorld, Parameter.mRenderView->GetCamera()->GetPosition(), Parameter.mWorldToLocal );

			// 骨骼动画信息
			if ( Parameter.mBoneData != null && Parameter.mBoneData->mSharedBoneCount > 0 )
			{
				VertexShader->SetBoneLocalTransform( &Parameter.mBoneData->mBoneLocalMatrixArray[0], Parameter.mBoneData->mSharedBoneCount );
			}
		}

		if ( Parameter.mPickID > 0 )
		{
			Parameter.mMaterialInstance->CommitShader( true );

			GlobalShaderRef<ColorBrushPixelShader> PixelShaderRef;
			PixelShaderRef->SetColor( Parameter.mPickID );
			PixelShaderRef->Commit();
		}
		else
		{
			Parameter.mMaterialInstance->CommitShader( false );
		}
		
		//TStaticBlendState<>::Commit();
		//TStaticDepthState<>::Commit();

		//TStaticRasterizerState<FM_Solid,CM_CCW>::Commit();


		Parameter.mMaterialInstance->CommitRenderState();


		RHI->Draw( PT_TriangleList, Parameter.mVertexFactory->GetIndexBufferRHI(), Parameter.mVertexFactory->GetVertexCount() );
	}

	bool MeshComponent::IsTransparent()
	{
		if ( !mMaterialInstance.IsResourceReady() )
			return false;

		MaterialInstancePtr Mat = mMaterialInstance.GetResource();

		return Mat->IsTransparent();
	}

	bool MeshComponent::IsEnableDepthWrite( )
	{
		if ( !mMaterialInstance.IsResourceReady() )
			return false;

		MaterialInstancePtr Mat = mMaterialInstance.GetResource();

		return Mat->IsEnableDepthWrite();
	}

	float MeshComponent::GetViewDepthSquared(Camera* C )
	{
		return (mOwner->GetDerivedPosition() - C->GetPosition()).MagnitudeSquared();

		Box BBox = Box( mOwner->GetBoundingBox() );

		Actor* Parent = mOwner->GetParent();
		Assert( Parent != null );
		
		float FinalDistance = Math::cMaxFloat;
		for ( dword i = 0;i < Box::MaxPoints;i++)
		{
			Vector3 WorldPoint = BBox.mCornerPoint[ i ] * Parent->GetFullTransform();
			float Distance = ( WorldPoint - C->GetPosition() ).MagnitudeSquared();

			FinalDistance = Math::Min( Distance, FinalDistance );
		}

		

		return FinalDistance;
	}

	void MeshComponent::Serialize( BinarySerializer& Ser )
	{
		if ( Ser.IsLoading() )
		{

			WString MaterialInstanceName;
			
			Ser << MaterialInstanceName;

			SetObjectPropertyByString( L"Material", MaterialInstanceName.c_str(), true );
		}
		else
		{
			WString MaterialInstanceName;

			GetObjectPropertyAsString( L"Material", MaterialInstanceName, true );

			Ser << MaterialInstanceName;
		}
	}




}