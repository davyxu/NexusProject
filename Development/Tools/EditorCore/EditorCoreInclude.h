#pragma once

#include <set>

#include "CommandManager.h"
#include "ReflectionObject.h"
#include "EditorObject.h"

enum eECAdjustMode
{
	AdjustMode_None,
	AdjustMode_Position,
	AdjustMode_Scale,
	AdjustMode_Rotation,
};

enum eECAdjustSpace
{
	AdjustSpace_Local,
	AdjustSpace_World,
};


enum eECRenderWindowType
{
	RWT_Scene = 0,
	RWT_Preview,
	RWT_Material,
	RWT_Max,
};



typedef std::vector<Actor*> SelectedActorArray;

class ECEventListener
{
public:
	virtual void OnObjectSelectionChange( EditorObject* EditorObject ) = 0;

	virtual void OnObjectChangeProperties( EditorObject* EditorObject ) = 0;

	virtual void OnObjectAdd( Actor* A ) = 0;

	virtual void OnObjectRemove( Actor* A ) = 0;

	virtual void OnObjectClear( ) = 0;

	virtual void OnObjectChangeParent( Actor* A, Actor* NewParent ) = 0;

	virtual void OnGizmoAdjustModeChange( eECAdjustMode Mode ) = 0;

	virtual void OnGizmoAdjustSpaceChange( eECAdjustSpace Space ) = 0;
};


class ECFileSystemIterateListener
{
public:
	virtual handle OnIterateFileNode( handle Parent, const wchar* Name, const wchar* FullPathName ) = 0;
};

class ECObjectIterateListener
{
public:
	virtual handle OnIterateObjectNode( handle Parent, Actor* A ) = 0;
};

struct ECWindowMember 
{
	PresentWindow*	mWindow;
	CameraActor*	mCameraActor;
	GameWorld*		mWorld;

	ECWindowMember( )
		: mWindow( null )
		, mCameraActor( null )
		, mWorld( null )
	{

	}
};

class IEditorCore
{
public:
	virtual void Release( ) = 0;

	virtual void Initialize( handle SceneWindow, handle PreviewWindow, handle MaterialWindow ) = 0;

public:
	virtual void OnEditorObjectChangeProperties( RTTIObject* Object, PropertyValue* PV ) = 0;

	virtual void OnMouseMove( const Point& MP ) = 0;

	virtual void OnMouseLeftButtonDown( const Point& MP ) = 0;

	virtual void OnMouseLeftButtonUp( const Point& MP ) = 0;

public:

	virtual void SetEditorEventListener( ECEventListener* Listener ) = 0;

	virtual ECEventListener* GetEditorEventListener( ) = 0;

	virtual Actor* CmdAddActorViaStaticMesh( const WString& ResourceName, Actor* Parent = null ) = 0;

	virtual Actor* CmdAddActorViaSkeletonMesh( const WString& ResourceName, Actor* Parent = null ) = 0;

	virtual void CmdRemoveActor( const ActorArray& Actors ) = 0;

	virtual EditorObject* GetSelectedObject( ) = 0;

	virtual void SetGizmoAdjustMode( eECAdjustMode Mode ) = 0;

	virtual eECAdjustMode GetGizmoAdjustMode( ) = 0;

	virtual void SetGizmoAdjustSpace( eECAdjustSpace Space ) = 0;

	virtual eECAdjustSpace GetGizmoAdjustSpace( ) = 0;

	virtual Logger* GetLogger( ) = 0;

	virtual void IterateFileSystem( const wchar* PathName, ECFileSystemIterateListener* Listener, handle Root ) = 0;

	virtual void IterateObject( Actor* RootActor, ECObjectIterateListener* Listener, handle Root ) = 0;

	virtual void CmdSetSelection( const SelectedActorArray& SelectedActor, bool KeepOld = false, bool Remove = false ) = 0;

	virtual ECWindowMember* GetWindowMember( eECRenderWindowType WindowType ) = 0;

	virtual Vector3 GetGroupSpawnPoint( const Point& MP ) = 0;

	virtual void CmdSaveScene( const wchar* FileName ) = 0;

	virtual void CmdLoadScene( const wchar* FileName, bool MergeToScene ) = 0;

	virtual void CmdClearScene( ) = 0;

	virtual void CmdChangeParent( Actor* A, Actor* NewParent ) = 0;

	virtual CommandManager* GetCommandManager( ) = 0;
};

extern IEditorCore* CreateEditorCore( );


