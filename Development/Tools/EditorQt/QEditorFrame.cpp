#include "EditorPCH.h"
#include "PrivateInclude.h"

QEditorFrame* GEditorFrame = null;

QEditorFrame::QEditorFrame(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, mScenePanel( null )
	, mPropertyPanel( null )
{
	ui.setupUi(this);

	GEditorFrame = this;

	
	// 注册类信息
	RTTI_REGISTER( ActorFactoryGizmo );
	RTTI_REGISTER( EditorObject );
	RTTI_REGISTER( ActorFactoryEditor )
	RTTI_REGISTER( EditorDrawUtilityComponent)

	// 初始化属性
	EditorObject::StaticInitProperty();
	QEditorFrame::StaticInitProperty( );
}

QEditorFrame::~QEditorFrame()
{
	delete GTypeMapperManager;

	delete mTimer;
}




void QEditorFrame::OnPropertyAutoSleep( RTTIObject* Instance, bool* Value, bool IsGet )
{
	if ( IsGet )
	{
		*Value = (GEditorFrame->mTimer->interval() != 0);	
	}
	else
	{		
		GEditorFrame->mTimer->setInterval( *Value ? Application::cCpuResondTimeMS:0 );		
	}
}


void QEditorFrame::StaticInitProperty( )
{	
	PropertyCollection* PC = RTTI_CLASS_INFO(GameEngine)->GetPropertyCollection( );

	PropertyGroup* Group = PC->QueryGroup( L"GameEngine" );
	Assert( Group != null );
	PC->Add( PROPERTY_CALLBACK( GameEngine, bool, L"AutoSleep", &QEditorFrame::OnPropertyAutoSleep ), Group );
}

void QEditorFrame::Initialize( )
{
	GTypeMapperManager = new QTypeMapperManager;
	GCommandManager = new CommandManager;

	//////////////////////////////////////////////////////////////////////////
	// UI布局
	//////////////////////////////////////////////////////////////////////////
	connect( ui.actionSaveLayout,SIGNAL(triggered()), this, SLOT(OnSaveLayout()) );
	connect( ui.actionLoad_Layout,SIGNAL(triggered()), this, SLOT(OnLoadLayout()) );

	connect( ui.actionNewScene,SIGNAL(triggered()), this, SLOT(OnNewScene()) );
	connect( ui.actionOpenScene,SIGNAL(triggered()), this, SLOT(OnLoadScene()) );
	connect( ui.actionSaveScene,SIGNAL(triggered()), this, SLOT(OnSaveScene()) );

	// 渲染窗口中的布局

	QTabWidget* Tab = new QTabWidget( this );
	Tab->setObjectName("VisualTab");

	QWidget* RenderWidgetWrapper = new QWidget( Tab );

	mScenePanel = new QScenePanel( RenderWidgetWrapper );
	
	Tab->addTab( RenderWidgetWrapper, tr("Scene"));

	setCentralWidget( Tab );

	// 资源
	AddDock(  mProjectPanel = new QProjectPanel( this ), Qt::LeftDockWidgetArea );

	// 对象
	AddDock(  mHierarchyPanel = new QHierarchyPanel( this ) , Qt::LeftDockWidgetArea );


	// 属性
	AddDock( mPropertyPanel = new QPropertyGrid( this ), Qt::RightDockWidgetArea );

	// 材质
	AddDock( mMaterialPanel = new QMaterialPanel( this ), Qt::TopDockWidgetArea );

	// 材质
	AddDock( mPreviewPanel = new QPreviewPanel( this ), Qt::BottomDockWidgetArea );

	ReadSettings();

	// Timer初始化,引擎new后加载配置文件
	mTimer = new QTimer( this );


	//////////////////////////////////////////////////////////////////////////
	// 引擎初始化
	//////////////////////////////////////////////////////////////////////////

	// 必须先show,否则render窗口的size是错的

	new GameEngine;

	mScenePanel->Initialize( RTTI_CLASS_INFO(ActorFactoryArcBallCamera) );

	mPreviewPanel->Initialize( RTTI_CLASS_INFO(ActorFactoryFocusCamera ) );

	GEngine->Initialize( null );

	// 必须在引擎初始化好shader管理器才可以做这一步
	mScenePanel->GetObjectModifier()->Initialize( mScenePanel->GetWorld() );

	show();

	//@TODO 临时刷新


	connect( mTimer, SIGNAL(timeout()), this, SLOT(OnTick()));

	mTimer->start();

	mProjectPanel->RefreshNode();

	mHierarchyPanel->RefreshNode();

	mScenePanel->RefreshAdjustModeAction( );

	mScenePanel->RefreshAdjustSpaceAction( );

	mMaterialPanel->Initialize();
}

void QEditorFrame::Finalize( )
{
	mMaterialPanel->Finalize();

	GCommandManager->Clear();

	SafeDelete( GCommandManager );

	mScenePanel->ClearMistActor();
	mMaterialPanel->ClearMistNodes();


	GEngine->Finalize();	

	SafeDelete( GEngine );

	
}

QDockWidget* QEditorFrame::AddDock( QWidget* w, Qt::DockWidgetArea Area )
{
	QDockWidget *dockw = new QDockWidget( this );

	dockw->setWindowTitle( w->windowTitle() );

	//@TODO 中文名可能导致错误
	dockw->setObjectName( w->windowTitle() );

	dockw->setWidget( w );

	dockw->setAllowedAreas( Qt::AllDockWidgetAreas );

	addDockWidget(Area, dockw);

	return dockw;
}


void QEditorFrame::OnTick( )
{
	GEngine->Tick();

	mScenePanel->update( );
	mScenePanel->GetObjectModifier()->UpdateGizmo();


	mPreviewPanel->update( );
}

const char* cEditorLayoutFile = "../Config/EditorLayout.ini";
void QEditorFrame::WriteSettings( )
{
	QSettings settings(cEditorLayoutFile, QSettings::IniFormat );
	settings.beginGroup("mainWindow");
	settings.setValue("size", size());
	settings.setValue("state", saveState());
	settings.setValue("pos", pos() );
	settings.endGroup();


}

void QEditorFrame::ReadSettings()
{
	QSettings settings(cEditorLayoutFile, QSettings::IniFormat);
	settings.beginGroup("mainWindow");
	resize(settings.value("size").toSize());
	restoreState(settings.value("state").toByteArray());
	this->move(settings.value("pos").toPoint());
	settings.endGroup();
}

void QEditorFrame::keyReleaseEvent( QKeyEvent * evt )
{
	__super::keyReleaseEvent( evt );

	switch ( evt->key() )
	{
	case Qt::Key_Z:
		{
			if ( evt->modifiers() == Qt::ControlModifier && GCommandManager->CanUndo() )
			{
				GCommandManager->Undo();
				
			}
		}
		return;
	case Qt::Key_Y:
		{
			if ( evt->modifiers() == Qt::ControlModifier && GCommandManager->CanRedo() )
			{
				GCommandManager->Redo();
			}
		}
		return;

	case Qt::Key_F1:
		{
			mScenePanel->GetObjectModifier()->SetGizmoAdjustMode( AdjustMode_Position );
		}
		return;

	case Qt::Key_F2:
		{
			mScenePanel->GetObjectModifier()->SetGizmoAdjustMode( AdjustMode_Rotation );
		}
		return;
	case Qt::Key_F3:
		{
			mScenePanel->GetObjectModifier()->SetGizmoAdjustMode( AdjustMode_Scale );
		}
		return;
	case Qt::Key_F4:
		{
			mScenePanel->GetObjectModifier()->SetGizmoAdjustSpace( (mScenePanel->GetObjectModifier()->GetGizmoAdjustSpace() == AdjustSpace_Local )? AdjustSpace_World:AdjustSpace_Local );
		}
		return;
	}

	NotifyKeyEvent( evt, MKS_Up );
}

void QEditorFrame::NotifyKeyEvent( QKeyEvent * evt, eMessageKeyState MKS )
{
	QRenderPanel* RP = dynamic_cast<QRenderPanel*>( focusWidget() );

	if ( RP == null )
		return;

	MessageInputDriver* InputDriver = (MessageInputDriver*)RP->GetRenderWindow()->GetInputDriver();

	eKeyButton NotifyKey = KB_ForceDWORD;

	if ( evt->key() >= Qt::Key_A  && evt->key() <= Qt::Key_Z )
		NotifyKey = (eKeyButton)evt->key();
	else if ( evt->key() >= Qt::Key_0  && evt->key() <= Qt::Key_9 )
		NotifyKey = (eKeyButton)evt->key();
	else
	{
		switch ( evt->key() )
		{
		case Qt::Key_Control:
			NotifyKey = KB_LeftControl;
			break;
		case Qt::Key_Shift:
			NotifyKey = KB_LeftShift;
			break;
		case Qt::Key_Space:
			NotifyKey = KB_Space;
			break;
		}
	}

	if ( NotifyKey!= KB_ForceDWORD )
		InputDriver->NotifyKeyState( NotifyKey, MKS );
}

void QEditorFrame::keyPressEvent( QKeyEvent * evt )
{
	__super::keyPressEvent( evt );

	NotifyKeyEvent( evt, MKS_Down );
}



void QEditorFrame::OnObjectSelectionChange( EditorObject* Obj )
{
	if ( Obj->GetActorCount() == 0 )
		mPropertyPanel->ClearProperties();
	else
		mPropertyPanel->ReflectProperties( Obj );

	mHierarchyPanel->MarkSelectNode( Obj );
}

void QEditorFrame::OnObjectChangeProperties( EditorObject* Obj )
{
	if ( Obj->GetActorCount() == 0 )
		mPropertyPanel->ClearProperties();
	else
		mPropertyPanel->ReflectProperties( Obj );
}


void QEditorFrame::OnSaveLayout( )
{
	WriteSettings();
}

void QEditorFrame::OnLoadLayout( )
{
	ReadSettings();
}

void QEditorFrame::OnLoadScene( )
{
	//QFileDialog dlg( this, tr("Open Scene File"), QDir::currentPath(), tr("Scene Files(*.scene)"";;All files(*.*)"));
	////dlg.setDirectory( QDir::current());
	////dlg.setViewMode(QFileDialog::Detail);     
	////dlg.setFileMode(QFileDialog::ExistingFiles);     
	////dlg.setAcceptMode(QFileDialog::AcceptOpen);     
	////dlg.setFilter();     
	////dlg.setWindowTitle(tr("Open Scene File"));     
	////dlg.setResolveSymlinks(true);     
	////dlg.setNameFilterDetailsVisible(false); 

	//if(dlg.exec() == QDialog::Accepted)     
	//{     
	//	QStringList filelist = dlg.selectedFiles();

	//	if ( filelist.size() > 0 )
	//	{
	//		mScenePanel->CmdLoadScene( filelist[0].toStdWString().c_str(), false );
	//	}
	//	
	//}     

	mScenePanel->CmdLoadScene( L"t.scene", false );

}

void QEditorFrame::OnSaveScene( )
{
	mScenePanel->CmdSaveScene( L"t.scene" );
}

void QEditorFrame::OnNewScene( )
{
	mScenePanel->CmdClearScene();
}




