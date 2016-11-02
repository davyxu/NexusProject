#pragma once

#include "ui_MaterialPanel.h"


class QPropertyGrid;
class QMaterialGraphicesView;
class QMaterialScene;
class QMaterialNode;
class QMaterialSlot;


/**
	材质编辑主界面
*/
class QMaterialPanel : public QWidget	
{
	Q_OBJECT
public:
	QMaterialPanel( QWidget* parent );

	virtual ~QMaterialPanel( );

	void OpenMaterial( const WString& ResourceName );

	MaterialDescriptorPtr CreateMaterial( const WString ResourceName );

	void Initialize( );

	void Finalize( );

	QMaterialGraphicesView* GetGraphicsView( ){ return mGraphicsView;}

	QMaterialScene* GetGraphicsScene( ){ return mGraphicsScene; }

	QMaterialNode* CmdSpawnNode( RTTIClass* NodeClass );

	void CmdDeleteNode( QMaterialNode* Node );

	void CmdConnectSlot( QMaterialSlot* OutputSlot, QMaterialSlot* InputSlot );

	void CmdDisconnectSlot( QMaterialSlot* Slot );
	
	void CmdReset( );

	void CmdLoadMaterial( const QString& Name );

	void AddMistNode( QMaterialNode* Node );

	void ClearMistNodes( );

private Q_SLOTS:
	void OnActionBuildMaterial( );
	void OnActionUpdateToSelected( );
	void OnActionReset( );
	void OnActionLoadMaterial( );

private:
	Ui::MaterialPanel ui;

	typedef QMap<QTreeWidgetItem*, WString> ItemToStringMap;
	ItemToStringMap mItemToStringMap;

	QPropertyGrid* mMemberProperty;

	QMaterialScene* mGraphicsScene;

	QMaterialGraphicesView* mGraphicsView;

	QAction* mActionLoad;
	QAction* mActionReset;
	QAction* mActionBuild;
	QAction* mActionUpdateToSelected;
	
	// 已经脱离世界的Node
	QSet<QMaterialNode*> mMistNodes;
};


/**
	材质元素创建用树形控件
*/
class QMaterialElementTree : public QTreeWidget
{
public:
	explicit QMaterialElementTree(QMaterialPanel *parent = 0);

	
	void AddElement( QTreeWidgetItem* Parent, RTTIClass* ClassInfo, const QString& Name, const QString& Desc );

	QTreeWidgetItem* AddType( const QString& Name );

protected:
	virtual void mousePressEvent(QMouseEvent *evt);

	RTTIClass* GetClassFromItem( QTreeWidgetItem* Item );

private:
	typedef QMap<QTreeWidgetItem*, RTTIClass*> ItemToClassMap;
	ItemToClassMap mItemToClassMap;

	QMaterialPanel* mParent;
};