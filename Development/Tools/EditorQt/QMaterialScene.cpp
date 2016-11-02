#include "EditorPCH.h"
#include "PrivateInclude.h"



//////////////////////////////////////////////////////////////////////////
// QMaterialScene
//////////////////////////////////////////////////////////////////////////


QMaterialScene::QMaterialScene( QMaterialPanel* Parent )
: QGraphicsScene( Parent )
, mPanel( Parent )
, mPickedNode( null )
, mConnectLine( null )
, mSourceSlot( null )
, mSelectedNode( null )
, mSelectedSlot( null )
, mRoot( null )
{	
	mActionDisconnect = new QAction(this);
	mActionDisconnect->setObjectName(QString::fromUtf8("Action.Material.Slot.Disconnect"));	
	mActionDisconnect->setText(QApplication::translate("this", "Disconnect", 0, QApplication::UnicodeUTF8));
	mActionDisconnect->setToolTip(QApplication::translate("this", "Disconnect the connection between node and slot", 0, QApplication::UnicodeUTF8));
	connect( mActionDisconnect,SIGNAL(triggered()), this, SLOT(OnActionDisconnect()) );

	mActionDeleteNode = new QAction(this);
	mActionDeleteNode->setObjectName(QString::fromUtf8("Action.Material.Node.Delete"));	
	mActionDeleteNode->setText(QApplication::translate("this", "Delete", 0, QApplication::UnicodeUTF8));
	mActionDeleteNode->setToolTip(QApplication::translate("this", "Delete the node", 0, QApplication::UnicodeUTF8));
	connect( mActionDeleteNode,SIGNAL(triggered()), this, SLOT(OnActionDeleteNode()) );
}

QMaterialScene::~QMaterialScene( )
{
	// 删除未托管的连接
	foreach( QMaterialConnection* Connection, mFreeConnectionList )
	{
		delete Connection;
	}

	mFreeConnectionList.clear();
}


void QMaterialScene::OnActionDisconnect( )
{
	if( mSelectedSlot == null )
		return;

	mPanel->CmdDisconnectSlot( mSelectedSlot );
}


void QMaterialScene::OnActionDeleteNode( )
{
	if( mSelectedNode == null )
		return;

	mPanel->CmdDeleteNode( mSelectedNode );
}





void QMaterialScene::dragEnterEvent( QGraphicsSceneDragDropEvent *evt )
{
	// 材质创建拖放
	if ( evt->mimeData()->hasFormat( cDragDropType_MaterialSpawn ) )
	{
		evt->accept( );
	}
	else
	{
		// 普通拖放
		__super::dragEnterEvent( evt );
	}
}

void QMaterialScene::dragMoveEvent( QGraphicsSceneDragDropEvent *evt )
{
	// 材质创建拖放
	if ( evt->mimeData()->hasFormat( cDragDropType_MaterialSpawn ) )
	{
		evt->accept( );
	}
	else
	{
		// 普通拖放
		__super::dragMoveEvent( evt );
	}
}

void QMaterialScene::dropEvent( QGraphicsSceneDragDropEvent *evt )
{
	__super::dropEvent( evt );

	QByteArray exData = evt->mimeData()->data(cDragDropType_MaterialSpawn);

	QDataStream dataStream (&exData, QIODevice::ReadOnly);


	int Data;
	dataStream >> Data;

	QMaterialNode* Node = mPanel->CmdSpawnNode( (RTTIClass*)Data );

	const QRectF& Bound = Node->GetLogicBound();

	Node->group()->setPos( evt->scenePos() - QPointF( Bound.width() /2, Bound.height()/2) );	
}


void QMaterialScene::mousePressEvent( QGraphicsSceneMouseEvent *evt )
{
	__super::mousePressEvent( evt );

	switch ( evt->button() )
	{
	case Qt::LeftButton:
		{
			QGraphicsItem* PickedItem = itemAt( evt->scenePos() );

			if ( PickedItem == null )
				return;


			QMaterialNode* Node = dynamic_cast<QMaterialNode*>( PickedItem );

			// 拾取到节点
			if ( Node != null )
			{
				// 找到他所在的组
				QGraphicsItemGroup* Group = Node->group();

				if ( Group == null )
					return;

				// 开始移动组
				mPickedNode = Node;

				mPickedNodeConnections = GetNodeConnections( Node );


				mSavedPos = evt->scenePos() - Group->pos();

				// 记录移动的节点
				QMaterialNodeList ToBeMovedNode;
				ToBeMovedNode.push_back( mPickedNode );

				GCommandManager->Add( new CmdMaterialNodeMove( ToBeMovedNode ) );

				return;
			}

			QMaterialSlot* Slot = dynamic_cast<QMaterialSlot*>( PickedItem );

			// 拾取到槽
			if ( Slot != null )
			{
				mConnectLine = new QGraphicsLineItem( QLineF( evt->scenePos(), evt->scenePos()) );
				mConnectLine->setPen( QPen(Qt::black, 2) );

				addItem( mConnectLine );

				mSourceSlot = Slot;
			}
		}
		break;
	case Qt::RightButton:
		{
			mSelectedNode = null;
			mSelectedSlot = null;
		}
		break;
	}

}

void QMaterialScene::mouseMoveEvent(QGraphicsSceneMouseEvent *evt)
{
	__super::mouseMoveEvent( evt );

	// 拖动连接线
	if ( mConnectLine != null )
	{
		QLineF Org = mConnectLine->line();
		mConnectLine->setLine( QLineF(Org.p1(), evt->scenePos()) );
	}


	// 拖动节点
	if ( mPickedNode != null && mPickedNode->group() )
	{

		mPickedNode->group()->setPos( evt->scenePos() - mSavedPos );

		// 更新Node上的连线位置
		// 遍历所有连接，找到与此节点相关的连接进行更新
		foreach( QMaterialConnection* Connecion, mPickedNodeConnections )
		{
			Connecion->UpdateGraphics();
		}
	}
}

void QMaterialScene::UpdateNodeConnections( QMaterialNode* Node )
{
	// 遍历所有连接，找到与此节点相关的连接进行更新
	foreach( QMaterialConnection* Connecion, GetNodeConnections( Node ) )
	{
		Connecion->UpdateGraphics();
	}
}

void QMaterialScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *evt)
{
	__super::mouseReleaseEvent( evt );

	switch ( evt->button() )
	{
	case Qt::LeftButton:
		{
			mPickedNode = null;

			if ( mConnectLine != null )
			{
				QList<QGraphicsItem *> PickedItem = items( evt->scenePos() );

				// 从上到下,找到一个合适的槽
				foreach( QGraphicsItem* Item, PickedItem )
				{
					QMaterialSlot* Slot = dynamic_cast<QMaterialSlot*>( Item );

					// 槽不能是源槽
					if ( Slot == null || mSourceSlot == Slot )
						continue;

					// 必须为不同类型的连接槽
					if ( mSourceSlot->IsOutputSlot() == Slot->IsOutputSlot() )			
						break;

					if ( mSourceSlot->IsOutputSlot() )
					{
						Math::Swap( mSourceSlot, Slot );
					}


					mPanel->CmdConnectSlot( Slot, mSourceSlot );

					break;
				}


				this->removeItem( mConnectLine );
				delete mConnectLine;		
				mConnectLine = null;
			}
		}
		break;
	}


}

void QMaterialScene::contextMenuEvent( QGraphicsSceneContextMenuEvent *evt )
{
	QGraphicsItem* PickedItem = itemAt( evt->scenePos() );

	if ( PickedItem == null )
		return;

	QMaterialNode* Node = dynamic_cast<QMaterialNode*>( PickedItem );

	// 拾取到节点
	if ( Node != null )
	{
		QMenu* PopupMenu = new QMenu( mPanel->GetGraphicsView() );
		PopupMenu->addAction( mActionDeleteNode );

		mSelectedNode = Node;

		PopupMenu->exec( QCursor::pos());		
	}

	QMaterialSlot* Slot = dynamic_cast<QMaterialSlot*>( PickedItem );

	if ( Slot != null )
	{
		QMenu* PopupMenu = new QMenu( mPanel->GetGraphicsView() );
		PopupMenu->addAction( mActionDisconnect );

		mSelectedSlot = Slot;

		PopupMenu->exec( QCursor::pos());		
	}
}

QMaterialNode* QMaterialScene::CreateNode( RTTIClass* NodeType ) 
{
	QMaterialNode* NewNode = new QMaterialNode( NodeType, mPanel->GetGraphicsView() );

	NewNode->InitializeMembers( this );

	return NewNode;
}

void QMaterialScene::CreateRoot( )
{
	mRoot = CreateNode( RTTI_CLASS_INFO(MaterialNodePointLight) );
}




void QMaterialScene::ConnectSlot( QMaterialSlot* OutputSlot, QMaterialSlot* InputSlot )
{
	Assert( OutputSlot->IsOutputSlot() );
	Assert( !InputSlot->IsOutputSlot() );

	QMaterialConnection* Connection = AllocConnection();

	Connection->Reset( OutputSlot, InputSlot );

	addItem( Connection  );
}

QList<QMaterialConnection*> QMaterialScene::GetNodeConnections( QMaterialNode* Node )
{
	QList<QMaterialConnection*> Connections;
	foreach( QMaterialConnection* Connecion, mConnectionList )
	{
		if ( Connecion->IsRelateTo( Node ) )
			Connections.push_back( Connecion );
	}

	return Connections;
}

QMaterialConnection* QMaterialScene::AllocConnection( )
{
	QMaterialConnection* Connection;

	if ( mFreeConnectionList.size() > 0 )
	{
		Connection = mFreeConnectionList.back();
		mFreeConnectionList.pop_back();		
	}
	else
	{
		Connection = new QMaterialConnection( mPanel->GetGraphicsView() );
	}

	mConnectionList.push_back( Connection );

	return Connection;
}

void QMaterialScene::FreeConnection( QMaterialSlot* Slot )
{
	for( QList<QMaterialConnection*>::iterator it = mConnectionList.begin();
		it != mConnectionList.end();
		)
	{
		QMaterialConnection* Connection = *it;

		bool NeedRemove = false;

		if ( Slot->IsOutputSlot() )
		{
			NeedRemove = (Connection->GetOutputSlot() == Slot );
		}
		else
		{
			NeedRemove = (Connection->GetInputSlot() == Slot );
		}

		if ( NeedRemove )
		{
			// 从场景删除
			removeItem( Connection );

			// 将废弃的连接保存起来
			mFreeConnectionList.push_back( Connection );

			it = mConnectionList.erase( it );
		}
		else
		{
			++it;
		}
	}
}

void QMaterialScene::DisconnectSlot( QMaterialSlot* Slot )
{
	// 遍历所有连接
	foreach (QMaterialSlot *ConnectedSlot, Slot->GetSlotConnections())
	{
		// 将连接的输入端移除这个槽
		ConnectedSlot->RemoveSlotConnection( Slot );
	}

	// 清除槽连接的所有的输出连接
	Slot->GetSlotConnections().clear();

	// 回收连接对象
	FreeConnection( Slot );
}
void QMaterialScene::DetachNode( QMaterialNode* Node )
{
	QGraphicsItemGroup* Group = Node->group();
	if ( Group == null )
		return;

	// 遍历节点上所有的输入节点，断开
	foreach( QMaterialSlot* InputSlot, Node->GetInputSlots() )
	{
		DisconnectSlot( InputSlot );
	}

	// 断开输出节点
	DisconnectSlot( Node->GetOutputSlot() );

	removeItem( Group );
}


inline Vector2 ToVector2( const QPointF& Value )
{
	return Vector2( Value.x(), Value.y() );
}

inline QPointF ToPointF( const Vector2& Value )
{
	return QPointF( Value.x, Value.y );
}

void QMaterialScene::SaveMaterial( const WString& Name )
{
	Assert( mRoot != null );

	MaterialDescriptor* MatDesc = GObjectManager->ConstructObject<MaterialDescriptor>( Name );

	if ( MatDesc == null )
		return;


	// 添加到生成器中
	foreach( QGraphicsItem* Item, items() )
	{
		QMaterialNodeGroup* Group = dynamic_cast<QMaterialNodeGroup*>( Item );
		if ( Group == null )
			continue;

		QMaterialNode* Node = Group->GetNode();

		MaterialNode* CoreNode = MatDesc->AddNode( Node->GetMataerialNodeClass() );

		Node->mIDForSave = CoreNode->GetID();

		// 将节点位置保存在核心节点中
		CoreNode->SetObjectPropertyT<Vector2>(L"ScenePos", ToVector2( Node->scenePos() ) );
	}

	// 连接槽
	foreach( QMaterialConnection* Connection, mConnectionList )
	{
		MaterialNode* InputCoreNode = MatDesc->GetNode( Connection->GetInputSlot()->GetMaterialNode()->mIDForSave );
		
		MaterialSlot* InputCoreSlot = InputCoreNode->GetSlotByName( Connection->GetInputSlot()->GetName().toStdWString().c_str() );

		InputCoreSlot->Connect( Connection->GetOutputSlot()->GetMaterialNode()->mIDForSave );
	}

	GObjectManager->SaveToFile( MatDesc, FILE_VER_MATERIAL_DESCRIPTOR );

	GObjectManager->DestructObject( MatDesc );
}

void QMaterialScene::FindCoreConnections( MaterialNode* Node, QMap<MaterialNode*, QMaterialNode*>& CoreToQNodeMap )
{
	for ( dword i = 0; i< Node->GetSlotCount(); i++)
	{
		MaterialSlot* Slot = Node->GetSlot( i );

		MaterialNode* LinkedNode = Slot->GetLinkedNode();

		if ( LinkedNode != null )
		{			
			mPanel->CmdConnectSlot( CoreToQNodeMap[LinkedNode]->GetOutputSlot(), CoreToQNodeMap[Node]->GetSlotByName( QString::fromWCharArray( Slot->GetName() ) ) );

			FindCoreConnections( LinkedNode, CoreToQNodeMap );
		}
	}
}

void QMaterialScene::LoadMaterial( const WString& Name )
{
	MaterialDescriptor* MatDesc = GObjectManager->LoadObject<MaterialDescriptor>( Name, false );

	if ( MatDesc == null )
		return;

	QMap<MaterialNode*, QMaterialNode*> CoreToQNodeMap;

	QMaterialNodeList CreatedNodes;
	
	for ( MaterialNode* Node = MatDesc->GetFirstNode();
		 Node != null;
		 Node = MatDesc->GetNextNode() )
	{
		QMaterialNode* NewNode;

		// Root节点不用创建
		if ( Node->GetClass() != mRoot->GetMataerialNodeClass() )
		{
			NewNode = CreateNode( Node->GetClass() );
			CreatedNodes.push_back( NewNode );
		}
		else
		{
			NewNode = mRoot;
		}

		CoreToQNodeMap[Node] = NewNode;

		// 恢复场景位置
		Vector2 ScenePos;
		if ( Node->GetObjectPropertyT<Vector2>( L"ScenePos", ScenePos ) )
		{
			NewNode->group()->setPos( ToPointF(ScenePos) );
		}
	}

	GCommandManager->Add( new CmdMaterialNodeCreate( CreatedNodes ) );

	MaterialNode* CoreTerminateNode = MatDesc->GetTerminateNode();
	if ( CoreTerminateNode == null )
		return;

	FindCoreConnections( CoreTerminateNode, CoreToQNodeMap );
}