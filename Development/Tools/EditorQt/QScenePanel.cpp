#include "EditorPCH.h"
#include "PrivateInclude.h"


QScenePanel::QScenePanel( QWidget* Parent /*= NULL */ )
: QRenderPanel( Parent )
{
	// 工具栏
	QToolBar* menuTools = new QToolBar(tr("QRenderWidgetToolBar"), Parent );

	actionObjectTranslate = new QAction(this);
	actionObjectTranslate->setObjectName(QString::fromUtf8("Action.Object.Translate"));
	actionObjectTranslate->setCheckable(true);
	actionObjectTranslate->setText(QApplication::translate("this", "Translate", 0, QApplication::UnicodeUTF8));
	actionObjectTranslate->setToolTip(QApplication::translate("this", "Translate selected objects", 0, QApplication::UnicodeUTF8));

	actionObjectRotate = new QAction(this);
	actionObjectRotate->setObjectName(QString::fromUtf8("Action.Object.Rotate"));
	actionObjectRotate->setCheckable(true);
	actionObjectRotate->setText(QApplication::translate("this", "Rotate", 0, QApplication::UnicodeUTF8));
	actionObjectRotate->setToolTip(QApplication::translate("this", "Rotate selected objects", 0, QApplication::UnicodeUTF8));

	actionObjectScale = new QAction(this);
	actionObjectScale->setObjectName(QString::fromUtf8("Action.Object.Scale"));
	actionObjectScale->setCheckable(true);
	actionObjectScale->setText(QApplication::translate("this", "Scale", 0, QApplication::UnicodeUTF8));
	actionObjectScale->setToolTip(QApplication::translate("this", "Scale selected objects", 0, QApplication::UnicodeUTF8));

	actionAdjustSpace = new QAction(this);
	actionAdjustSpace->setObjectName(QString::fromUtf8("Action.Object.AdjustSpace"));
	actionAdjustSpace->setText(QApplication::translate("this", "AdjustSpace", 0, QApplication::UnicodeUTF8));
	actionAdjustSpace->setToolTip(QApplication::translate("this", "Modify gizmo adjust space", 0, QApplication::UnicodeUTF8));

	actionFreeView = new QAction(this);
	actionFreeView->setObjectName(QString::fromUtf8("Action.Object.FreeView"));
	actionFreeView->setCheckable(true);
	actionFreeView->setText(QApplication::translate("this", "FreeView", 0, QApplication::UnicodeUTF8));

	menuTools->addAction(actionFreeView);
	menuTools->addAction(actionObjectTranslate);
	menuTools->addAction(actionObjectRotate);
	menuTools->addAction(actionObjectScale);
	menuTools->addSeparator();
	menuTools->addAction(actionAdjustSpace);
	menuTools->addSeparator();


	connect( actionFreeView,SIGNAL(triggered()), this, SLOT(OnActionFreeView()) );
	connect( actionObjectTranslate,SIGNAL(triggered()), this, SLOT(OnActionObjectTranslate()) );
	connect( actionObjectRotate,SIGNAL(triggered()), this, SLOT(OnActionObjectRotate()) );
	connect( actionObjectScale,SIGNAL(triggered()), this, SLOT(OnActionObjectScale()) );

	connect( actionAdjustSpace,SIGNAL(triggered()), this, SLOT(OnActionAdjustSpace()) );

	// 页面内布局
	QVBoxLayout* vbox = new QVBoxLayout( Parent );
	vbox->setMargin( 0 );
	vbox->setSpacing( 0 );
	vbox->addWidget( menuTools );
	vbox->addWidget( this );
	Parent->setLayout( vbox );

	this->setAcceptDrops( true );

	mObjectModifier = new ObjectModifier;
}

QScenePanel::~QScenePanel()
{

	delete mObjectModifier;
}


void QScenePanel::dragEnterEvent( QDragEnterEvent *evt )
{
	QStringList sl = evt->mimeData()->formats();

	if ( !evt->mimeData()->hasFormat( cDragDropType_ProjectSpawn ) )
		return;

	quint32 ResourceType = RFT_Unknown;
	
	QByteArray exData = evt->mimeData()->data(cDragDropType_ProjectSpawn);

	QDataStream dataStream (&exData, QIODevice::ReadOnly);
	dataStream >> ResourceType;

	switch ( ResourceType )
	{
	case RFT_StaticMesh:
	case RFT_SkeletonMesh:
		evt->accept( );
		break;
	}
}

 
void QScenePanel::dropEvent( QDropEvent *evt )
{
	QByteArray exData = evt->mimeData()->data(cDragDropType_ProjectSpawn);

	QDataStream dataStream (&exData, QIODevice::ReadOnly);

	quint32 ResourceType = RFT_Unknown;

	QString FileName;
	dataStream >> ResourceType >> FileName;

	Actor* A = null;
	switch ( ResourceType )
	{
	case RFT_StaticMesh:
		A = CmdAddActorViaStaticMesh( FileName.toStdWString() );
		break;
	case RFT_SkeletonMesh:
		A = CmdAddActorViaSkeletonMesh( FileName.toStdWString() );
		break;
	default:
		return;
	}

	if ( A == null )
		return;


	Vector3 SpawnPosition = mObjectModifier->GetGroupSpawnPoint( Point( evt->pos().x(), evt->pos().y() ) );

	A->SetPosition( SpawnPosition );
	
}




void QScenePanel::OnActionFreeView( )
{
	mObjectModifier->SetGizmoAdjustMode( AdjustMode_None );
	RefreshAdjustModeAction( );
}

void QScenePanel::OnActionObjectTranslate( )
{
	mObjectModifier->SetGizmoAdjustMode( AdjustMode_Position );
	RefreshAdjustModeAction( );
}

void QScenePanel::OnActionObjectRotate( )
{
	mObjectModifier->SetGizmoAdjustMode( AdjustMode_Rotation );
	RefreshAdjustModeAction( );
}

void QScenePanel::OnActionObjectScale( )
{
	mObjectModifier->SetGizmoAdjustMode( AdjustMode_Scale );
	RefreshAdjustModeAction( );
}

void QScenePanel::OnActionAdjustSpace( )
{
	eECAdjustSpace Space = ( mObjectModifier->GetGizmoAdjustSpace() == AdjustSpace_Local ) ? AdjustSpace_World:AdjustSpace_Local;
	mObjectModifier->SetGizmoAdjustSpace( Space );

	RefreshAdjustModeAction( );
}



void QScenePanel::RefreshAdjustModeAction( )
{
	actionFreeView->setChecked( mObjectModifier->GetGizmoAdjustMode() == AdjustMode_None );
	actionObjectTranslate->setChecked( mObjectModifier->GetGizmoAdjustMode() == AdjustMode_Position );
	actionObjectRotate->setChecked( mObjectModifier->GetGizmoAdjustMode() == AdjustMode_Rotation );
	actionObjectScale->setChecked( mObjectModifier->GetGizmoAdjustMode() == AdjustMode_Scale );	
}

void QScenePanel::RefreshAdjustSpaceAction( )
{
	switch ( mObjectModifier->GetGizmoAdjustSpace() )
	{
	case AdjustSpace_Local:
		actionAdjustSpace->setText( QString::fromUtf8("Local") );
		break;
	case AdjustSpace_World:
		actionAdjustSpace->setText( QString::fromUtf8("World") );
		break;
	}
}




void QScenePanel::mousePressEvent( QMouseEvent *evt )
{
	__super::mousePressEvent( evt );

	Point MP( evt->x(), evt->y() );

	eMouseButton Button;
	switch ( evt->button() )
	{
	case Qt::LeftButton:
		mObjectModifier->OnMouseLeftButtonDown( MP );	
		Button = MB_Left;
		break;
	case Qt::RightButton:
		Button = MB_Right;
		break;
	case Qt::MidButton:
		Button = MB_Middle;
		break;
	default:
		return;
	}
}



void QScenePanel::mouseReleaseEvent( QMouseEvent *evt )
{
	Point MP( evt->x(), evt->y() );

	eMouseButton Button;
	switch ( evt->button() )
	{
	case Qt::LeftButton:
		mObjectModifier->OnMouseLeftButtonUp( MP );
		break;
	case Qt::RightButton:
		Button = MB_Right;
		break;
	case Qt::MidButton:
		Button = MB_Middle;
		break;
	default:
		return;
	}

}

void QScenePanel::Initialize( RTTIClass* CameraControllerType )
{
	__super::Initialize( CameraControllerType );	
}


void QScenePanel::mouseMoveEvent( QMouseEvent *evt )
{
	__super::mouseMoveEvent( evt );

	Point MP( evt->x(), evt->y() );	

	mObjectModifier->OnMouseMove( MP );
}


Actor* QScenePanel::CmdAddActorViaStaticMesh( const WString& ResourceName, Actor* Parent )
{	

	Actor* A = mWorld->SpawnActor( RTTI_CLASS_INFO(ActorFactoryStaticMesh), L"", Parent );
	{
		StaticMeshComponent* Component = A->GetComponent<StaticMeshComponent>();
		Component->SetMesh( GObjectManager->LoadObject<StaticMeshResource>( ResourceName ));
		Component->SetMaterialInstance( GObjectManager->LoadObject<MaterialInstance>(L"EngineMaterial.DiffuseLighting.MI") );
	}


	CmdAddActorCommon( A );

	return A;
}

Actor* QScenePanel::CmdAddActorViaSkeletonMesh( const WString& ResourceName, Actor* Parent )
{	

	Actor* A = mWorld->SpawnActor( RTTI_CLASS_INFO(ActorFactorySkeletonMesh), L"", Parent );
	{
		SkeletonMeshComponent* Component = A->GetComponent<SkeletonMeshComponent>();
		Component->SetSkin( GObjectManager->LoadObject<SkeletonMeshResource>( ResourceName ));
		Component->SetMaterialInstance( GObjectManager->LoadObject<MaterialInstance>(L"EngineMaterial.SpecularLighting.MI") );
	}


	CmdAddActorCommon( A );

	return A;
}

Actor* QScenePanel::CmdAddActorCommon( Actor* A )
{
	ActorArray Actors;
	Actors.push_back( A );
	GCommandManager->Add( new CmdActorAdd( Actors ) );

	A->SetFlag( EAF_EditorObject );

	GEditorFrame->GetHierarchyPanel()->RefreshNode();

	return A;
}

void QScenePanel::CmdRemoveActor( const ActorArray& Actors )
{
	GCommandManager->Add( new CmdActorRemove( Actors ) );

	for ( dword i = 0;i< Actors.size();i++ )
	{
		Actor* A = Actors[i];

		GameWorld::IsolateFromSystem( A );

		A->GetParent()->RemoveChild( A );
	}

	GEditorFrame->GetHierarchyPanel()->RefreshNode();
}


void QScenePanel::CmdClearScene( )
{		
	Actor* RootActor = mWorld->GetRootActor();

	ActorArray SavedRootActors;


	for ( Actor* Child = RootActor->GetChildFirst();
		Child != null;
		Child = RootActor->GetChildNext() )
	{
		// 必须是编辑对象
		if ( !Child->HasFlag( EAF_EditorObject ) )
			continue;

		SavedRootActors.push_back( Child );
	}

	CmdRemoveActor( SavedRootActors );
}


void QScenePanel::CmdSaveScene( const wchar* FileName )
{
	FileStream Stream( FileName, FAM_Write );
	if ( !Stream.IsValid() )
		return;

	BinarySerializer Ser( &Stream, false );

	Ser.SerializerFileHeader( FILE_VER_SCENE );

	mWorld->SerializeActors( Ser, mWorld->GetRootActor(), EAF_EditorObject );
}



void QScenePanel::CmdLoadScene( const wchar* FileName, bool MergeToScene )
{
	GCommandManager->BeginCollection( L"Load Scene");

	if ( !MergeToScene )
	{
		CmdClearScene();
	}

	//GEditorFrame->GetHierarchyPanel()->RefreshNode();

	FileStream Stream( FileName, FAM_Read );
	if ( !Stream.IsValid() )
		return;

	BinarySerializer Ser( &Stream, true );

	Ser.SerializerFileHeader( 0 );

	if ( !Ser.IsStreamOK() )
	{
		return;
	}

	ActorArray RootChildActors;

	mWorld->SerializeActors( Ser, mWorld->GetRootActor(), EAF_None, &RootChildActors );

	GEditorFrame->GetHierarchyPanel()->RefreshNode();

	GCommandManager->Add( new CmdActorAdd( RootChildActors ) );

	GCommandManager->EndCollection( true );

}

void QScenePanel::AddMistActor( Actor* A )
{
	if ( mMistActors.find( A ) != mMistActors.end() )
		return;

	mMistActors.insert( A );
}

void QScenePanel::ClearMistActor( )
{
	foreach( Actor* A, mMistActors )
	{
		mWorld->Kill( A );
	}

	mMistActors.clear();
}
