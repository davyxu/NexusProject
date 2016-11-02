#include "EditorPCH.h"
#include "PrivateInclude.h"


//////////////////////////////////////////////////////////////////////////
// QHierarchyView
//////////////////////////////////////////////////////////////////////////
QHierarchyPanel::QHierarchyPanel( QWidget* parent )
: QWidget( parent )
, mBlockMarkSelection( false )
, mBlockItemChange( false )
{
	ui.setupUi( this );

	mObjectTree = new QObjectTree( this );

	connect( mObjectTree,SIGNAL(itemSelectionChanged()), this, SLOT(OnItemSelectionChanged()) );


	
	QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem;
	__qtreewidgetitem->setText(0, QString::fromUtf8("1"));
	mObjectTree->setHeaderItem(__qtreewidgetitem);
	mObjectTree->setObjectName(QString::fromUtf8("mObjectTree"));
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(mObjectTree->sizePolicy().hasHeightForWidth());
	mObjectTree->setSizePolicy(sizePolicy);
	mObjectTree->setMinimumSize(QSize(0, 0));
	mObjectTree->setProperty("showDropIndicator", QVariant(false));
	mObjectTree->setDragDropMode(QAbstractItemView::NoDragDrop);
	mObjectTree->header()->setVisible(false);

	mObjectTree->setAcceptDrops( true );
	mObjectTree->setDragDropMode( QAbstractItemView::InternalMove);
		
	ui.verticalLayout->addWidget(mObjectTree);


	//connect( mObjectTree,SIGNAL(itemClicked(QTreeWidgetItem * , int )), this, SLOT(OnItemClicked(QTreeWidgetItem * , int )) );



}


QHierarchyPanel::~QHierarchyPanel()
{

}

void QHierarchyPanel::OnItemSelectionChanged()
{
	if ( mBlockItemChange )
		return;


	QList<QTreeWidgetItem*> SelectedItems = mObjectTree->selectedItems();

	SelectedActorArray SelectedArray;
	SelectedArray.resize( SelectedItems.size() );

	dword Index = 0;
	for ( QList<QTreeWidgetItem*>::iterator it = SelectedItems.begin();
		it != SelectedItems.end();
		++it)
	{
		Assert( mItemToActorMap.contains( *it ) );

		SelectedArray[Index++] = mItemToActorMap[ *it ];
	}

	// 避免修改选择时触发UI修改递归
	mBlockMarkSelection = true;
		GEditorFrame->GetScenePanel()->GetObjectModifier()->CmdSetSelection( SelectedArray );
	mBlockMarkSelection = false;
}

void QHierarchyPanel::FindObject( Actor* A,  handle Parent )
{
	if ( A == null )
		return;

	if ( !A->HasFlag( EAF_EditorObject ) )
		return;

	handle NodeHandle = OnIterateObjectNode( Parent,  A );

	for ( Actor* ChildActor = A->GetChildFirst();
		ChildActor != null;
		ChildActor = A->GetChildNext() )
	{
		FindObject( ChildActor,NodeHandle );
	}
}


void QHierarchyPanel::IterateObject( Actor* RootActor,  handle Root )
{

	for ( Actor* ChildActor = RootActor->GetChildFirst();
		ChildActor != null;
		ChildActor = RootActor->GetChildNext() )
	{
		FindObject( ChildActor,Root );
	}
}


NekoEngine::handle QHierarchyPanel::OnIterateObjectNode( handle Parent, Actor* A )
{
	QTreeWidgetItem* ChildNode;

	if ( Parent == null )
		ChildNode = new QTreeWidgetItem( mObjectTree );
	else
		ChildNode = new QTreeWidgetItem( (QTreeWidgetItem*)Parent);

	ChildNode->setText( 0, QString::fromStdWString( A->GetName() ) );

	mActorToItemMap[ A ] = ChildNode;
	mItemToActorMap[ ChildNode ] = A;


	return ChildNode;
}

void QHierarchyPanel::RefreshNode( )
{
	mObjectTree->clear();
	mActorToItemMap.clear();
	mItemToActorMap.clear();

	IterateObject( GEditorFrame->GetScenePanel()->GetWorld()->GetRootActor(), null );
}

void QHierarchyPanel::MarkSelectNode( EditorObject* Obj )
{
	if ( mBlockMarkSelection )
		return;

	// 避免选择项改变时触发核心选择递归
	mBlockItemChange = true;

	for ( ActorToItemMap::Iterator it = mActorToItemMap.begin();
		it != mActorToItemMap.end();
		++it )
	{
		QTreeWidgetItem* TreeItem = it.value();

		TreeItem->setSelected( Obj->Exists( it.key() ) );
	}

	mBlockItemChange = false;
}


void QHierarchyPanel::CmdChangeParent( Actor* A, Actor* NewParent )
{
	Assert( NewParent != null );

	// 父级没有变化
	if ( A->GetParent() == NewParent )
		return;

	// 自己就是新的父级
	if ( A == NewParent )
		return;

	// 自己是新的父级的节点
	if ( NewParent->ExistChild( A ) )
		return;


	GCommandManager->Add( new CmdActorChangeParent( A, A->GetParent() ) );

	// 将我自己从父级上断开
	if ( A->GetParent() != null )
		A->GetParent()->RemoveChild( A );

	// 将自己加入新的父级
	NewParent->AddChild( A );

	GEditorFrame->GetHierarchyPanel()->RefreshNode();
}

Actor* QHierarchyPanel::GetActorFromNode( QTreeWidgetItem* Item )
{
	if ( !mItemToActorMap.contains( Item ) )
	{
		return null;
	}

	return mItemToActorMap[ Item ];
}


QObjectTree::QObjectTree( QHierarchyPanel *parent /*= 0*/ )
: QTreeWidget( parent )
, mParent( parent )
{

	
}

Actor* QObjectTree::GetActorAtPoint( const QPoint& pos ) 
{
	QTreeWidgetItem* Item = itemAt( pos );
	if ( Item == null )
		return null;

	return mParent->GetActorFromNode( Item );
}

void QObjectTree::mousePressEvent( QMouseEvent *evt )
{
	__super::mousePressEvent( evt );

	if ( evt->button() != Qt::LeftButton )
		return;

	Actor* A = GetActorAtPoint(evt->pos());

	if ( A == null )
		return;

	QMimeData *mimeData = new QMimeData;

	QByteArray exData;
	QDataStream dataStream (&exData, QIODevice::WriteOnly);
	dataStream	<< (int)A;

	mimeData->setData(cDragDropType_AdjustParent, exData );

	QDrag* drag = new QDrag(this);
	drag->setMimeData( mimeData );

	drag->exec();
}


void QObjectTree::dragEnterEvent( QDragEnterEvent *evt )
{
	if ( !evt->mimeData()->hasFormat( cDragDropType_AdjustParent ) )
		return;

	evt->acceptProposedAction();
}


void QObjectTree::dropEvent( QDropEvent *evt )
{
	Actor* NewParent = GetActorAtPoint( evt->pos() );

	if ( NewParent == null )
		return;


	QByteArray exData = evt->mimeData()->data(cDragDropType_AdjustParent);

	QDataStream dataStream (&exData, QIODevice::ReadOnly);

	Actor* ToBeChild = null;

	int Pointer = 0;
	dataStream >> Pointer;
	ToBeChild = (Actor*)Pointer;

	mParent->CmdChangeParent( ToBeChild, NewParent );
}

bool QObjectTree::dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action)
{
	QList<QUrl> urlList;
	QTreeWidgetItem *item;

	urlList = data->urls(); // retrieve list of urls

	foreach(QUrl url, urlList) // iterate over list
	{
		// make new QTreeWidgetItem and set its text
		// if parent is null - add top level item (this parent)
		if (parent == NULL) item = new QTreeWidgetItem(this); 
		else 
			// else add QTreeWidgetItem with parent and expand parent
		{    
			item = new QTreeWidgetItem(parent);
			parent->setExpanded( true );
		}

		// set item text
		item->setText( 0, url.toLocalFile() );
	}

	return true;    
}


QStringList QObjectTree::mimeTypes () const
{
	QStringList qstrList;
	// list of accepted mime types for drop
	qstrList.append(cDragDropType_AdjustParent);
	return qstrList;
}


Qt::DropActions QObjectTree::supportedDropActions () const
{
	// returns what actions are supported when dropping
	return Qt::CopyAction | Qt::MoveAction;
}