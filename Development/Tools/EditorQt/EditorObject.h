#pragma once

enum eEditorActorFlag
{
	EAF_None			= 0x00000000,
	EAF_Utility			= 0x00000001,
	EAF_EditorObject	= 0x00000002,
};

class EditorObject : public RTTIObject
				   , public ReflectionObject
{
public:

	DECLARE_RTTI_CLASS( EditorObject, RTTIObject, ClassFlag_Intrinsic )

	EditorObject( );

	void Add( Actor* Obj );

	void Remove( Actor* Obj );

	void Clear( );

	void UpdateWrapper( );

	Actor* GetFirstActor( );

	Actor* GetNextActor( );

	dword GetActorCount( );

	bool Empty( ){ return mActorArray.size() == 0 ;}

	bool Exists( Actor* Obj );

	const Vector3&	GetDerivedPosition( );

	const Quaternion& GetDerivedRotation( );

	const Vector3& GetDerivedScale();

	void SetDerivedPosition( const Vector3& Pos );
	
	void SetDerivedScale( const Vector3& Scale );
	
	void SetDerivedRotation( const Quaternion& Rot );

	AABB GetBoundingBox( );

	Matrix4 GetFullTransform( );

	static void StaticInitProperty();

private:
	void OnPropertyRotation( Vector3* Value, bool IsGet );
	void OnPropertyPosition( Vector3* Value, bool IsGet );
	void OnPropertyScale( Vector3* Value, bool IsGet );
	void OnPropertyDebugName( WString* Value, bool IsGet );

	typedef std::set<Actor*> ActorSet;
	static bool ExistInParent( Actor* A, ActorSet& OperatedActors );

	virtual dword GetFirstGroup( );

	virtual dword GetNextGroup( );

	virtual PropertyValue* GetFirstProperty( dword GroupIndex );

	virtual PropertyValue* GetNextProperty( dword GroupIndex );

	virtual RTTIObject* GetGroupMappedInstance( dword GroupIndex );

	virtual const wchar* GetGroupName( dword GroupIndex );

private:
	Actor* mObject;
	MovableObject mNode;

	ActorArray mActorArray;
	ActorArray::iterator mInnerIterator;
	
	ActorSet mTransformSet;

	AABB mWorldAABB;


	struct ReflectionGroup 
	{
		WString mName;
		RTTIObject* mInstance;
		PropertyCollection* mPropertyCollection;
	};

	typedef std::vector<ReflectionGroup> ReflectionGroupArray;
	ReflectionGroupArray mReflectionGroupArray;
	dword mReflectionGroupIterator;
};


typedef TRefPtr<EditorObject> QEditorObjectPtr; 