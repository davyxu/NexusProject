#pragma once


#include "ui_EditorFrame.h"


class QScenePanel;
class QPropertyGrid;
class QHierarchyPanel;
class QProjectPanel;
class QMaterialPanel;
class QPreviewPanel;

class QEditorFrame : public QMainWindow
{
	Q_OBJECT

public:
	QEditorFrame(QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~QEditorFrame();

	void Initialize( );

	void Finalize( );

	QScenePanel* GetScenePanel( ){ return mScenePanel;}

	QPropertyGrid* GetPropertyPanel( ){ return mPropertyPanel; }

	QHierarchyPanel* GetHierarchyPanel( ){ return mHierarchyPanel; }

	QProjectPanel* GetProjectPanel( ){ return mProjectPanel; }

	QMaterialPanel* GetMaterialPanel( ){ return mMaterialPanel; }	

	void OnObjectSelectionChange( EditorObject* EditorObject );

	void OnObjectChangeProperties( EditorObject* EditorObject );

public Q_SLOTS:
	void OnSaveLayout( );

	void OnLoadLayout( );

	void OnTick( );

	void OnNewScene( );

	void OnLoadScene( );

	void OnSaveScene( );

private:

	void WriteSettings( );
	void ReadSettings();


	QDockWidget* AddDock( QWidget* w, Qt::DockWidgetArea Area );

	virtual void keyPressEvent(QKeyEvent * evt );

	virtual void keyReleaseEvent(QKeyEvent * evt );	

	static void StaticInitProperty( );

	static void OnPropertyAutoSleep( RTTIObject* Instance, bool* Value, bool IsGet );

	void NotifyKeyEvent( QKeyEvent * evt, eMessageKeyState MKS );

private:
	Ui::EditorFrame ui;
	QTimer*				mTimer;
	QScenePanel*		mScenePanel;
	QPropertyGrid*		mPropertyPanel;
	QHierarchyPanel*	mHierarchyPanel;
	QProjectPanel*		mProjectPanel;
	QMaterialPanel*		mMaterialPanel;
	QPreviewPanel*		mPreviewPanel;

	
};

extern QEditorFrame* GEditorFrame;