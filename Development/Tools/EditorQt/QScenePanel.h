#pragma once

#include "QRenderPanel.h"



class QScenePanel : public QRenderPanel
{
	Q_OBJECT
public:
	QScenePanel( QWidget* Parent = NULL );

	virtual ~QScenePanel();

	void RefreshAdjustModeAction( );

	void RefreshAdjustSpaceAction( );

	QWidget* mHolder;	

	virtual void Initialize( RTTIClass* CameraControllerType );

	ObjectModifier* GetObjectModifier( ){return mObjectModifier;}

	Actor* CmdAddActorViaStaticMesh( const WString& ResourceName, Actor* Parent = null );

	Actor* CmdAddActorViaSkeletonMesh( const WString& ResourceName, Actor* Parent = null );

	void CmdRemoveActor( const ActorArray& Actors );

	void CmdSaveScene( const wchar* FileName );

	void CmdLoadScene( const wchar* FileName, bool MergeToScene );

	void CmdClearScene( );

	void AddMistActor( Actor* A );

	void ClearMistActor( );

protected:
	virtual void mouseMoveEvent( QMouseEvent *evt );

	virtual void mousePressEvent(QMouseEvent *evt);

	virtual void mouseReleaseEvent(QMouseEvent *evt);

	// 在MainFrame添加按键消息
	// virtual void keyReleaseEvent(QKeyEvent* evt );

	void dragEnterEvent(QDragEnterEvent *event);
	
	void dropEvent(QDropEvent *event);	

	Actor* CmdAddActorCommon( Actor* A );

public Q_SLOTS:

	void OnActionFreeView( );

	void OnActionObjectTranslate( );

	void OnActionObjectRotate( );

	void OnActionObjectScale( );

	void OnActionAdjustSpace( );
	

private:
	void OnCreateObjectFromAssert( const wchar* Location );
	

private:

	QAction *actionObjectTranslate;
	QAction *actionObjectRotate;
	QAction *actionObjectScale;
	QAction *actionAdjustSpace;
	QAction *actionFreeView;

	ObjectModifier* mObjectModifier;

	// 已经脱离世界的Actor
	QSet<Actor*> mMistActors;
	
};