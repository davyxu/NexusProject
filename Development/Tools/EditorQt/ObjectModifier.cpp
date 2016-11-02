#include "EditorPCH.h"
#include "PrivateInclude.h"




ObjectModifier::ObjectModifier()
: mEditorAxis( AAxis_None )
, mLastTranslationDelta( Vector3::cZero )
, mIsAdjustingGizmo( false )
, mIsVolumeSelecting( false )
, mGizmoAdjustMode( AdjustMode_None )
, mGizmoAdjustSpace( AdjustSpace_World )
, mSelectedObject( null )
{	



	mSelectedObject = new EditorObject;
}


ObjectModifier::~ObjectModifier( )
{
	SafeDelete( mSelectedObject );
}


void ObjectModifier::Initialize( GameWorld* World )
{
	mWorld = World;

	mEditorHelper = World->SpawnActor( RTTI_CLASS_INFO(ActorFactoryEditor), L"@EditorHelper" );
	

	CreateGizmo();	

	// 初始调试代码放这里
}




Actor* ObjectModifier::PickSingleObject( const Ray& PickRay )
{

	WorldPick Pick( mWorld ) ;

	ActorFactoryGizmo* GizmoFactory = mWorld->GetActorFactory<ActorFactoryGizmo>( );
	Assert( GizmoFactory != null );

	// 对结果排序
	if ( Pick.Pick( PickRay, true ) )
	{
		for( dword i = 0;i < Pick.GetCount(); i++)
		{
			Actor* A = Pick.GetActor(i);

			// 不取Gizmo
			eAdjustAxis Part = GizmoFactory->CheckGizmoPart( A );
			if ( Part != AAxis_None )
				continue;

			return A;
		}	
	}

	return null;
}


void ObjectModifier::OnMouseLeftButtonDown( const Point& MP )
{
	BeginAdjustGizmo( MP );

	// 没有选中的轴,就可以开始框选
	if ( mEditorAxis == AAxis_None )
	{
		mVolumeSelectPt1 = mVolumeSelectPt2 = MP;
		mIsVolumeSelecting = true;
	}
	
}

void ObjectModifier::OnMouseLeftButtonUp( const Point& MP )
{
	if ( mIsAdjustingGizmo )
	{
		EndAdjustGizmo( );

		GEditorFrame->OnObjectChangeProperties( mSelectedObject );

		// 有调整过物体属性时,需要更新Node的大小
		mSelectedObject->UpdateWrapper();
	}
	else
	{	
		if ( mIsVolumeSelecting )
		{
			if (!VolumeSelect( mVolumeSelectPt1, mVolumeSelectPt2))
			{
				// 如果区域选择区域太小,进行单选
				SingleSelect( MP );
			}

			mIsVolumeSelecting = false;
		}
		else
		{
			// 没有多选时,单选
			SingleSelect( MP );
		}
	
	}

	
}


void ObjectModifier::OnMouseMove( const Point& MP )
{
	AdjustGizmo( MP );

	// 更新框选目标
	if ( mIsVolumeSelecting )
	{
		mVolumeSelectPt2 = MP;
	}
}





EditorObject* ObjectModifier::GetSelectedObject()
{
	return mSelectedObject;
}



void ObjectModifier::CmdSetSelection( const SelectedActorArray& SelectedActor, bool KeepOld, bool Remove )
{
	EditorObject* SelObj = null;

	// 检查是否有重复选择
	bool SameSelection = true;

	// 选择量一样,检查内容
	if ( SelectedActor.size() == mSelectedObject->GetActorCount() )
	{
		for ( dword i = 0;i< SelectedActor.size();i ++ )
		{
			Actor* A = SelectedActor[i];

			if ( !mSelectedObject->Exists( A ) )
			{
				SameSelection = false;
				break;
			}
		}
	}
	else
	{
		SameSelection = false;
	}

	// 有相同的选择不进行cmd记录, 但是消除选择除外
	if ( SameSelection && !Remove )
		return;



	CmdActorSelect* Cmd = new CmdActorSelect( GetSelectedObject() );
	if ( SelectedActor.size() > 0 )
	{
		// 不需要保持老内容, 不需要减选时,清除当前选择
		if ( !KeepOld && !Remove )
		{
			mSelectedObject->Clear();	
		}


		for ( dword i = 0;i< SelectedActor.size();i ++ )
		{
			Actor* A = SelectedActor[i];

			// 减选
			if ( Remove )
			{
				mSelectedObject->Remove( A );
			}
			else
			{
				// 存在的取消选择, 没有的,选择
				if ( mSelectedObject->Exists( A ) )
					mSelectedObject->Remove( A );
				else
					mSelectedObject->Add( SelectedActor[i] );
			}
		}

		mSelectedObject->UpdateWrapper();
	}
	else
	{
		mSelectedObject->Clear();
	}


	GCommandManager->Add( Cmd );

	mGizmo->SetVisible( mSelectedObject->GetActorCount() > 0 );

	GEditorFrame->OnObjectSelectionChange( mSelectedObject );
}


void ObjectModifier::SingleSelect( const Point& MP )
{
	SceneView* View = GEditorFrame->GetScenePanel()->GetRenderWindow()->GetView( 0 );

	Ray PickRay = View->CreateRay( MP.x, MP.y );

	Actor* A = PickSingleObject( PickRay );

	SelectedActorArray SelectedArray;

	if ( A != null )
		SelectedArray.push_back( A );

	IInputDriver* InputDrv = GEditorFrame->GetScenePanel()->GetRenderWindow()->GetInputDriver();

	CmdSetSelection(	SelectedArray, 
		InputDrv->IsKeyDown(KB_LeftControl),
		InputDrv->IsKeyDown(KB_LeftAtl) );
}

Vector3 ObjectModifier::GetGroupSpawnPoint( const Point& MP )
{
	SceneView* View = GEditorFrame->GetScenePanel()->GetRenderWindow()->GetView( 0 );

	Ray PickRay = View->CreateRay( MP.x, MP.y );

	float Distance = 0.0f;
	Plane::Intersect( PickRay, Plane::cXYPlane, Distance );

	return PickRay.DistanceVector( Distance );
}

bool ObjectModifier::VolumeSelect( const Point& Pt1, const Point& Pt2 )
{
	int Left	= Math::Min( Pt1.x, Pt2.x ) + 1;
	int Right	= Math::Max( Pt1.x, Pt2.x ) - 1;
	int Top		= Math::Min( Pt1.y, Pt2.y ) + 1;
	int Bottom	= Math::Max( Pt1.y, Pt2.y ) - 1;


	const float MinPickDistance = 3.0f;

	const float MaxPickDistance = 20.0f;

	const int MinAreaSelect = 4;
	if ( ( Right - Left ) <= MinAreaSelect || ( Bottom - Top ) <= MinAreaSelect )
		return false;

	SceneView* View = GEditorFrame->GetScenePanel()->GetRenderWindow()->GetView( 0 );

	PlaneBoundedVolume BoundVolume( View->CreateRay( Left, Top ), 
		View->CreateRay( Right, Top), 
		View->CreateRay( Left, Bottom), 
		View->CreateRay( Right, Bottom ),
		MinPickDistance,
		MaxPickDistance
		);

	GameWorld* World = GEditorFrame->GetScenePanel()->GetWorld();

	WorldPick Pick( World );

	if ( !Pick.Pick( BoundVolume ) )
		return true;

	FRect ScreenArea( (float)Left / View->GetWidth(), (float)Top / View->GetHeight(), (float)Right / View->GetWidth(), (float)Bottom/ View->GetHeight() );


	SelectedActorArray SelectedArray;

	// 遍历绑定盒拾取结果
	for( dword i = 0;i < Pick.GetCount(); i++)
	{
		Actor* A = Pick.GetActor(i);

		// 不能拾取编辑器组件
		if ( A->HasFlag( EAF_Utility) )
			continue;

		// Mesh精确拾取
		if (!A->Pick( View, ScreenArea ))
			continue;

		SelectedArray.push_back( A );
	}

	IInputDriver* InputDrv = GEditorFrame->GetScenePanel()->GetRenderWindow()->GetInputDriver();

	CmdSetSelection(	SelectedArray, 
		InputDrv->IsKeyDown(KB_LeftControl),
		InputDrv->IsKeyDown(KB_LeftAtl) );

	return true;
}