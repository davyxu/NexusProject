#include "EnginePCH.h"
#include "NekoEngine_System.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS( Actor );

	Actor::Actor()
		: mParent( null )		
		, mChildAABB( AABB::cNullBox )
		, mSelfAABB( AABB::cNullBox )
		, mVisible( true )
		, mPickable( true )
		, mUserData( 0 )		
		, mFlag( 0 )
		, mWorld( 0 )
		, mFactoryClass( null )
	{
		mFirstGeometryComponent = mComponentList.end();
	}

	Actor::~Actor()
	{
		ClearComponent();
	}

	Actor* Actor::GetParent()
	{
		return mParent;
	}


	void Actor::Tick( float Elapse )
	{
		// 更新组件
		for ( ComponentList::iterator it = mComponentList.begin();
			it != mComponentList.end();
			++it )
		{
			ActorComponent* Component = *it;
			Component->Tick( Elapse );
		}
	}


	ActorComponent* Actor::AddComponent( RTTIClass* Class, dword Index )
	{	
		Assert( !GetComponent( Class, Index ) && "ActorComponent already exist");

		ActorComponent* Component = RTTI_CAST(ActorComponent, Class->CreateInstance() );
		Assert( Component != null );

		Component->mIndex = Index;

		Component->mOwner = this;

		Component->OnAttach();

		// 可绘制部件放置在后部
		if ( Component->IsDrawable() )
		{
			mComponentList.push_back( Component );

			if ( mFirstGeometryComponent == mComponentList.end() )
			{
				mFirstGeometryComponent = --mComponentList.end( );
			}
		}
		// 普通的放在前端
		else
		{
			mComponentList.push_front( Component );
		}

		return Component;
	}


	void Actor::ClearComponent()
	{
		for ( ComponentList::iterator it = mComponentList.begin();
			it != mComponentList.end();
			++it )
		{
			ActorComponent* Component = *it;
			Component->OnDetach();

			delete Component;
		}

		mComponentList.clear();
	}


	ActorComponent* Actor::GetFirstComponent(  )
	{
		mComponentIterator = mComponentList.begin();

		return GetNextComponent();
	}

	ActorComponent* Actor::GetNextComponent()
	{
		if ( mComponentIterator == mComponentList.end() )
			return null;

		ActorComponent* Component = *mComponentIterator;
		mComponentIterator++;

		return Component;
	}

	ActorComponent* Actor::GetComponent( RTTIClass* Class, dword Index )
	{

		for ( ComponentList::iterator it = mComponentList.begin();
			it != mComponentList.end();
			++it )
		{
			ActorComponent* Component = *it;
			if ( Component->IsA( Class) && Component->mIndex == Index )
				return Component;
		}

		return null;
	}


	void Actor::RemoveComponent( RTTIClass* Class, dword Index )
	{
		for ( ComponentList::iterator it = mComponentList.begin();
			it != mComponentList.end();
			++it )
		{
			ActorComponent* Component = *it;
			if ( Component->IsA( Class) && Component->mIndex == Index )
			{
				mComponentList.erase( it );
				break;
			}
		}
	}





	void Actor::AddChild( Actor* A )
	{
		Assert( A != null );
		Assert( !ExistChild( A ) && "Duplicate add child actor!" );
		Assert( A->GetParent() == null && "Actor already has parent");

		A->SetParent( this );

		mChildActorList.push_back( A );
	}

	void Actor::SetParent( Actor* A )
	{
		mParent = A;

		// 变换了父级关系后,需要马上更新矩阵
		NeedUpdateTransform( TT_All );
	}

	Actor* Actor::GetChildFirst( )
	{
		mChildIterator = mChildActorList.begin();
	
		return GetChildNext();
	}

	Actor* Actor::GetChildNext( )
	{
		if ( mChildIterator == mChildActorList.end() )
			return null;

		Actor* A = *mChildIterator;

		mChildIterator++;

		return A;
	}

	bool Actor::ExistChild( Actor* A )
	{
		return std::find( mChildActorList.begin(), mChildActorList.end(), A ) != mChildActorList.end() ;
	}

	void Actor::RemoveChild( Actor* A )
	{
		A->SetParent( null );

		mChildActorList.remove( A );
	}

	void Actor::RemoveAllChild( )
	{
		for (ChildActorList::iterator it = mChildActorList.begin(); 
			it != mChildActorList.end();
			++it)
		{
			Actor* A = *it;

			A->SetParent( null );
		}

		mChildActorList.clear();
	}

	void Actor::RefreshCombineBoundingBox( )
	{
		mChildAABB = AABB::cInitBox;

		for (ChildActorList::iterator it = mChildActorList.begin(); 
			it != mChildActorList.end();
			++it)
		{
			Actor* A = *it;

			mChildAABB += A->mSelfAABB;

			A->RefreshCombineBoundingBox();
		}
	}

	void Actor::FindDrawableComponent( RenderQueue* Queue )
	{
		for ( ComponentList::iterator it = mFirstGeometryComponent;
			it != mComponentList.end();
			++it )
		{
			GeometryComponent* Component = RTTI_CAST(GeometryComponent, *it );
			
			Queue->Add( Component );
		}
	}

	void Actor::FindVisibleObject( SceneView* RV, RenderQueue* Queue )
	{
		if ( !this->GetVisible() )
			return;
	
		// 将自己的绑定盒和子节点的绑定盒合起来, 检查绑定盒是否可见
		if ( !RV->IsInFrustum( GetComboWorldBoundingBox() ) )
		{
			return;
		}

		// 将可绘制的组件都放进来
		FindDrawableComponent( Queue );
		
		// 遍历子节点
		
		for (ChildActorList::iterator it = mChildActorList.begin();
			it != mChildActorList.end();
			++it)
		{
			Actor* A = *it;
			
			A->FindVisibleObject( RV, Queue );
		}
	}

	void Actor::SetTestColor( dword Color )
	{
		for ( ComponentList::iterator it = mFirstGeometryComponent;
			it != mComponentList.end();
			++it )
		{
			GeometryComponent* Component = RTTI_CAST(GeometryComponent, *it );

			if ( Component != null )
				Component->mPickID = Color;
		}

	}
	void Actor::SetPickable( bool Pickable )
	{
		mPickable = Pickable;
	}

	bool Actor::GetPickable()
	{
		return mPickable != 0;
	}



	bool Actor::Pick( const Ray& PickRay, float& Distance )
	{
		for ( ComponentList::iterator it = mFirstGeometryComponent;
			it != mComponentList.end();
			++it )
		{
			MeshComponent* Component = RTTI_CAST(MeshComponent, *it );

			if ( Component != null && Component->Pick( PickRay, Distance ) )
				return true;
		}

		return false;
	}

	bool Actor::Pick( SceneView* RV, const FRect& ScreenArea )
	{
		for ( ComponentList::iterator it = mFirstGeometryComponent;
			it != mComponentList.end();
			++it )
		{
			MeshComponent* Component = RTTI_CAST(MeshComponent, *it );

			if ( Component == null)
				continue;

			if ( Component->Pick( RV, ScreenArea) )
				return true;
		}

		return false;
	}


	void Actor::Serialize( BinarySerializer& Ser )
	{
		MovableObject::Serialize( Ser );

		Ser << mFlag;

		for ( ComponentList::iterator it = mComponentList.begin();
			it != mComponentList.end();
			++it )
		{
			ActorComponent* Component = *it;
			
			Component->Serialize( Ser );
		}
	}

	AABB Actor::GetComboWorldBoundingBox()
	{
		return ( mSelfAABB + mChildAABB ) * GetFullTransform();
	}

	AABB Actor::GetWorldBoundingBox()
	{
		return mSelfAABB * GetFullTransform();
	}

	void Actor::SetFlag( dword Flag )
	{
		mFlag = Flag;
	}

	NekoEngine::dword Actor::GetFlag()
	{
		return mFlag;
	}

	bool Actor::HasFlag( dword Mask )
	{
		return (mFlag & Mask) == Mask;
	}

	MovableObject* Actor::GetParentMovable()
	{
		return mParent;
	}

	MovableObject* Actor::GetFirstMovable()
	{
		return GetChildFirst();
	}


	MovableObject* Actor::GetNextMovable()
	{
		return GetChildNext();
	}

	WString Actor::GetDebugName()
	{
		if ( mDebugName != L"")
			return mDebugName;

		return GetName();		
	}

	bool Actor::SetComponentPropertyByString( RTTIClass* Class, const wchar* Name, const wchar* Value )
	{
		ActorComponent* Component = GetComponent( Class );
		if ( Component == null )
			return false;

		return Component->SetObjectPropertyByString( Name, Value, true );
	}

	bool Actor::GetComponentPropertyAsString( RTTIClass* Class, const wchar* Name, WString& Value )
	{
		ActorComponent* Component = GetComponent( Class );
		if ( Component == null )
			return false;

		return Component->GetObjectPropertyAsString( Name, Value, true );
	}

	void Actor::BeginDispose(  )
	{
		RemoveAllChild();

		if ( mParent )
			mParent->RemoveChild( this );
	}



	
}