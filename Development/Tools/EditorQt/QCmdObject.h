#pragma once



class CmdActorAdd : public CmdBase
{
public:
	/// 保存创建的Actor,便于在Undo时删除
	CmdActorAdd( const ActorArray& SavedActors );

	virtual void OnHistoryChanged( );

	virtual void Execute( );

private:
	ActorArray mSavedActors;
};

class CmdActorRemove : public CmdBase
{
public:
	CmdActorRemove( const ActorArray& SavedActors );

	virtual void OnHistoryChanged( );

	virtual void Execute( );

private:
	ActorArray mSavedActors;
	ActorArray mSavedParents;
};


class CmdActorSelect : public CmdBase
{
public:
	CmdActorSelect( EditorObject* Selection );


	virtual void Execute( );

private:
	SelectedActorArray mSelectedArray;

};


class CmdSetObjProperty : public CmdBase
{
public:
	CmdSetObjProperty( const WString& PropertyName, const WString& PropertyValue );

	virtual void Execute( );

private:
	WString mPropertyName;
	WString mPropertyValue;
};

class CmdActorChangeParent : public CmdBase
{
public:
	CmdActorChangeParent( Actor* A , Actor* PrevParent );

	virtual void Execute( );

private:
	Actor* mActor;
	Actor* mPrevParent;
};


class CmdActorClear : public CmdBase
{
public:
	CmdActorClear( const ActorArray& SavedActors, Actor* Root );


	virtual void Execute( );

	virtual void OnHistoryChanged();
	
private:
	Actor* mRoot;
	ActorArray mSavedActors;
};


class CmdActorRestore : public CmdBase
{
public:
	CmdActorRestore( Actor* Root );

	virtual void Execute( );

	virtual void OnHistoryChanged();

private:
	Actor* mRoot;
	ActorArray mSavedActors;
};




/**
	创建Node后需要使用这个Cmd记忆Node并进行之后的操作
*/
class CmdMaterialNodeCreate : public CmdBase
{
public:
	CmdMaterialNodeCreate( const QMaterialNodeList& NodeArray );

	virtual void OnHistoryChanged( );

	virtual void Execute( );

private:
	QMaterialNodeList mNodeList;
};


// 需要保存的连接
struct BackupSlotConnection
{
	QMaterialSlot* mOutputSlot;
	QMaterialSlot* mInputSlot;

	BackupSlotConnection( QMaterialSlot* OutputSlot, QMaterialSlot* InputSlot )
		: mInputSlot( InputSlot )
		, mOutputSlot( OutputSlot )
	{

	}
};


/**
	删除Node前，需要记忆哪些Node需要删除
*/
class CmdMaterialNodeDelete : public CmdBase
{
public:
	CmdMaterialNodeDelete( const QMaterialNodeList& NodeArray );

	virtual void OnHistoryChanged( );

	virtual void Execute( );

private:
	// 节点信息
	QMap<QMaterialNode*, QList<BackupSlotConnection> > mSavedNodeInfo;

	QMaterialNodeList mNodeArray;
};

/**
	节点移动，记录移动前位置
*/
class CmdMaterialNodeMove : public CmdBase
{
public:
	CmdMaterialNodeMove( const QMaterialNodeList& NodeArray );

	virtual void OnHistoryChanged( );

	virtual void Execute( );

private:
	QMaterialNodeList mNodeArray;
	QVector<QPointF> mNodePositions;
};


/**
	连接一个槽，记录断开前的连接
*/
class CmdMaterialSlotConnect : public CmdBase
{
public:
	CmdMaterialSlotConnect( QMaterialSlot* InputSlot );

	virtual void OnHistoryChanged( ){}

	virtual void Execute( );

private:
	QMaterialSlot* mInputSlot;
};


/**
	断开一个槽，记录断开前的连接
*/
class CmdMaterialSlotDisconnect : public CmdBase
{
public:
	CmdMaterialSlotDisconnect( QMaterialSlot* SlotToDisconnect );

	virtual void OnHistoryChanged( ){}

	virtual void Execute( );

private:
	QList<BackupSlotConnection> mConnections;
};