#pragma once

#include "ui_HierarchyPanel.h"

class QHierarchyPanel : public QWidget
					 
{
	Q_OBJECT
public:
	QHierarchyPanel( QWidget* parent );

	virtual ~QHierarchyPanel( );


	virtual handle OnIterateObjectNode( handle Parent, Actor* A ) ;

	void RefreshNode( );

	void MarkSelectNode( EditorObject* Obj );

	Actor* GetActorFromNode( QTreeWidgetItem* Item );

	void CmdChangeParent( Actor* A, Actor* NewParent );

private Q_SLOTS:

	void OnItemSelectionChanged( );

private:
	void FindObject( Actor* A, handle Parent );
	void IterateObject( Actor* RootActor, handle Root );
	
private:
	Ui::HierarchyPanel ui;
	typedef QMap<Actor*, QTreeWidgetItem*> ActorToItemMap;
	typedef QMap<QTreeWidgetItem*, Actor*> ItemToActorMap;
	ActorToItemMap mActorToItemMap;
	ItemToActorMap mItemToActorMap;
	
	bool mBlockMarkSelection;

	bool mBlockItemChange;


	QTreeWidget* mObjectTree;
};





class QObjectTree : public QTreeWidget
{
public:
	explicit QObjectTree(QHierarchyPanel *parent = 0);


protected:
	virtual void mousePressEvent(QMouseEvent *evt);

	virtual void dragEnterEvent( QDragEnterEvent *evt );

	virtual void dropEvent( QDropEvent *evt );

	// 树控件自己拖放时必须加以下3函数
	virtual bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);    
	QStringList mimeTypes() const;
	Qt::DropActions supportedDropActions () const;

	Actor* GetActorAtPoint( const QPoint& pos );
	QHierarchyPanel* mParent;
};