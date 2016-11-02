#include "EditorPCH.h"
#include "PrivateInclude.h"

inline QRectF ExtendRect( QRectF& Data, qreal Side )
{
	return QRectF( Data.x() - Side, Data.y() - Side, Data.width() + Side * 2, Data.height() + Side * 2 );
}



//////////////////////////////////////////////////////////////////////////
// QMaterialNode
//////////////////////////////////////////////////////////////////////////

QMaterialNode::QMaterialNode(  RTTIClass* NodeClass, QMaterialGraphicesView* View  )
: mCoreNode( RTTI_CAST(MaterialNode, NodeClass->CreateInstance() ) )
, mView( View )
, mOutputSlot( null )
{
	QFontMetrics FM = mView->fontMetrics();	
	mLogicBound.setWidth(	NameWidthMargin + // 名字宽保留
		QMaterialSlot::SlotWidth + // 输出点得宽度
		FM.width( QString::fromStdWString(mCoreNode->GetName()) ) // 名字长度决定节点宽度
		);

	mLogicBound.setHeight(	NameHeight + // 名字高度
		SlotGap +	 // 最上1个的保留间隔
		(QMaterialSlot::SlotHeight + SlotGap ) * Math::Max( mCoreNode->GetSlotCount(), (dword)1 ) // 输入槽的总高度
		);

	mSafeBound = mLogicBound;		
}

QMaterialNode::~QMaterialNode() 
{
	delete mCoreNode;
}


void QMaterialNode::InitializeMembers( QGraphicsScene* Scene )
{
	mGroupItem = new QMaterialNodeGroup( this );

	mGroupItem->addToGroup( this );

	mName = QString::fromStdWString( mCoreNode->GetName() );

	for ( dword i = 0;i< mCoreNode->GetSlotCount();i++)
	{
		MaterialSlot* CoreSlot = mCoreNode->GetSlot( i );

		QMaterialSlot* NewSlot = new QMaterialSlot( mView, this, QString::fromStdWString( CoreSlot->GetName() ) );
		NewSlot->setPos( mLogicBound.width(), NameHeight + SlotGap + (QMaterialSlot::SlotHeight + SlotGap )* i );

		mInputSlots.append( NewSlot );

		mGroupItem->addToGroup( NewSlot );
	}

	if ( !mCoreNode->IsTerminateNode() )
	{		
		mOutputSlot = new QMaterialSlot( mView, this, QString(mName).append("'s OutputSlot") );
		mOutputSlot->setPos( - QMaterialSlot::SlotWidth, (mLogicBound.height() - NameHeight) /2- QMaterialSlot::SlotHeight/2 + NameHeight );

		mGroupItem->addToGroup( mOutputSlot );
	}

	Scene->addItem( mGroupItem );
}

QMaterialSlot* QMaterialNode::GetSlotByName( const QString& Name )
{
	foreach( QMaterialSlot* Slot , mInputSlots )
	{
		if ( Slot->GetName() == Name ) 
			return Slot;
	}

	return null;
}

void QMaterialNode::ReleaseMembers( QGraphicsScene* Scene )
{
	delete mGroupItem;
}




QRectF QMaterialNode::boundingRect() const
{
	return mSafeBound;
}

void QMaterialNode::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{	
	// 背景
	painter->setPen(Qt::NoPen);
	painter->setBrush( Qt::lightGray );
	painter->drawRect( mLogicBound );

	// 名字区域
	painter->setBrush( Qt::gray );
	painter->drawRect( 0,0, mLogicBound.width(), NameHeight );

	painter->setPen(Qt::black);
	painter->drawLine( 0, NameHeight, mLogicBound.width(), NameHeight );
	painter->drawText( 0, 0, mLogicBound.width(), NameHeight, Qt::AlignHCenter| Qt::AlignVCenter, mName );

	// 边框
	
	painter->setBrush(Qt::NoBrush);
	painter->drawRect( mLogicBound );


	// 槽的名称
	dword Index = 0;

	foreach( QMaterialSlot* Slot, mInputSlots )
	{		
		int y = NameHeight + SlotGap + (QMaterialSlot::SlotHeight + SlotGap )* Index;

		painter->drawText( -5, y , mLogicBound.width(), QMaterialSlot::SlotHeight, Qt::AlignRight|Qt::AlignVCenter, Slot->GetName() );				

		Index++;
	}

}





//////////////////////////////////////////////////////////////////////////
// QMaterialSlot
//////////////////////////////////////////////////////////////////////////
QMaterialSlot::QMaterialSlot( QMaterialGraphicesView* View, QMaterialNode* Parent, const QString& Name )
: mParent( Parent )
, mBound( 0,0, SlotWidth, SlotHeight )
, mView( View )
, mName( Name )
{

}

QRectF QMaterialSlot::boundingRect() const
{
	return mBound;
}


void QMaterialSlot::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::darkGray);
	painter->drawRect( mBound );

	if ( mView->GetMouseHoverSlot() == this )
	{
		painter->setPen(Qt::red);
	}
	else
	{
		painter->setPen(Qt::black);
	}

	painter->setBrush(Qt::NoBrush);
	painter->drawRect( mBound );
}



void QMaterialSlot::AddSlotConnection( QMaterialSlot* Slot )
{
	mSlotConnection.append( Slot );
}

void QMaterialSlot::RemoveSlotConnection( QMaterialSlot* Slot )
{
	mSlotConnection.removeAll( Slot );
}

QMaterialSlot* QMaterialSlot::GetOneSlotConnection()
{
	if ( mSlotConnection.size() == 0 )
		return null;

	return mSlotConnection.front();
}


//////////////////////////////////////////////////////////////////////////
// QMaterialArrow
//////////////////////////////////////////////////////////////////////////
QMaterialConnection::QMaterialConnection( QMaterialGraphicesView* View )
: mView( View )
, mOutputSlot( null )
, mInputSlot( null )
{	
	
}


void QMaterialConnection::Reset( QMaterialSlot* OutputSlot, QMaterialSlot* InputSlot )
{
	mOutputSlot = OutputSlot;
	mInputSlot = InputSlot;

	mNodes.clear();

	// 记录父级
	mNodes.push_back( OutputSlot->GetMaterialNode() );
	mNodes.push_back( InputSlot->GetMaterialNode() );

	// 互相添加连接
	OutputSlot->AddSlotConnection( InputSlot );

	InputSlot->AddSlotConnection( OutputSlot );

	UpdateGraphics();
}

bool QMaterialConnection::IsRelateTo( QMaterialNode* Node )
{
	return mNodes.indexOf( Node ) != -1;
}


void QMaterialConnection::UpdateGraphics( )
{
	mOutputPoint = mapFromItem( mOutputSlot, QPointF( 0, mOutputSlot->boundingRect().height() /2 ) );

	QRectF Bound = mInputSlot->boundingRect();
	mInputPoint = mapFromItem( mInputSlot, QPointF(Bound.width() , Bound.height() /2 ) );

	setLine( QLineF( mOutputPoint, mInputPoint ));
}




