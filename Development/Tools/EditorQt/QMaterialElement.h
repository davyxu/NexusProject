#pragma once

class QMaterialGraphicesView;
class QMaterialSlot;
class QMaterialConnection;
class QMaterialNode;
class QMaterialPanel;


/*
	层次结构
	QMaterialNodeGroup	<-放在QMaterialScene中
		QMaterialNode	由Group管理,通过group获得Group对象
		QMaterialSlot	被Node对象管理

	QMaterialConnection <-放在QMaterialScene中

*/


/**
	材质节点封装组
*/
class QMaterialNodeGroup : public QGraphicsItemGroup
{
public:
	QMaterialNodeGroup( QMaterialNode* Node )
		: mNode( Node )
	{

	}

	QMaterialNode* GetNode( ){ return mNode; }

private:
	QMaterialNode* mNode;
};

/**
	材质节点中最大的矩形块
*/
class QMaterialNode : public QGraphicsItem
{

public:
	enum ElementSize
	{
		NodeWidth = 100,
		NameHeight = 15,
		NameWidthMargin = 10,

		SlotGap = 5,		
	};

	QMaterialNode( RTTIClass* NodeClass, QMaterialGraphicesView* View  );

	virtual ~QMaterialNode();

	void InitializeMembers( QGraphicsScene* Scene );

	/// 将Node的Group删除，然后删除Group时会自动删除Node
	void ReleaseMembers( QGraphicsScene* Scene );

	const QRectF& GetLogicBound( ){ return mLogicBound; }

	/// 获取输出槽
	QMaterialSlot* GetOutputSlot( ){ return mOutputSlot; }

	/// 获取输入槽集合
	QList<QMaterialSlot*>& GetInputSlots( ){ return mInputSlots; }

	RTTIClass* GetMataerialNodeClass( ){ return mCoreNode->GetClass(); }

	/// 通过名称获得槽
	QMaterialSlot* GetSlotByName( const QString& Name );

	dword mIDForSave;

protected:
	virtual QRectF boundingRect() const;

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	



private:
	QString mName;

	QMaterialGraphicesView* mView;

	QRectF mLogicBound;

	QRectF mSafeBound;

	QMaterialSlot* mOutputSlot;
	
	MaterialNode* mCoreNode;			

	QGraphicsItemGroup* mGroupItem;

	QList<QMaterialSlot*> mInputSlots;

};


typedef QList<QMaterialNode*> QMaterialNodeList;

/**
	材质插槽
*/
class QMaterialSlot : public QGraphicsItem
{

public:
	enum ElementSize
	{
		SlotWidth = 15,

		SlotHeight = 15,
	};

	QMaterialSlot( QMaterialGraphicesView* View,  QMaterialNode* Parent, const QString& Name );

	/// 是否为输出节点
	bool IsOutputSlot( ){ return mParent->GetOutputSlot() == this; }

	/// 添加槽对槽连接
	void AddSlotConnection( QMaterialSlot* Slot );

	/// 删除槽对槽连接
	void RemoveSlotConnection( QMaterialSlot* Slot );

	/// 获取父级节点
	QMaterialNode* GetMaterialNode( ){ return mParent; }

	/// 获得槽的连接槽集合
	QList<QMaterialSlot*>& GetSlotConnections( ) { return mSlotConnection;}

	QMaterialSlot* GetOneSlotConnection( );

	/// 获取名称
	QString GetName( ){ return mName; }

public:
	virtual QRectF boundingRect() const;

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);	
	

private:
	QString mName;

	QMaterialGraphicesView* mView;

	QMaterialNode* mParent;

	QRectF mBound;

	QList<QMaterialSlot*> mSlotConnection;	
};

/**
	材质连接
*/
class QMaterialConnection : public QGraphicsLineItem
{
public:
	enum ElementSize
	{
		SlotWidth = 15,

		SlotHeight = 15,
	};

	QMaterialConnection(  QMaterialGraphicesView* View );

	/// 重新设置连接的两端槽
	void Reset( QMaterialSlot* OutputSlot, QMaterialSlot* InputSlot );

	/// 更新线条
	void UpdateGraphics( );

	/// 获取输出槽
	QMaterialSlot* GetOutputSlot( ){return mOutputSlot; }

	/// 获取输入槽
	QMaterialSlot* GetInputSlot( ){return mInputSlot; }

	/// 这个连接是否和Node相关
	bool IsRelateTo( QMaterialNode* Node );

private:
	QMaterialGraphicesView* mView;

	QMaterialSlot* mOutputSlot;
	QMaterialSlot* mInputSlot;

	QPointF mOutputPoint;
	QPointF mInputPoint;

	QRectF mBound;	

	QList<QMaterialNode*> mNodes;
};

