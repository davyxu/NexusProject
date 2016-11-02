#include "EditorPCH.h"
#include "PrivateInclude.h"

IMPLEMENT_RTTI_CLASS( EditorObject )

EditorObject::EditorObject(  )
: mObject( null )
{

}
void EditorObject::StaticInitProperty()
{
	PropertyCollection* PC = RTTI_CLASS_INFO(EditorObject)->GetPropertyCollection( );

	PC->Add( PROPERTY_DELEGATE( EditorObject,WString,L"DebugName",	&EditorObject::OnPropertyDebugName ) )->SetReadOnly( true );
	PC->Add( PROPERTY_DELEGATE( EditorObject,Vector3,L"Position",	&EditorObject::OnPropertyPosition ) );
	PC->Add( PROPERTY_DELEGATE( EditorObject,Vector3,L"Rotation",	&EditorObject::OnPropertyRotation ) );
	PC->Add( PROPERTY_DELEGATE( EditorObject,Vector3,L"Scale",		&EditorObject::OnPropertyScale ) );
}

NekoEngine::dword EditorObject::GetFirstGroup()
{
	mReflectionGroupIterator = 0;

	return GetNextGroup();
}

NekoEngine::dword EditorObject::GetNextGroup()
{
	if ( mReflectionGroupIterator >= mReflectionGroupArray.size() )
		return ReflectionObject::InvalidGroupID;

	return mReflectionGroupIterator++;
}

PropertyValue* EditorObject::GetFirstProperty( dword GroupIndex )
{
	if ( GroupIndex >= mReflectionGroupArray.size() )
		return null;

	return mReflectionGroupArray[ GroupIndex ].mPropertyCollection->GetFirstProperty();
}

PropertyValue* EditorObject::GetNextProperty( dword GroupIndex )
{
	if ( GroupIndex >= mReflectionGroupArray.size() )
		return null;

	return mReflectionGroupArray[ GroupIndex ].mPropertyCollection->GetNextProperty();
}

RTTIObject* EditorObject::GetGroupMappedInstance( dword GroupIndex )
{
	if ( GroupIndex >= mReflectionGroupArray.size() )
		return null;

	return mReflectionGroupArray[ GroupIndex ].mInstance;
}

const wchar* EditorObject::GetGroupName( dword GroupIndex )
{
	if ( GroupIndex >= mReflectionGroupArray.size() )
		return null;

	return mReflectionGroupArray[ GroupIndex ].mName.c_str();
}


void EditorObject::OnPropertyRotation( Vector3* Value, bool IsGet )
{
	if ( IsGet )
	{
		*Value = Matrix4().FromQuaternion( GetDerivedRotation() ).GetRotator( ).ToEuler();
	}
	else
	{
		SetDerivedRotation( Rotator().FromEuler( *Value ).ToQuaternion( ) );
	}
}


void EditorObject::OnPropertyPosition( Vector3* Value, bool IsGet )
{
	if ( IsGet )
	{
		*Value = GetDerivedPosition();
	}
	else
	{
		SetDerivedPosition( *Value );
	}
}

void EditorObject::OnPropertyScale( Vector3* Value, bool IsGet )
{
	if ( IsGet )
	{
		*Value = GetDerivedScale();
	}
	else
	{
		SetDerivedScale( *Value );
	}
}

void EditorObject::OnPropertyDebugName( WString* Value, bool IsGet )
{
	if ( IsGet )
	{
		if ( GetActorCount() == 1 )
		{
			*Value = GetFirstActor()->GetDebugName();
		}
		
	}
}



const Vector3& EditorObject::GetDerivedPosition()
{
	return mNode.GetDerivedPosition();
}

const Vector3& EditorObject::GetDerivedScale()
{
	return mNode.GetDerivedScale();
}

const Quaternion& EditorObject::GetDerivedRotation()
{
	return mNode.GetDerivedRotation();
}




NekoEngine::AABB EditorObject::GetBoundingBox()
{
	return mWorldAABB;
}

Matrix4 EditorObject::GetFullTransform()
{
	return mNode.GetFullTransform();
}



void EditorObject::Add( Actor* Obj )
{
	Assert( !Exists(Obj) )		

	mActorArray.push_back( Obj );
}

void EditorObject::Clear( )
{
	mActorArray.clear();
}

void EditorObject::Remove( Actor* Obj )
{
	ActorArray::iterator it = std::find( mActorArray.begin(), mActorArray.end(), Obj );

	if ( it != mActorArray.end() )
		mActorArray.erase( it );
}



NekoEngine::dword EditorObject::GetActorCount()
{
	return mActorArray.size();
}


Actor* EditorObject::GetFirstActor( )
{
	mInnerIterator = mActorArray.begin();

	return GetNextActor();
}

Actor* EditorObject::GetNextActor( )
{
	if ( mInnerIterator == mActorArray.end() )
		return null;

	Actor* A = *mInnerIterator;

	++mInnerIterator;

	return A;
}

bool EditorObject::Exists( Actor* Obj )
{
	return std::find( mActorArray.begin(), mActorArray.end(), Obj ) != mActorArray.end();
}

bool SortPickResult( const WorldPick::PickResult& A, const WorldPick::PickResult& B )
{
	return A.mDistance < B.mDistance;
}

// 对结点的子节点数量进行从大到小排序,将最顶层放在前面,先调节
struct SortHierarchy
{
	dword GetTotalChildCount( Actor* A )
	{
		dword ChildCount = A->GetChildCount();
		if ( ChildCount == 0 )
			return 0;

		for ( Actor* C = A->GetChildFirst();
			C != null;
			C = A->GetChildNext() )
		{
			ChildCount += GetTotalChildCount( C ) ;
		}

		return ChildCount;
	}

	bool operator( )( Actor* A, Actor* B )
	{
		dword AChildCount = GetTotalChildCount( A );
		dword BChildCount = GetTotalChildCount( B );
		
		return AChildCount > BChildCount;
	}
};

bool EditorObject::ExistInParent( Actor* A, ActorSet& OperatedActors )
{
	Actor* Parent = A->GetParent();

	if ( Parent == null )
		return false;

	if ( OperatedActors.find( Parent ) != OperatedActors.end() )
		return true;

	return ExistInParent( Parent, OperatedActors );
}

void EditorObject::UpdateWrapper( )
{
	if ( mActorArray.size() > 1 )
		std::stable_sort( mActorArray.begin(), mActorArray.end(), SortHierarchy() );

	Vector3 XTot = Vector3::cZero;
	Vector3 YTot = Vector3::cZero;
	Vector3 ZTot = Vector3::cZero;

	mWorldAABB = AABB::cInitBox;

	mTransformSet.clear();

	for ( Actor* A = GetFirstActor();
		A != null;
		A = GetNextActor() )
	{
		// 计算合并绑定盒
		mWorldAABB += A->GetWorldBoundingBox();

		XTot += A->GetDerivedRotation() * Vector3::cXAxis;
		YTot += A->GetDerivedRotation() * Vector3::cYAxis;
		ZTot += A->GetDerivedRotation() * Vector3::cZAxis;

		// 父节点作用过后,子节点不会被作用
		if ( ExistInParent( A, mTransformSet) )
			continue;

		// 记录需要变换的对象集合
		mTransformSet.insert( A );
	}


	float SelectedCount = (float)GetActorCount();

	// 设置合并中心
	if ( SelectedCount > 0 )
	{
		XTot /= SelectedCount;
		YTot /= SelectedCount;
		ZTot /= SelectedCount;

		mNode.SetPosition( mWorldAABB.GetCenter() );

		// FIXED BUG: 选Local模式,旋转茶壶,旋转角与Gizmo显示的角正好相反,倒退版本到2个月前都仍然有此BUG [10/20/2011 Davyxu]
		mNode.SetRotation( -Quaternion().FromAxes( XTot, YTot, ZTot ) );
	}
	else
	{
		mNode.SetPosition( Vector3::cZero );
		mNode.SetRotation( Quaternion::cIdentity );
	}

	mNode.SetScale( Vector3::cOne );


	mReflectionGroupArray.clear();

	ReflectionGroup GroupTransform;
	GroupTransform.mName = L"Transform";
	GroupTransform.mInstance = this;
	GroupTransform.mPropertyCollection = GetClass()->GetPropertyCollection();

	mReflectionGroupArray.push_back( GroupTransform );

	if ( mActorArray.size() == 1 )
	{
		Actor* A = mActorArray[0];

		for ( ActorComponent* Component = A->GetFirstComponent();
			Component != null;
			Component = A->GetNextComponent() )
		{
			ReflectionGroup Group;
			Group.mName = Component->GetClass()->GetRTTIClassString();
			Group.mInstance = Component;
			Group.mPropertyCollection = Component->GetClass()->GetPropertyCollection();

			mReflectionGroupArray.push_back( Group );
		}
		
	}
}





void EditorObject::SetDerivedPosition( const Vector3& Value )
{
	Vector3 Diff = Value - mNode.GetDerivedPosition();
	mNode.SetPosition( Value );

	for ( ActorSet::iterator it = mTransformSet.begin();
		it != mTransformSet.end();
		++it )
	{
		Actor* A = *it;

		A->SetDerivedPosition( A->GetDerivedPosition() + Diff );
	}
}

void EditorObject::SetDerivedRotation( const Quaternion& Value )
{
	Quaternion Diff = Value  * mNode.GetRotation().Inverse();

	mNode.SetRotation( Value );

	Vector3 GroupPos = mNode.GetPosition();

	for ( ActorSet::iterator it = mTransformSet.begin();
		it != mTransformSet.end();
		++it )
	{
		Actor* A = *it;

		Vector3 NewPos =  Diff * (A->GetDerivedPosition() - GroupPos )  + GroupPos;
		A->SetDerivedPosition( NewPos );

		A->SetDerivedRotation( Diff * A->GetDerivedRotation()   );
	}
}

void EditorObject::SetDerivedScale( const Vector3& Value )
{

	Vector3 Scale;
	Vector3 Diff = Value / mNode.GetScale();

	mNode.SetScale( Value );

	Vector3 GroupPos = mNode.GetPosition();

	for ( ActorSet::iterator it = mTransformSet.begin();
		it != mTransformSet.end();
		++it )
	{
		Actor* A = *it;

		Vector3 NewPos = A->GetDerivedPosition() - GroupPos;

		Vector3 AxisX = mNode.GetDerivedRotation() * Vector3::cXAxis;
		Vector3 AxisY = mNode.GetDerivedRotation() * Vector3::cYAxis;
		Vector3 AxisZ = mNode.GetDerivedRotation() * Vector3::cZAxis;

		Vector3 Pos1 = AxisX * Vector3::DotProduct( AxisX, NewPos );
		Vector3 Pos2 = AxisY * Vector3::DotProduct( AxisY, NewPos );
		Vector3 Pos3 = AxisZ * Vector3::DotProduct( AxisZ, NewPos );

		NewPos = Pos1 * Diff.x + Pos2 * Diff.y + Pos3 * Diff.z + GroupPos;

		A->SetDerivedPosition( NewPos );


		Scale = A->GetDerivedRotation().Inverse() * A->GetDerivedScale();
		Scale = mNode.GetRotation() * Scale;

		Scale *= Diff;

		Scale = mNode.GetRotation().Inverse() * Scale;
		Scale = A->GetDerivedRotation() * Scale;

		A->SetDerivedScale( Scale );
	}
}

