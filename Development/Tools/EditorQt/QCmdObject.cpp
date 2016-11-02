#include "EditorPCH.h"
#include "PrivateInclude.h"

#include "EditorPCH.h"
#include "PrivateInclude.h"


//////////////////////////////////////////////////////////////////////////
// QCmdActorAdd
//////////////////////////////////////////////////////////////////////////
CmdActorAdd::CmdActorAdd( const ActorArray& SavedActors )
: mSavedActors( SavedActors )
{

}


void CmdActorAdd::Execute()
{
	// Delete Actor here
	GEditorFrame->GetScenePanel()->CmdRemoveActor( mSavedActors );
}

void CmdActorAdd::OnHistoryChanged()
{
	for ( dword i = 0;i < mSavedActors.size();i++)
	{
		GEditorFrame->GetScenePanel()->AddMistActor( mSavedActors[i] );
	}
}




//////////////////////////////////////////////////////////////////////////
// QCmdActorRemove
//////////////////////////////////////////////////////////////////////////
CmdActorRemove::CmdActorRemove( const ActorArray& SavedActors )
: mSavedActors( SavedActors )
{
	mSavedParents.resize( SavedActors.size() );

	for ( dword i = 0;i< SavedActors.size();i++ )
	{
		mSavedParents[i] = SavedActors[i]->GetParent();
	}
}


void CmdActorRemove::Execute()
{
	// 重新将Actor连接到父级
	for ( dword i = 0;i< mSavedParents.size();i++ )
	{
		GameWorld::RestoreFromSystem( mSavedActors[i] );

		mSavedParents[i]->AddChild( mSavedActors[i] );
	}


	GEditorFrame->GetHierarchyPanel()->RefreshNode();

	GCommandManager->Add( new CmdActorAdd( mSavedActors ) );
}

void CmdActorRemove::OnHistoryChanged()
{
	for ( dword i = 0;i < mSavedActors.size();i++)
	{
		GEditorFrame->GetScenePanel()->AddMistActor( mSavedActors[i] );
	}
}


//////////////////////////////////////////////////////////////////////////
// QCmdSelectActor
//////////////////////////////////////////////////////////////////////////
CmdActorSelect::CmdActorSelect( EditorObject* Selection )
{
	if ( Selection != null )
	{
		dword Index = 0;
		mSelectedArray.resize( Selection->GetActorCount() );
		for ( Actor* A = Selection->GetFirstActor();
			A != null ;
			A = Selection->GetNextActor() )
		{
			mSelectedArray[Index++] = A;
		}
	}

}



void CmdActorSelect::Execute()
{
	GEditorFrame->GetScenePanel()->GetObjectModifier()->CmdSetSelection( mSelectedArray, false, false );
}


//////////////////////////////////////////////////////////////////////////
// QCmdPropertySet
//////////////////////////////////////////////////////////////////////////
CmdSetObjProperty::CmdSetObjProperty( const WString& PropertyName, const WString& PropertyValue )
: mPropertyName( PropertyName )
, mPropertyValue( PropertyValue )
{

}

// UI,用户,Cmd避免递归调用策略:
// 1. 用户操纵物体的修改:  UI <-Update--Obj(modified)-SaveOldValue--> CmdSystem
// 2. Undo/Redo操作的修改:  UI <-Update--Obj(modified)<--Set--> CmdSystem
// 3. 用户操纵UI的修改:  UI -OnChange-->Obj(modified)-SaveOldValue--> CmdSystem


void CmdSetObjProperty::Execute()
{
	EditorObject* Obj = GEditorFrame->GetScenePanel()->GetObjectModifier()->GetSelectedObject();
	Assert( Obj != null );
	if ( Obj == null )
		return;

	// 将改之前的保存
	WString OldValue;
	if ( Obj->GetObjectPropertyAsString( mPropertyName.c_str(), OldValue ) )
	{
		GCommandManager->Add( new CmdSetObjProperty( mPropertyName, OldValue ) );
	}


	// 设置属性
	Obj->SetObjectPropertyByString( mPropertyName.c_str(), mPropertyValue.c_str() );


	GEditorFrame->OnObjectChangeProperties( Obj );

}




//////////////////////////////////////////////////////////////////////////
// CmdActorChangeParent
//////////////////////////////////////////////////////////////////////////
CmdActorChangeParent::CmdActorChangeParent( Actor* A , Actor* PrevParent  )
: mActor( A )
, mPrevParent( PrevParent )
{

}

void CmdActorChangeParent::Execute()
{
	// Delete Actor here
	GEditorFrame->GetHierarchyPanel()->CmdChangeParent( mActor, mPrevParent );
}


//////////////////////////////////////////////////////////////////////////
// CmdActorClear
//////////////////////////////////////////////////////////////////////////
CmdActorClear::CmdActorClear( const ActorArray& SavedActors, Actor* Root )
: mRoot( Root )
, mSavedActors( SavedActors )
{

}


void CmdActorClear::OnHistoryChanged()
{
	for ( dword i = 0;i< mSavedActors.size();i++ )
	{
		GEditorFrame->GetScenePanel()->AddMistActor( mSavedActors[i] );
	}

}

void CmdActorClear::Execute()
{
	for ( dword i = 0;i< mSavedActors.size();i++ )
	{
		GameWorld::RestoreFromSystem( mSavedActors[i] );

		mRoot->AddChild( mSavedActors[i] );
	}

	GEditorFrame->GetHierarchyPanel()->RefreshNode();

	GCommandManager->Add( new CmdActorRestore( mRoot ) );
}


//////////////////////////////////////////////////////////////////////////
// CmdActorRestore
//////////////////////////////////////////////////////////////////////////
CmdActorRestore::CmdActorRestore( Actor* Root )
: mRoot( Root )
{

}

void CmdActorRestore::OnHistoryChanged()
{


}

void CmdActorRestore::Execute()
{
	GEditorFrame->GetScenePanel()->CmdClearScene( );
}



//////////////////////////////////////////////////////////////////////////
// CmdMaterialNodeCreate
//////////////////////////////////////////////////////////////////////////
CmdMaterialNodeCreate::CmdMaterialNodeCreate( const QMaterialNodeList& NodeArray )
: mNodeList( NodeArray )
{
	
}

void CmdMaterialNodeCreate::OnHistoryChanged()
{
	foreach( QMaterialNode* Node, mNodeList )
	{
		GEditorFrame->GetMaterialPanel()->AddMistNode( Node );
	}

	mNodeList.clear();
}

void CmdMaterialNodeCreate::Execute()
{
	QMaterialScene* Scene = GEditorFrame->GetMaterialPanel()->GetGraphicsScene();
	
	GCommandManager->Add( new CmdMaterialNodeDelete( mNodeList ) );

	foreach( QMaterialNode* Node, mNodeList )
	{
		Scene->DetachNode( Node );
	}
}

//////////////////////////////////////////////////////////////////////////
// CmdMaterialNodeDelete
//////////////////////////////////////////////////////////////////////////

CmdMaterialNodeDelete::CmdMaterialNodeDelete( const QMaterialNodeList& NodeArray )
: mNodeArray( NodeArray )
{
	QMaterialScene* Scene = GEditorFrame->GetMaterialPanel()->GetGraphicsScene();

	// 记住每个节点的每个连接，便于以后恢复
	foreach( QMaterialNode* Node, mNodeArray )
	{
		QList<BackupSlotConnection> SlotConnections;		

		foreach( QMaterialConnection* Connection, Scene->GetNodeConnections( Node ) )
		{
			SlotConnections.push_back( BackupSlotConnection( Connection->GetOutputSlot(), Connection->GetInputSlot() ) );
		}

		mSavedNodeInfo.insert( Node, SlotConnections );
	}
}

void CmdMaterialNodeDelete::OnHistoryChanged()
{

	QMapIterator<QMaterialNode*, QList<BackupSlotConnection>> Iterator( mSavedNodeInfo );

	// 删除节点
	while ( Iterator.hasNext() )
	{
		Iterator.next();

		QMaterialNode* Node = Iterator.key();

		GEditorFrame->GetMaterialPanel()->AddMistNode( Node );
	}

	mSavedNodeInfo.clear();
}

void CmdMaterialNodeDelete::Execute()
{
	
	GCommandManager->Add( new CmdMaterialNodeCreate( mNodeArray ) );

	QMaterialScene* Scene = GEditorFrame->GetMaterialPanel()->GetGraphicsScene();

	{
		QMapIterator<QMaterialNode*, QList<BackupSlotConnection>> Iterator( mSavedNodeInfo );

		// 恢复节点
		while ( Iterator.hasNext() )
		{
			Iterator.next();

			QMaterialNode* Node = Iterator.key();

			Scene->addItem( Node->group() );
		}
	}

	{
		QMapIterator<QMaterialNode*, QList<BackupSlotConnection>> Iterator( mSavedNodeInfo );

		// 恢复连接
		while ( Iterator.hasNext() )
		{
			Iterator.next();

			const QList<BackupSlotConnection>& SlotConnections = Iterator.value();

			foreach( BackupSlotConnection Connections, SlotConnections )
			{
				Scene->ConnectSlot( Connections.mOutputSlot, Connections.mInputSlot );
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// CmdMaterialNodeMove
//////////////////////////////////////////////////////////////////////////


CmdMaterialNodeMove::CmdMaterialNodeMove( const QMaterialNodeList& NodeArray )
: mNodeArray( NodeArray )
{
	mNodePositions.resize( NodeArray.size() );

	dword Index = 0;
	foreach( QMaterialNode* Node, NodeArray )
	{
		mNodePositions[ Index++ ] = Node->group()->scenePos();
	}
}

void CmdMaterialNodeMove::OnHistoryChanged()
{

}

void CmdMaterialNodeMove::Execute()
{
	QMaterialScene* Scene = GEditorFrame->GetMaterialPanel()->GetGraphicsScene();

	GCommandManager->Add( new CmdMaterialNodeMove( mNodeArray ) );


	dword Index = 0;
	foreach( QMaterialNode* Node, mNodeArray )
	{
		Node->group()->setPos( mNodePositions[ Index++ ] );		 

		//@TODO 避免重复更新连接
		Scene->UpdateNodeConnections( Node );
	}
}

//////////////////////////////////////////////////////////////////////////
// CmdMaterialSlotConnect
//////////////////////////////////////////////////////////////////////////

CmdMaterialSlotConnect::CmdMaterialSlotConnect( QMaterialSlot* InputSlot )
: mInputSlot( InputSlot )
{

}

void CmdMaterialSlotConnect::Execute()
{
	// 断开这个输入槽
	GEditorFrame->GetMaterialPanel()->CmdDisconnectSlot( mInputSlot );
}




//////////////////////////////////////////////////////////////////////////
// CmdMaterialSlotDisconnect
//////////////////////////////////////////////////////////////////////////

CmdMaterialSlotDisconnect::CmdMaterialSlotDisconnect( QMaterialSlot* SlotToDisconnect )
{
	// 记录这个节点的连接信息
	foreach( QMaterialSlot* ConnectedSlot, SlotToDisconnect->GetSlotConnections() )
	{
		if ( SlotToDisconnect->IsOutputSlot() )
			mConnections.push_back( BackupSlotConnection( SlotToDisconnect, ConnectedSlot ) );
		else
			mConnections.push_back( BackupSlotConnection( ConnectedSlot, SlotToDisconnect ) );
	}
}

void CmdMaterialSlotDisconnect::Execute()
{
	QMaterialScene* Scene = GEditorFrame->GetMaterialPanel()->GetGraphicsScene();

	// 做一批次的重新连接操作
	GCommandManager->BeginCollection( L"Slot Connect");

	foreach( BackupSlotConnection Connection, mConnections )
	{
		GCommandManager->Add( new CmdMaterialSlotConnect( Connection.mInputSlot ) );

		Scene->ConnectSlot( Connection.mOutputSlot, Connection.mInputSlot );
	}

	GCommandManager->EndCollection( true );
}
