#include "EditorPCH.h"
#include "PrivateInclude.h"

void QProjectPanel::ScanDirectory( const wchar* PathName, handle Parent  )
{
	WString DirName = PathName;


	if ( DirName.length( ) > 0 )
		DirName += L"\\";

	FileFinder	Finder;
	FileFinder::FileInfo	Info;


	if ( Finder.FindFirst( (DirName + L"*").c_str(), Info ) )
	{
		do
		{
			if ( Info.mName == L"." || Info.mName == L".." )
				continue;

			// Found a file.
			if ( ( Info.mAttributes & FileFinder::Attribute_Directory ) == 0 )
			{
				// Return false means stop enum
				OnIterateFileNode( Parent, Info.mName.c_str(), (DirName + Info.mName).c_str() );

			}
			else
			{
				WString combinepath = DirName + Info.mName;

				if ( Directory::IsHidden( combinepath.c_str() ) )
					continue;

				handle NodeHandle = OnIterateFileNode( Parent, Info.mName.c_str(), combinepath.c_str() );

				ScanDirectory( combinepath.c_str(),  NodeHandle  );
			}
		}
		// Find next file.
		while ( Finder.FindNext( Info ) );
	}		
}



//////////////////////////////////////////////////////////////////////////
// QProjectView
//////////////////////////////////////////////////////////////////////////
QProjectPanel::QProjectPanel( QWidget* parent )
: QWidget( parent )
{
	ui.setupUi( this );

	mFileTree = new QFileTree(this);
	QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem;
	__qtreewidgetitem->setText(0, QString::fromUtf8("1"));
	mFileTree->setHeaderItem(__qtreewidgetitem);
	mFileTree->setObjectName(QString::fromUtf8("FileTree"));
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(mFileTree->sizePolicy().hasHeightForWidth());
	mFileTree->setSizePolicy(sizePolicy);
	mFileTree->setMinimumSize(QSize(0, 0));
	mFileTree->setProperty("showDropIndicator", QVariant(false));
	mFileTree->setDragDropMode(QAbstractItemView::NoDragDrop);
	mFileTree->header()->setVisible(false);

	ui.verticalLayout->addWidget(mFileTree);



	connect( mFileTree,SIGNAL(itemClicked(QTreeWidgetItem * , int )), this, SLOT(OnItemClicked(QTreeWidgetItem * , int )) );
}

void QProjectPanel::OnItemClicked(  QTreeWidgetItem * item, int column )
{
	//if ( mItemToStringMap.contains( item ) )
	//{
	//	WString FullPath = mItemToStringMap[ item ];
	//	WString ResourceName = GResourceMaster->ResourceName_Build( FullPath.c_str() );
	//	if ( ResourceMaster::ResourceName_ParseType( ResourceName ) == RFT_Unknown )
	//	{
	//		ResourceName = L"";
	//	}	
	//}
	
}

bool QProjectPanel::GetResourceNameFromItem( QTreeWidgetItem* Item, WString& ResourceName, eResourceType& RT )
{
	if ( !mItemToStringMap.contains( Item ) )
	{
		return false;
	}

	WString FullPath = mItemToStringMap[ Item ];

	ResourceName = ResourceLocation::Build( FullPath );

	RT = ResourceLocation::ParseType( ResourceName );

	return RT != RFT_Unknown ;
}

void QProjectPanel::RefreshNode( )
{
	mFileTree->clear();

	ScanDirectory( L"..\\Content", null );	
}

QProjectPanel::~QProjectPanel()
{

}


NekoEngine::handle QProjectPanel::OnIterateFileNode( handle Parent, const wchar* Name, const wchar* FullPathName )
{
	QTreeWidgetItem* ChildNode;

	if ( Parent == null )
		ChildNode = new QTreeWidgetItem( mFileTree );
	else
		ChildNode = new QTreeWidgetItem( (QTreeWidgetItem*)Parent);

	ChildNode->setText( 0, QString::fromWCharArray( Name ) );
	ChildNode->setToolTip( 0, QString::fromWCharArray( FullPathName ));
	mItemToStringMap[ ChildNode ] = FullPathName;

	return ChildNode;
}


QFileTree::QFileTree( QProjectPanel *parent /*= 0*/ )
: QTreeWidget( parent )
, mParent( parent )
{
	
}

void QFileTree::mousePressEvent( QMouseEvent *evt )
{
	__super::mousePressEvent( evt );

	if ( evt->button() != Qt::LeftButton )
		return;

	QTreeWidgetItem* Item = itemAt( evt->pos() );
	if ( Item == null )
		return;

	WString ResourceName;
	eResourceType ResourceType;
	if (!mParent->GetResourceNameFromItem( Item, ResourceName, ResourceType ))
		return;


	QMimeData *mimeData = new QMimeData;
	
	QByteArray exData;
	QDataStream dataStream (&exData, QIODevice::WriteOnly);
	dataStream	<< (quint32)ResourceType 
				<< QString::fromStdWString( ResourceName );

	mimeData->setData(cDragDropType_ProjectSpawn, exData );

	QDrag* drag = new QDrag(this);
	drag->setMimeData( mimeData );

	drag->exec();
}
