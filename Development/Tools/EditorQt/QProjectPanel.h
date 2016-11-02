#pragma once

#include "ui_ProjectPanel.h"


class QProjectPanel : public QWidget
{
	Q_OBJECT
public:
	QProjectPanel( QWidget* parent );

	virtual ~QProjectPanel( );

	virtual handle OnIterateFileNode( handle Parent, const wchar* Name, const wchar* FullPathName );

	void RefreshNode( );
	
	bool GetResourceNameFromItem( QTreeWidgetItem* Item, WString& ResourceName, eResourceType& RT );

	void ScanDirectory( const wchar* PathName, handle Parent );

private Q_SLOTS:
	void OnItemClicked( QTreeWidgetItem * item, int column );
	

private:
	Ui::ProjectPanel ui;
	typedef QMap<QTreeWidgetItem*, WString> ItemToStringMap;
	ItemToStringMap mItemToStringMap;

	QTreeWidget* mFileTree;
};


class QFileTree : public QTreeWidget
{
public:
	 explicit QFileTree(QProjectPanel *parent = 0);


protected:
	virtual void mousePressEvent(QMouseEvent *evt);

	QProjectPanel* mParent;
};