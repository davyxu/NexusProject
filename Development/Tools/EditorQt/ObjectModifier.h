#pragma once






class ObjectModifier
{
public:
	ObjectModifier( );

	virtual ~ObjectModifier( );

	void Initialize( GameWorld* World );

	EditorObject* GetSelectedObject( );

	// 旋转,缩放,位移
	void SetGizmoAdjustMode( eECAdjustMode Mode );	

	eECAdjustMode GetGizmoAdjustMode( );

	// 本地或世界空间调整
	void SetGizmoAdjustSpace( eECAdjustSpace Space );

	eECAdjustSpace GetGizmoAdjustSpace( );

	void CmdSetSelection( const SelectedActorArray& SelectedActor, bool KeepOld = false , bool Remove = false );

	Vector3 GetGroupSpawnPoint( const Point& MP );

public:	

	virtual void OnMouseMove( const Point& MP );

	virtual void OnMouseLeftButtonDown( const Point& MP );

	virtual void OnMouseLeftButtonUp( const Point& MP );

	void UpdateGizmo( );

public:

	Actor* GetGizmo( ){ return mGizmo;}

private:
	void CreateGizmo( );
	eAdjustAxis PickGizmoAxis( const Ray& PickRay );
	Plane FindGizmoTranslationPlane( const Ray& PickRay, dword TranslationAxis );
	Vector3 GetGizmoIntersect( const Ray& PickRay, const Plane& PlaneToUse, dword TranslationAxis, Vector3& LastPosition );
	
	void BeginAdjustGizmo( const Point& MP );
	void AdjustGizmo( const Point& MP );
	void EndAdjustGizmo( );

	Actor* PickSingleObject( const Ray& PickRay );
	bool VolumeSelect( const Point& Pt1, const Point& Pt2);
	void SingleSelect( const Point& MP );	


private:
	friend class EditorDrawUtilityComponent;

	EditorObject*		mSelectedObject;		// 选中的物体集合, 不会为空
	Actor*				mGizmo;					// Gizmo对象
	Actor*				mEditorHelper;			// 绘制选中物体及其他的工作
	eAdjustAxis			mEditorAxis;			// 选中的轴

	// 拾取Gizmo前保存的物体属性
	Quaternion			mLastDerivedRotation;
	Vector3				mLastDerivedPosition;
	Vector3				mLastScale;
	Plane				mLastUsedPlane;
	Vector3				mLast3DDelta;
	Vector3				mLastTranslationDelta;
	Vector2				mSavedMousePt;			// Gizmo拾取保存的鼠标位置
	bool				mIsAdjustingGizmo;		// 正在调整Gizmo
	eECAdjustMode		mGizmoAdjustMode;		// Gizmo调节模式
	eECAdjustSpace		mGizmoAdjustSpace;		// Gizmo调整空间方式

	bool				mIsVolumeSelecting;		// 是否进行区域选择
	Point				mVolumeSelectPt1;		// 选中第一点
	Point				mVolumeSelectPt2;		// 选中第二点

	GameWorld*			mWorld;

};
