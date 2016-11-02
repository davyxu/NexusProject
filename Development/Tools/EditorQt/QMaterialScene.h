#pragma once

class QMaterialPanel;
class QMaterialSlot;
class QMaterialNode;
class QMaterialConnection;

/**
	材质编辑的观察View
*/
class QMaterialGraphicesView : public QGraphicsView
{
	Q_OBJECT
public:
	QMaterialGraphicesView( QMaterialPanel* Panel );

	QMaterialSlot* GetMouseHoverSlot(){ return mMouseHoverSlot;}

	const QBrush& GetElementBGBrush( ){ return mBrushElementBG; }

protected:
	virtual void wheelEvent(QWheelEvent *event);

	virtual void mouseMoveEvent(QMouseEvent *event);


private:
	void scaleView(qreal scaleFactor);
	
	QMaterialPanel* mPanel;

	QMaterialSlot* mMouseHoverSlot;

	QBrush mBrushElementBG;
};


/**
	材质编辑场景
*/
class QMaterialScene : public QGraphicsScene
{
	Q_OBJECT
public:

	QMaterialScene( QMaterialPanel* Parent );

	virtual ~QMaterialScene( );

	/**
		拖放到QGraphicsItem时放下调用此函数
	*/
	virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event );


	virtual void dropEvent( QGraphicsSceneDragDropEvent *event );

	/**
		拖动时,处理拖动事件
		@remark
			这个事件必须处理, 否则, 只能拖动到QGraphicsItem上才有图标变化
	*/
	virtual void dragMoveEvent( QGraphicsSceneDragDropEvent *evt );

	/// 鼠标按下事件
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

	/// 鼠标移动事件
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *evt);

	/// 鼠标释放事件
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *evt);

	/// 右键菜单
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	/// 可以给输出/输入槽进行断开操作
	void DisconnectSlot( QMaterialSlot* Slot );

	/// 将一个节点从场景中独立，断开后需要自己管理内存
	void DetachNode( QMaterialNode* Node );

	/// 连接两个槽
	void ConnectSlot( QMaterialSlot* OutputSlot, QMaterialSlot* InputSlot );

	/// 分配一个连接
	QMaterialConnection* AllocConnection( );

	/// 释放一个槽对应的连接
	void FreeConnection( QMaterialSlot* Slot );

	/// 更新节点对应的
	void UpdateNodeConnections( QMaterialNode* Node );

	/// 获取节点对应的连接对象
	QList<QMaterialConnection*> GetNodeConnections( QMaterialNode* Node );

	QMaterialNode* CreateNode( RTTIClass* NodeType );

	/// 创建根节点,根节点不受到Cmd控制
	void CreateRoot( );

	QMaterialNode* GetRoot( ){ return mRoot;}

	void SaveMaterial( const WString& Name );

	void LoadMaterial( const WString& Name );

private:	
	void FindCoreConnections( MaterialNode* Node, QMap<MaterialNode*, QMaterialNode*>& CoreToQNodeMap );

private Q_SLOTS:

	void OnActionDisconnect( );

	void OnActionDeleteNode( );


private:
	QMaterialPanel* mPanel;

	QList<QMaterialConnection*> mPickedNodeConnections;
	QMaterialNode* mPickedNode;
	QPointF mSavedPos;
	QGraphicsLineItem* mConnectLine;
	QMaterialSlot* mSourceSlot;

	QAction* mActionDisconnect;
	QAction* mActionDeleteNode;

	QMaterialSlot* mSelectedSlot;
	QMaterialNode* mSelectedNode;

	QList<QMaterialConnection*> mConnectionList;
	QList<QMaterialConnection*> mFreeConnectionList;

	QMaterialNode* mRoot;
};
