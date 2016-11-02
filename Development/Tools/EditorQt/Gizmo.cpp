#include "EditorPCH.h"
#include "PrivateInclude.h"

//////////////////////////////////////////////////////////////////////////
// ActorFactoryGizmo
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_RTTI_CLASS( ActorFactoryGizmo )

ActorFactoryGizmo::ActorFactoryGizmo()
{

}

ActorFactoryGizmo::~ActorFactoryGizmo()
{

}

#define CREATE_GIZMO_PART( Part , Parent )	CreatePart( UTEXT(#Part), Part, Parent )
Actor* ActorFactoryGizmo::CreateActor( const WString& Name, void* Parameter  )
{
	PrepareMaterial();

	Actor* GizmoBase = GObjectManager->ConstructObject<Actor>( Name );
	GizmoBase->SetFlag( EAF_Utility );

	CREATE_GIZMO_PART( AAxis_X, GizmoBase );
	CREATE_GIZMO_PART( AAxis_Y, GizmoBase );
	CREATE_GIZMO_PART( AAxis_Z, GizmoBase );

	CREATE_GIZMO_PART( AAxis_XZ, GizmoBase );
	CREATE_GIZMO_PART( AAxis_XY, GizmoBase );
	CREATE_GIZMO_PART( AAxis_YZ, GizmoBase );

	
	CREATE_GIZMO_PART( AAxisBox_X, GizmoBase );
	CREATE_GIZMO_PART( AAxisBox_Y, GizmoBase );
	CREATE_GIZMO_PART( AAxisBox_Z, GizmoBase );
	

	CREATE_GIZMO_PART( ATriangle_XZ, GizmoBase );
	CREATE_GIZMO_PART( ATriangle_XY, GizmoBase );
	CREATE_GIZMO_PART( ATriangle_YZ, GizmoBase );

	CREATE_GIZMO_PART( ATriBox_XYZ, GizmoBase );

	CREATE_GIZMO_PART( ARing_XZ, GizmoBase );
	CREATE_GIZMO_PART( ARing_XY, GizmoBase );
	CREATE_GIZMO_PART( ARing_YZ, GizmoBase );


	return GizmoBase;
}

Actor* ActorFactoryGizmo::GetGizmoPart( eAdjustAxis Part )
{
	GizmoActorMap::iterator it = mGizmoActorMap.find( Part );

	if ( it == mGizmoActorMap.end() )
		return null;

	return it->second;
}

void ActorFactoryGizmo::SetAdjustMode( eECAdjustMode Mode )
{
	// 隐藏所有
	for ( GizmoActorMap::iterator it = mGizmoActorMap.begin(); 
		it != mGizmoActorMap.end();
		++it )
	{
		Actor* A = it->second;
		if ( A )
			A->SetVisible( false );
	}

	switch ( Mode )
	{
	case AdjustMode_Position:
		{
			GetGizmoPart(AAxis_X)->SetVisible( true );
			GetGizmoPart(AAxis_Y)->SetVisible( true );
			GetGizmoPart(AAxis_Z)->SetVisible( true );

			GetGizmoPart(AAxis_XZ)->SetVisible( true );
			GetGizmoPart(AAxis_XY)->SetVisible( true );
			GetGizmoPart(AAxis_YZ)->SetVisible( true );
		}
		break;
	case AdjustMode_Scale:
		{
			GetGizmoPart(AAxisBox_X)->SetVisible( true );
			GetGizmoPart(AAxisBox_Y)->SetVisible( true );
			GetGizmoPart(AAxisBox_Z)->SetVisible( true );
			GetGizmoPart(ATriangle_XY)->SetVisible( true );
			GetGizmoPart(ATriangle_XZ)->SetVisible( true );
			GetGizmoPart(ATriangle_YZ)->SetVisible( true );
			GetGizmoPart(ATriBox_XYZ)->SetVisible( true );
		}
		break;
	case AdjustMode_Rotation:
		{
			GetGizmoPart(ARing_XY)->SetVisible( true );
			GetGizmoPart(ARing_XZ)->SetVisible( true );
			GetGizmoPart(ARing_YZ)->SetVisible( true );
		}
		break;
	}
}

void ActorFactoryGizmo::SetHighLightGizmoPart( eAdjustAxis Part, bool IsHightLight )
{
	Actor* A = GetGizmoPart(Part);
	if ( A == null )
		return;

	StaticMeshComponent* Component = A->GetComponent<StaticMeshComponent>();
	Component->SetMaterialInstance( GetMaterialByPart( Part, IsHightLight  ) );
}

eAdjustAxis ActorFactoryGizmo::CheckGizmoPart( Actor* A )
{
	for ( GizmoActorMap::iterator it = mGizmoActorMap.begin(); 
		  it != mGizmoActorMap.end();
		  ++it )
	{
		if ( it->second == A )
			return it->first;
	}

	return AAxis_None;
}

Actor* ActorFactoryGizmo::CreatePart( const wchar* Name, eAdjustAxis Part, Actor* Parent )
{
	const wchar* TypeString;
	switch ( Part )
	{
	case AAxis_X:
	case AAxis_Y:
	case AAxis_Z:
		TypeString = L"EngineModel.Editor.Axis.SM";
		break;
	case AAxis_XY:
	case AAxis_XZ:
	case AAxis_YZ:
		TypeString = L"EngineModel.Editor.Plane.SM";
		break;
	case ATriangle_XY:
	case ATriangle_YZ:
	case ATriangle_XZ:
		TypeString = L"EngineModel.Editor.Triangle.SM";
		break;
	case AAxisBox_X:
	case AAxisBox_Y:
	case AAxisBox_Z:
		TypeString = L"EngineModel.Editor.AxisBox.SM";
		break;
	case ATriBox_XYZ:
		TypeString = L"EngineModel.Editor.TriBox.SM";
		break;
	case ARing_XY:
	case ARing_XZ:
	case ARing_YZ:
		TypeString = L"EngineModel.Editor.Ring.SM";
		break;
	default:
		return null;
	}

	Actor* NewActor = GObjectManager->ConstructObject<Actor>( Name );

	NewActor->SetFlag( EAF_Utility );
	NewActor->AddComponent<StaticMeshComponent>();		

	StaticMeshComponent* Component = NewActor->GetComponent<StaticMeshComponent>();

	// 让Gizmo显示在所有对象体的最前面
	Component->SetRenderQueueGroup( RQF_SkiesLater );

	Component->SetMesh( GObjectManager->LoadObject<StaticMeshResource>(TypeString));		

	Component->SetMaterialInstance( GetMaterialByPart( Part, false ) );

	// 片的坐标中心位于体心,为的是半透明排序能正常进行
	const float HalfSize = 4.0f;

	// 轴的本地原点是在箭头,因此以下要根据高度做变换
	const float AxisHeight = 16.0f;

	// 变换矫正
	switch ( Part )
	{
	case AAxis_Z:
	case AAxisBox_Z:
		NewActor->Translate( Vector3( 0, 0, AxisHeight) );
		break;
	case AAxis_X:
	case AAxisBox_X:
		NewActor->Translate( Vector3( AxisHeight, 0.0f, 0.0f ) );
		NewActor->Rotate( Quaternion().RotationY(Math::cHalfPi));
		break;
	case AAxis_Y:
	case AAxisBox_Y:
		NewActor->Rotate( Quaternion().RotationX(-Math::cHalfPi));
		NewActor->Translate( Vector3( 0, AxisHeight, 0.0f ) );
		break;
	case AAxis_XY:
		NewActor->Rotate( Quaternion().RotationX(-Math::cHalfPi));
		NewActor->Translate( Vector3( HalfSize, HalfSize , 0.0f) );
		break;
	case AAxis_XZ:
		NewActor->Translate( Vector3( HalfSize, 0.0f, HalfSize  ) );
		break;
	case AAxis_YZ:
		NewActor->Rotate( Quaternion().RotationZ(Math::cHalfPi));
		NewActor->Translate( Vector3(  0.0f, HalfSize, HalfSize  ) );
		break;
	case ATriangle_XY:
		NewActor->Rotate( Quaternion().RotationX( -Math::cHalfPi ) );
		break;
	case ATriangle_YZ:
		NewActor->Rotate( Quaternion().RotationZ( Math::cHalfPi ) );
		break;
	case ARing_YZ:
		NewActor->Rotate( Quaternion().RotationY( -Math::cHalfPi ) );
		break;
	case ARing_XZ:
		NewActor->Rotate( Quaternion().RotationX( Math::cHalfPi ) );
		break;
	}

	mGizmoActorMap[ Part ] = NewActor;
	NewActor->AddRef();

	if ( Parent != null )
		Parent->AddChild( NewActor );

	return NewActor;
}

void ActorFactoryGizmo::PrepareMaterial()
{
	if ( !mConstantColorMat.Valid() )
	{
		mConstantColorMat = GObjectManager->LoadObject<MaterialDescriptor>(L"EngineMaterial.ConstantColor.MT", false );

		mMIRed			= GenerateMaterial( L"@GizmoAxis_Red"			, mConstantColorMat, Color( 1.0f, 0.7f, 0.0f, 0.0f ) );
		mMIGreen		= GenerateMaterial( L"@GizmoAxis_Green"			, mConstantColorMat, Color( 1.0f, 0.0f, 0.7f, 0.0f ) );
		mMIBlue			= GenerateMaterial( L"@GizmoAxis_Blue"			, mConstantColorMat, Color( 1.0f, 0.0f, 0.0f, 0.7f ) );

		mMIRedHL		= GenerateMaterial( L"@GizmoAxis_RedHL"			, mConstantColorMat, Color( 1.0f, 1.0f, 0.0f, 0.0f ) );
		mMIGreenHL		= GenerateMaterial( L"@GizmoAxis_GreenHL"		, mConstantColorMat, Color( 1.0f, 0.0f, 1.0f, 0.0f ) );
		mMIBlueHL		= GenerateMaterial( L"@GizmoAxis_BlueHL"			, mConstantColorMat, Color( 1.0f, 0.0f, 0.0f, 1.0f ) );

		mMIYellow = GenerateMaterial( L"@GizmoPiece_Yellow"	, mConstantColorMat, Color( 0.3f, 1.0f, 1.0f, 0.0f ) );
		mMIYellowHL = GenerateMaterial( L"@GizmoPiece_YellowHL"	, mConstantColorMat, Color( 0.6f, 1.0f, 1.0f, 0.0f ) );
	}
}

MaterialInstance* ActorFactoryGizmo::GetMaterialByPart( eAdjustAxis Part, bool IsHighLight )
{
	switch ( Part )
	{
	case AAxis_Z:
	case ARing_XY:
	case AAxisBox_Z:
		return IsHighLight ? mMIBlueHL: mMIBlue;
	case AAxis_X:
	case ARing_YZ:
	case AAxisBox_X:
		return IsHighLight ? mMIRedHL : mMIRed;
	case AAxis_Y:
	case ARing_XZ:
	case AAxisBox_Y:
		return IsHighLight ? mMIGreenHL : mMIGreen;
	case AAxis_XY:
	case AAxis_XZ:
	case AAxis_YZ:
	case ATriangle_XY:
	case ATriangle_YZ:
	case ATriangle_XZ:
	case ATriBox_XYZ:
		return IsHighLight ? mMIYellowHL:mMIYellow;
	}


	return null;
}

NekoEngine::MaterialInstancePtr ActorFactoryGizmo::GenerateMaterial( const wchar* MaterialName, MaterialDescriptor* Mat, const Color& C )
{
	MaterialInstance* MI = GObjectManager->ConstructObject<MaterialInstance>( MaterialName );
	MI->LinkMaterial( mConstantColorMat );
	MI->SetColor( L"Color", C );

	if ( C.a < 1.0f )
	{
		MI->SetObjectPropertyT( L"TwoSided", true );
		MI->SetObjectPropertyT( L"BlendMode", (dword)BLEND_Translucent );
	}

	MI->SetObjectPropertyT( L"EnableDepthWrite", false );

	MI->MarkPackageResourceReady();

	return MI;
}

//////////////////////////////////////////////////////////////////////////
// QEditor
//////////////////////////////////////////////////////////////////////////
void ObjectModifier::SetGizmoAdjustMode( eECAdjustMode Mode )
{
	mGizmoAdjustMode = Mode;

	GameWorld* World = GEditorFrame->GetScenePanel()->GetWorld();

	ActorFactoryGizmo* GizmoFactory = World->GetActorFactory<ActorFactoryGizmo>( );
	Assert( GizmoFactory != null );

	GizmoFactory->SetAdjustMode( Mode );

	GEditorFrame->GetScenePanel()->RefreshAdjustModeAction();
}

void ObjectModifier::SetGizmoAdjustSpace( eECAdjustSpace Space )
{
	mGizmoAdjustSpace = Space;

	GEditorFrame->GetScenePanel()->RefreshAdjustSpaceAction();
}


void ObjectModifier::CreateGizmo( )
{
	GameWorld* World = GEditorFrame->GetScenePanel()->GetWorld();

	mGizmo = World->SpawnActor( RTTI_CLASS_INFO(ActorFactoryGizmo), L"@Gizmo" );

	// 初始隐藏Gizmo
	mGizmo->SetVisible( false );

	ActorFactoryGizmo* GizmoFactory = World->GetActorFactory<ActorFactoryGizmo>( );
	Assert( GizmoFactory != null );

	GizmoFactory->SetAdjustMode( AdjustMode_None );
}

void ObjectModifier::UpdateGizmo( )
{
	if ( mSelectedObject->Empty() )
		return;

	Vector3 ObjPos = mSelectedObject->GetDerivedPosition();

	SceneView* View = GEditorFrame->GetScenePanel()->GetRenderWindow()->GetView( 0 );

	float MinSize = (float)Math::Min( View->GetWidth(), View->GetHeight() );

	float Distance = ( ObjPos - View->GetCamera()->GetPosition() ).Magnitude();

	Distance /= ( MinSize / 5.0f );

	// Gizmo根据窗口大小,保持缩放比,不随镜头远近调节
	mGizmo->SetScale( Vector3( Distance, Distance, Distance ) );

	// 缩放及本地空间调节时,Gizmo跟着物体旋转
	if ( mGizmoAdjustSpace == AdjustSpace_Local || mGizmoAdjustMode == AdjustMode_Scale )
	{
		Quaternion ObjRot = mSelectedObject->GetDerivedRotation();
		mGizmo->SetRotation( ObjRot );
	}
	else
	{
		mGizmo->SetRotation( Quaternion::cIdentity );
	}

	mGizmo->SetPosition( ObjPos );
}


void ObjectModifier::BeginAdjustGizmo( const Point& MP )
{
	if ( mSelectedObject->GetActorCount() == 0 )
		return;

	SceneView* View =GEditorFrame->GetScenePanel()->GetRenderWindow()->GetView( 0 );

	Ray PickRay = View->CreateRay( MP.x, MP.y );


	// 确定调节轴
	mEditorAxis = PickGizmoAxis( PickRay );

	if ( mEditorAxis != AAxis_None )
	{
		GameWorld* World = GEditorFrame->GetScenePanel()->GetWorld();

		ActorFactoryGizmo* GizmoFactory = World->GetActorFactory<ActorFactoryGizmo>( );
		Assert( GizmoFactory != null );

		// 高亮调节轴
		GizmoFactory->SetHighLightGizmoPart( mEditorAxis, true );


		mSavedMousePt = Vector2( float(MP.x), float(MP.y) );

		// 保存初始属性
		mLastDerivedRotation = mSelectedObject->GetDerivedRotation();
		mLastDerivedPosition = mSelectedObject->GetDerivedPosition();

		mLastScale = mSelectedObject->GetDerivedScale();

		// 确定射线穿插的平面
		mLastUsedPlane = FindGizmoTranslationPlane( PickRay, mEditorAxis );

		// 保存穿插位置
		mLast3DDelta = GetGizmoIntersect( PickRay, mLastUsedPlane, mEditorAxis, mLastDerivedPosition );


		mIsAdjustingGizmo = true;
	}

}

void ObjectModifier::EndAdjustGizmo( )
{
	GameWorld* World = GEditorFrame->GetScenePanel()->GetWorld();

	ActorFactoryGizmo* GizmoFactory = World->GetActorFactory<ActorFactoryGizmo>( );
	Assert( GizmoFactory != null );

	GizmoFactory->SetHighLightGizmoPart( mEditorAxis, false );

	if ( mIsAdjustingGizmo )
	{
		switch ( mGizmoAdjustMode )
		{
		case AdjustMode_Position:
			{
				if ( mSelectedObject->GetDerivedPosition() == mLastDerivedPosition )
					break;

				GCommandManager->Add( new CmdSetObjProperty( L"Position", value_tostringW( mLastDerivedPosition ) ) );
			}
			break;
		case AdjustMode_Rotation:
			{
				if ( mSelectedObject->GetDerivedRotation() == mLastDerivedRotation )
					break;

				Vector3 EulerValue = Matrix4().FromQuaternion( mLastDerivedRotation ).GetRotator( ).ToEuler();

				GCommandManager->Add( new CmdSetObjProperty( L"Rotation", value_tostringW( EulerValue ) ) );
			}
			break;
		case AdjustMode_Scale:
			{
				if ( mSelectedObject->GetDerivedScale() == mLastScale )
					break;

				GCommandManager->Add( new CmdSetObjProperty( L"Scale", value_tostringW( mLastScale ) ) );
			}
			break;
		}
	}

	mIsAdjustingGizmo = false;
}



void ObjectModifier::AdjustGizmo( const Point& MP )
{
	SceneView* View = GEditorFrame->GetScenePanel()->GetRenderWindow()->GetView( 0 );

	Ray PickRay = View->CreateRay( MP.x, MP.y );

	// 正在调节
	if ( mIsAdjustingGizmo && mSelectedObject->GetActorCount() > 0 )
	{
		Vector3 PickPos = GetGizmoIntersect( PickRay, mLastUsedPlane, mEditorAxis, mLastDerivedPosition );

		switch ( mGizmoAdjustMode )
		{
		case AdjustMode_Position:
			{
				Vector3 FinalPos = PickPos - mLast3DDelta + mLastDerivedPosition;

				mSelectedObject->SetDerivedPosition( FinalPos );
			}
			break;
		case AdjustMode_Scale:
			{
				float PickLength = PickPos.Magnitude();
				float LastLength = mLast3DDelta.Magnitude();

				Vector3 FinalScale = mLastScale;

				// 三轴缩放
				if ( mEditorAxis == ATriBox_XYZ )
				{

					FinalScale = mLastScale - mLastScale * (float(MP.y) - mSavedMousePt.y)/ (float)View->GetHeight() ;
				}
				else
				{
					// 单轴缩放
					if(mEditorAxis & AAxis_X) FinalScale.x *= (PickLength / LastLength);
					if(mEditorAxis & AAxis_Y) FinalScale.y *= (PickLength / LastLength);
					if(mEditorAxis & AAxis_Z) FinalScale.z *= (PickLength / LastLength);
				}


				mSelectedObject->SetDerivedScale( FinalScale );
			}
			break;
		case AdjustMode_Rotation:
			{
				// 垂直鼠标偏移
				float Delta = (float(MP.y) - mSavedMousePt.y);

				Quaternion Rot = mLastDerivedRotation;

				// 单面旋转
				switch( mEditorAxis )
				{
				case ARing_XY:
					Rot *= Quaternion().RotationAxis( Vector3::cZAxis, Math::ToRadian( Delta ) );
					break;
				case ARing_XZ:
					Rot *= Quaternion().RotationAxis( Vector3::cYAxis, Math::ToRadian( Delta ) );
					break;
				case ARing_YZ:
					Rot *= Quaternion().RotationAxis( Vector3::cXAxis, Math::ToRadian( Delta ) );
					break;
				}


				mSelectedObject->SetDerivedRotation( Rot );
			}
			break;
		}

	}
	else
	{
		GameWorld* World = GEditorFrame->GetScenePanel()->GetWorld();

		ActorFactoryGizmo* GizmoFactory = World->GetActorFactory<ActorFactoryGizmo>( );
		Assert( GizmoFactory != null );

		// 关闭前面轴高亮
		GizmoFactory->SetHighLightGizmoPart( mEditorAxis, false );

		// 拾取新的轴
		mEditorAxis = PickGizmoAxis( PickRay );

		// 开启新轴的高亮
		GizmoFactory->SetHighLightGizmoPart( mEditorAxis, true );

	}
}



eAdjustAxis ObjectModifier::PickGizmoAxis( const Ray& PickRay )
{
	GameWorld* World = GEditorFrame->GetScenePanel()->GetWorld();

	WorldPick Pick( World );

	ActorFactoryGizmo* GizmoFactory = World->GetActorFactory<ActorFactoryGizmo>( );
	Assert( GizmoFactory != null );

	// 对结果排序
	if ( Pick.Pick( PickRay, true ) )
	{
		for( dword i = 0;i < Pick.GetCount(); i++)
		{
			Actor* A = Pick.GetActor(i);

			// 获得拾取对象对应的轴
			eAdjustAxis Part = GizmoFactory->CheckGizmoPart( A );
			if ( Part == AAxis_None )
				continue;

			return Part;
		}	
	}

	return AAxis_None;
}

Plane ObjectModifier::FindGizmoTranslationPlane( const Ray& PickRay, dword TranslationAxis )
{
	Quaternion Dir;
	
	// 调节空间不同时,决定是否获取物体的旋转
	if ( mGizmoAdjustSpace == AdjustSpace_Local )
		Dir = mSelectedObject->GetDerivedRotation();
	else
		Dir = Quaternion::cIdentity;

	Vector3 Pos = mSelectedObject->GetDerivedPosition();

	SceneView* View = GEditorFrame->GetScenePanel()->GetRenderWindow()->GetView( 0 );
	Vector3 CamBack = View->GetCamera()->GetViewDir();
	CamBack = -CamBack;

	Plane PlaneX( Pos, Dir.GetXAxis() );
	Plane PlaneY( Pos, Dir.GetYAxis() );
	Plane PlaneZ( Pos, Dir.GetZAxis() );

	float X = PlaneX.ProjectVector( PickRay.GetDirection() ).Magnitude();
	float Y = PlaneY.ProjectVector( PickRay.GetDirection() ).Magnitude();
	float Z = PlaneZ.ProjectVector( PickRay.GetDirection() ).Magnitude();

	if(TranslationAxis & AAxis_X) X = 10000.0f;
	if(TranslationAxis & AAxis_Y) Y = 10000.0f;
	if(TranslationAxis & AAxis_Z) Z = 10000.0f;

	if (X < Y && X < Z)
		return PlaneX;

	if (Y < X && Y < Z )
		return PlaneY;

	return PlaneZ;
}

Vector3 ObjectModifier::GetGizmoIntersect( const Ray& PickRay, const Plane& PlaneToUse, dword TranslationAxis, Vector3& LastPosition )
{
	float PickDistance = 0.0f;
	if ( Intersection::RayVSPlane( PickRay, PlaneToUse, PickDistance ) )
	{
		Vector3 AxisX = Vector3::cZero;
		Vector3 AxisY = Vector3::cZero;
		Vector3 AxisZ = Vector3::cZero;

		Quaternion Dir;

		// 调节空间不同时,决定是否获取物体的旋转
		if ( mGizmoAdjustSpace == AdjustSpace_Local )
			Dir = mSelectedObject->GetDerivedRotation();
		else
			Dir = Quaternion::cIdentity;

		if ( TranslationAxis )
		{
			if(TranslationAxis & AAxis_X) AxisX = Dir.GetXAxis();
			if(TranslationAxis & AAxis_Y) AxisY = Dir.GetYAxis();
			if(TranslationAxis & AAxis_Z) AxisZ = Dir.GetZAxis();
		}
		else
		{
			AxisX = Dir.GetXAxis();
			AxisY = Dir.GetYAxis();
			AxisZ = Dir.GetZAxis();
		}

		Vector3 Proj = PickRay.DistanceVector( PickDistance ) - LastPosition;

		Vector3 PosX = ( AxisX * Vector3::DotProduct( AxisX, Proj ) );
		Vector3 PosY = ( AxisY * Vector3::DotProduct( AxisY, Proj ) );
		Vector3 PosZ = ( AxisZ * Vector3::DotProduct( AxisZ, Proj ) );

		// 合并三轴偏移
		Vector3 Pos = PosX + PosY+ PosZ;

		mLastTranslationDelta = Pos;

		return Pos;
	}

	return mLastTranslationDelta;
}

eECAdjustSpace ObjectModifier::GetGizmoAdjustSpace()
{
	return mGizmoAdjustSpace;
}

eECAdjustMode ObjectModifier::GetGizmoAdjustMode()
{
	return mGizmoAdjustMode;
}
