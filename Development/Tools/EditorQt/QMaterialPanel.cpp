#include "EditorPCH.h"
#include "PrivateInclude.h"


//////////////////////////////////////////////////////////////////////////
// QMaterialPanel
//////////////////////////////////////////////////////////////////////////
QMaterialPanel::QMaterialPanel( QWidget* parent )
: QWidget( parent )
{
	ui.setupUi( this );

	//----------------------------------------------
	//|
	//----------------------------------------------
	//|
	//|				QMaterialGraphicesView
	//|
	//----------------------------------------------
	//|							|
	//|  QMaterialElementTree	|	QPropertyGrid	
	//|							|
	//----------------------------------------------

	mGraphicsScene = new QMaterialScene( this );
	mGraphicsScene->setItemIndexMethod(QGraphicsScene::NoIndex);
	mGraphicsScene->setSceneRect(0,0, 400, 400);

	mGraphicsView = new QMaterialGraphicesView( this );

	mGraphicsView->setScene( mGraphicsScene );

	

	// 简单容器
	QVBoxLayout* DummyLayout = new QVBoxLayout( this );
	DummyLayout->setContentsMargins(2, 2, 2, 2);

	// 垂直分割
	QSplitter* VerticalSpliter = new QSplitter( Qt::Vertical, this );

	VerticalSpliter->setContentsMargins(0, 0, 0, 0);


	QToolBar* menuTools = new QToolBar(tr("QMaterialToolBar"), this );


	mActionLoad = new QAction(this);
	mActionLoad->setObjectName(QString::fromUtf8("Action.Material.Load"));	
	mActionLoad->setText(QApplication::translate("this", "Load", 0, QApplication::UnicodeUTF8));
	mActionLoad->setToolTip(QApplication::translate("this", "Load a material", 0, QApplication::UnicodeUTF8));
	connect( mActionLoad,SIGNAL(triggered()), this, SLOT(OnActionLoadMaterial() ) );


	mActionReset = new QAction(this);
	mActionReset->setObjectName(QString::fromUtf8("Action.Material.Reset"));	
	mActionReset->setText(QApplication::translate("this", "Reset", 0, QApplication::UnicodeUTF8));
	mActionReset->setToolTip(QApplication::translate("this", "Clear all material node and connections", 0, QApplication::UnicodeUTF8));

	connect( mActionReset,SIGNAL(triggered()), this, SLOT(OnActionReset()) );


	mActionBuild = new QAction(this);
	mActionBuild->setObjectName(QString::fromUtf8("Action.Material.Build"));	
	mActionBuild->setText(QApplication::translate("this", "Build", 0, QApplication::UnicodeUTF8));
	mActionBuild->setToolTip(QApplication::translate("this", "Build the material", 0, QApplication::UnicodeUTF8));
	
	connect( mActionBuild,SIGNAL(triggered()), this, SLOT(OnActionBuildMaterial()) );

	mActionUpdateToSelected = new QAction(this);
	mActionUpdateToSelected->setObjectName(QString::fromUtf8("Action.Material.UpdateToSelected"));	
	mActionUpdateToSelected->setText(QApplication::translate("this", "UpdateToSelected", 0, QApplication::UnicodeUTF8));
	mActionUpdateToSelected->setToolTip(QApplication::translate("this", "Update to selected object after build finished", 0, QApplication::UnicodeUTF8));
	mActionUpdateToSelected->setCheckable(true);
	connect( mActionBuild,SIGNAL(triggered()), this, SLOT(OnActionUpdateToSelected()) );


	menuTools->addAction(mActionLoad);
	
	menuTools->addAction(mActionReset);

	menuTools->addAction(mActionBuild);

	menuTools->addAction(mActionUpdateToSelected);

	QShortcut* BuildMat = new QShortcut(QKeySequence(tr("F7")), this);

	connect( BuildMat, SIGNAL( activated () ), this, SLOT( OnActionBuildMaterial() ) );

	VerticalSpliter->addWidget( menuTools );

	VerticalSpliter->addWidget( mGraphicsView );

	QSplitter* HorizontalSpliter = new QSplitter( Qt::Horizontal, this );

	HorizontalSpliter->setContentsMargins(0, 0, 0, 0);

	// 创建树
	HorizontalSpliter->addWidget( new QMaterialElementTree( this ) );

	mMemberProperty = new QPropertyGrid( this, false );
	// 属性
	HorizontalSpliter->addWidget( mMemberProperty );


	VerticalSpliter->addWidget( HorizontalSpliter );

	DummyLayout->addWidget( VerticalSpliter );

}

void QMaterialPanel::OnActionReset( )
{
	CmdReset();	
}

void QMaterialPanel::OnActionBuildMaterial( )
{
	mGraphicsScene->SaveMaterial( L"EngineMaterial.EditorMaterial.MT");
}

void QMaterialPanel::OnActionUpdateToSelected( )
{

}

void QMaterialPanel::OnActionLoadMaterial( )
{
	CmdLoadMaterial( "EngineMaterial.EditorMaterial.MT" );
}



void QMaterialPanel::Initialize( )
{

	mGraphicsScene->CreateRoot();
}

void QMaterialPanel::Finalize( )
{
	
}


QMaterialNode* QMaterialPanel::CmdSpawnNode( RTTIClass* NodeClass )
{
	QMaterialNode* Node = mGraphicsScene->CreateNode( NodeClass );

	QMaterialNodeList CreatedNodes;
	CreatedNodes.push_back( Node );

	GCommandManager->Add( new CmdMaterialNodeCreate( CreatedNodes ) );

	return Node;
}

void QMaterialPanel::CmdDeleteNode( QMaterialNode* Node )
{
	if ( Node == mGraphicsScene->GetRoot() )
		return;

	QMaterialNodeList ToBeDeleteNode;
	ToBeDeleteNode.push_back( Node );

	GCommandManager->Add( new CmdMaterialNodeDelete( ToBeDeleteNode ) );

	mGraphicsScene->DetachNode( Node );
}

void QMaterialPanel::CmdLoadMaterial( const QString& Name )
{
	GCommandManager->BeginCollection(L"Load Material");

	mGraphicsScene->LoadMaterial( Name.toStdWString() );

	GCommandManager->EndCollection( true );
}

void QMaterialPanel::CmdReset( )
{
	QList<QGraphicsItem*> Items = mGraphicsScene->items( );

	QMaterialNodeList NodesToDelete;
	foreach( QGraphicsItem* Item, Items)
	{
		QMaterialNodeGroup* Group = dynamic_cast<QMaterialNodeGroup*>( Item );
		if ( Group == null )
			continue;

		/// 根不能删除
		if ( Group->GetNode() == mGraphicsScene->GetRoot() )
			continue;

		NodesToDelete.push_back( Group->GetNode() );
	}

	GCommandManager->Add( new CmdMaterialNodeDelete( NodesToDelete ) );

	foreach( QMaterialNode* Node, NodesToDelete )
	{
		mGraphicsScene->DetachNode( Node );
	}
}

void QMaterialPanel::CmdConnectSlot( QMaterialSlot* OutputSlot, QMaterialSlot* InputSlot )
{
	GCommandManager->Add( new CmdMaterialSlotConnect( InputSlot ) );

	mGraphicsScene->ConnectSlot( OutputSlot, InputSlot );
}

void QMaterialPanel::CmdDisconnectSlot( QMaterialSlot* Slot )
{
	GCommandManager->Add( new CmdMaterialSlotDisconnect( Slot ) );

	mGraphicsScene->DisconnectSlot( Slot );
}



QMaterialPanel::~QMaterialPanel()
{

}





//////////////////////////////////////////////////////////////////////////
// QMaterialViewport
//////////////////////////////////////////////////////////////////////////
QMaterialGraphicesView::QMaterialGraphicesView(  QMaterialPanel* Panel )
: QGraphicsView( Panel )
, mMouseHoverSlot( null )
, mPanel( Panel )
, mBrushElementBG( QColor( 10, 10, 10 ) )
{
	setMouseTracking( true );
	setCacheMode(CacheBackground);
	setTransformationAnchor(NoAnchor );	
	setViewportUpdateMode(BoundingRectViewportUpdate);

	setAcceptDrops( true );
}

void QMaterialGraphicesView::wheelEvent(QWheelEvent *event)
{
	scaleView(pow((double)2, event->delta() / 240.0));
}

void QMaterialGraphicesView::scaleView(qreal scaleFactor)
{
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
		return;

	scale(scaleFactor, scaleFactor);
}

void QMaterialGraphicesView::mouseMoveEvent( QMouseEvent *event )
{
	__super::mouseMoveEvent( event );

	QMaterialSlot* Slot = dynamic_cast<QMaterialSlot*>( this->itemAt( event->pos() ) );


	if ( mMouseHoverSlot == Slot )
		return;


	if ( mMouseHoverSlot )
		mMouseHoverSlot->update();

	if ( Slot )
		Slot->update();

	mMouseHoverSlot = Slot;
}



//////////////////////////////////////////////////////////////////////////
// QMaterialElementTree
//////////////////////////////////////////////////////////////////////////
QMaterialElementTree::QMaterialElementTree( QMaterialPanel *parent /*= 0*/ )
: QTreeWidget( parent )
, mParent( parent )
{
	QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem;
	__qtreewidgetitem->setText(0, QString::fromUtf8("1"));
	setHeaderItem(__qtreewidgetitem);
	setObjectName(QString::fromUtf8("mElementTree"));

	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth( sizePolicy.hasHeightForWidth());

	setSizePolicy(sizePolicy);

	setMinimumSize(QSize(0, 0));

	setProperty("showDropIndicator", QVariant(false));

	setDragDropMode(QAbstractItemView::NoDragDrop);

	header()->setVisible(false);

	setAcceptDrops( true );
	setDragDropMode( QAbstractItemView::InternalMove);



	QTreeWidgetItem* TextureNode = AddType( tr("Texture"));
	AddElement( TextureNode, RTTI_CLASS_INFO(MaterialNodeTextureSampler), tr("Texture Sampler"), tr("Texture input and uv settings") );

	QTreeWidgetItem* MathNode = AddType( tr("Math"));
	AddElement( MathNode, RTTI_CLASS_INFO(MaterialNodeMathOperator), tr("Math Operator"), tr("Do math between multiply slots") );
	AddElement( MathNode, RTTI_CLASS_INFO(MaterialNodeNormalize), tr("Normalize"), tr("Normalize a vector") );

	QTreeWidgetItem* UtilityNode = AddType( tr("Utility"));
	AddElement( UtilityNode, RTTI_CLASS_INFO(MaterialNodeBumpOffset), tr("Bump Offset"), tr("Bump map calculator") );

	QTreeWidgetItem* ConstantNode = AddType( tr("Constant"));
	AddElement( ConstantNode, RTTI_CLASS_INFO(MaterialNodeConstant), tr("Constant"), tr("A constant in edit time") );

	QTreeWidgetItem* UniformNode = AddType( tr("Uniform"));
	AddElement( UniformNode, RTTI_CLASS_INFO(MaterialNodeUniformVector), tr("Uniform Vector"), tr("A vector suply by host") );

}

QTreeWidgetItem* QMaterialElementTree::AddType( const QString& Name )
{
	QTreeWidgetItem* Item = new QTreeWidgetItem( this );
	Item->setText( 0, Name );

	return Item;
}

void QMaterialElementTree::AddElement( QTreeWidgetItem* Parent, RTTIClass* ClassInfo, const QString& Name, const QString& Desc )
{
	QTreeWidgetItem* Item;

	if ( Parent == null )
		Item = new QTreeWidgetItem( this );
	else
		Item = new QTreeWidgetItem( Parent );

	Item->setText( 0,  Name );
	Item->setToolTip( 0, Desc );

	mItemToClassMap[Item] = ClassInfo;
}

RTTIClass* QMaterialElementTree::GetClassFromItem( QTreeWidgetItem* Item )
{
	if ( !mItemToClassMap.contains( Item ) )
	{
		return false;
	}

	return mItemToClassMap[ Item ];
}


void QMaterialElementTree::mousePressEvent( QMouseEvent *evt )
{
	__super::mousePressEvent( evt );

	if ( evt->button() != Qt::LeftButton )
		return;

	QTreeWidgetItem* Item = itemAt( evt->pos() );
	if ( Item == null )
		return;

	RTTIClass* Class = GetClassFromItem( Item );

	if ( Class == null )
		return;

	QMimeData *mimeData = new QMimeData;

	QByteArray exData;
	QDataStream dataStream (&exData, QIODevice::WriteOnly);
	dataStream	<< (int)Class;

	mimeData->setData(cDragDropType_MaterialSpawn, exData );

	QDrag* drag = new QDrag(this);
	drag->setMimeData( mimeData );

	drag->exec();
}

void QMaterialPanel::AddMistNode( QMaterialNode* Node )
{
	if ( mMistNodes.find( Node ) != mMistNodes.end() )
		return;

	mMistNodes.insert( Node );
}

void QMaterialPanel::ClearMistNodes( )
{
	foreach( QMaterialNode* Node, mMistNodes )
	{
		mGraphicsScene->DetachNode( Node );

		// 删除Group后,Node会被自动删除
		Node->ReleaseMembers( mGraphicsScene );	
	
	}

	mMistNodes.clear();
}
