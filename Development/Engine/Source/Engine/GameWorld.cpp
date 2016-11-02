#include "EnginePCH.h"
#include "NekoEngine_System.h"
#include "NekoEngine_Model.h"
#include "NekoEngine_Extend.h"

namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS( GameWorld )

	const wchar* cNullActorFactoryString = L"@NullActorFactory";
	const wchar* cWorldRootActorName = L"@WorldRoot";

	GameWorld::GameWorld( dword Index )
	{
		mRenderQueue = new RenderQueue;

		mRootActor = GObjectManager->ConstructObject<Actor>( StringHelper::FormatString(L"%s%d",cWorldRootActorName, Index ) );

		mRootActor->AddRef();

		// 绘制网格和箭头
		mRootActor->AddComponent<GridAxisComponent>( );

		// 根节点永远可见
		mRootActor->SetBoundingBox( AABB::cMaxBox );

	
	}

	GameWorld::~GameWorld()
	{
		Kill(  mRootActor );

		mRootActor->RemoveRef();

		mRootActor->RemoveAllChild();

		GObjectManager->DestructObject( mRootActor );

		mRootActor = null;

		for ( ActorFactoryMap::iterator it( mActorFactoryMap );it;++it)
		{
			ActorFactory* Factory = it.get_value();
			delete Factory;
		}

		mActorFactoryMap.clear();


		SafeDelete( mRenderQueue );
	}







	Actor* GameWorld::SpawnActor( RTTIClass* FactoryClass, const WString& Name, Actor* Parent, void* Parameter )
	{
		ActorFactory* Factory = GetActorFactory( FactoryClass );

		if ( Factory == null )
		{
			Factory = mActorFactoryMap.set( FactoryClass, RTTI_CAST(ActorFactory, FactoryClass->CreateInstance() ) );
			Factory->mOwner = this;
		}

		Actor* A = Factory->CreateActor( Name, Parameter );
		if ( A == null )
			return A;

		A->mWorld = this;
		A->mFactoryClass = FactoryClass;

		// World只添加这一级引用,Actor不使用垃圾回收机制
		A->AddRef();

		if ( Parent == null )
			Parent = mRootActor;

		Parent->AddChild( A );

		return A;
	}




	void GameWorld::Draw( SceneView* RV )
	{
		mRenderQueue->Clear();

		mRootActor->RefreshCombineBoundingBox( );

		mRootActor->FindVisibleObject( RV, mRenderQueue );

		mRenderQueue->Draw( RV );
	}

	Camera* GameWorld::GetCamera( const WString& Name )
	{
		CameraActor* CamObj = RTTI_CAST( CameraActor, GObjectManager->FindObject( RTTI_CLASS_INFO(CameraActor), Name ) );
		if ( CamObj == null )
			return null;

		return CamObj->GetCamera();
	}

	void GameWorld::Kill( const WString& Name, RTTIClass* ClassInfo )
	{
		Kill( GetActor( Name, ClassInfo ) );
	}	

	Actor* GameWorld::GetActor( const WString& Name, RTTIClass* ClassInfo  )
	{
		return RTTI_CAST( Actor, GObjectManager->FindObject( ClassInfo, Name ) );
	}

	void GameWorld::Kill( Actor* A )
	{
		// 迭代删除子
		for ( Actor* Child = A->GetChildFirst(); 
			Child != null; 
			Child = A->GetChildNext() )
		{
			Kill( Child );
		}

		// 删除自己
		if ( A != mRootActor )
		{
			A->RemoveAllChild();


			Actor* Parent = A->GetParent();
			
			if ( Parent != null )
				Parent->RemoveChild( A );

			// 如果是摄像机对象，清除其他的关联
			CameraActor* CamA = A->Cast<CameraActor>( );
			if ( CamA != null )
			{
				GEngine->NotifyDestoryCamera( CamA->GetCamera() );
			}

			// 删除World级别的引用
			A->RemoveRef();

			// 提前删除,避免自动垃圾回收
			GObjectManager->DestructObject( A );
		}
	}


	dword GetActorCountViaFlag( Actor* Parent, dword IncludeFlag )
	{
		dword ActorCount = 0;

		for ( Actor* Child = Parent->GetChildFirst(); 
			Child != null; 
			Child = Parent->GetChildNext() )
		{
			// 不能保存编辑器组件
			if ( !Child->HasFlag( IncludeFlag ) )
				continue;

			ActorCount++;
		}

		return ActorCount;
	}

	void GameWorld::SerializeActors( BinarySerializer& Ser, Actor* Parent, dword IncludeFlag, ActorArray* RootChildActors )
	{
		if ( Ser.IsLoading() )
		{
			dword ChildCount = 0;
			WString FactoryClassString;
			WString ActorName;

			Ser << ChildCount
				<< FactoryClassString 
				<< ActorName;

		
			Actor* ThisActor;

			if ( FactoryClassString != cNullActorFactoryString )
			{
				RTTIClass* FactoryClass = RTTIObject::FindClass( NameRef( FactoryClassString.c_str(), NameBuild_Find ) );

				Assert( FactoryClass != null );
				if ( FactoryClass == null )
					return;

				ThisActor = SpawnActor( FactoryClass, ActorName, Parent );


				ThisActor->Serialize( Ser );

				if ( Parent == mRootActor && RootChildActors != null )
				{
					RootChildActors->push_back( ThisActor );
				}
			}
			else
			{
				ThisActor = Parent;
			}


			for (dword i = 0 ;i< ChildCount; i++)
			{
				SerializeActors( Ser, ThisActor, IncludeFlag, RootChildActors );
			}
		}
		else
		{
			

			Ser << GetActorCountViaFlag( Parent , IncludeFlag );

			if ( Parent->mFactoryClass == null )
				Ser << cNullActorFactoryString;
			else
				Ser << Parent->mFactoryClass->GetRTTIClassString();

				Ser << Parent->GetName();

			if ( Parent->mFactoryClass != null )
			{
				Parent->Serialize( Ser );
			}

			for ( Actor* Child = Parent->GetChildFirst(); 
				Child != null; 
				Child = Parent->GetChildNext() )
			{
				// 不能保存编辑器组件
				if ( !Child->HasFlag( IncludeFlag ) )
					continue;

				SerializeActors( Ser, Child, IncludeFlag );
			}
		}
	}

	

	void GameWorld::IsolateFromSystem( Actor* A )
	{
		GObjectManager->RemoveNamedObject( A );

		for ( Actor* Child = A->GetChildFirst();
			Child != null;
			Child = A->GetChildNext() )
		{
			IsolateFromSystem( Child );
		}
	}

	void GameWorld::RestoreFromSystem( Actor* A )
	{
		GObjectManager->AddNamedObject(A);

		for ( Actor* Child = A->GetChildFirst();
			Child != null;
			Child = A->GetChildNext() )
		{
			RestoreFromSystem( Child );
		}
	}

	void GameWorld::Serialize( BinarySerializer& Ser )
	{
		SerializeActors( Ser, mRootActor, 0 );
	}



}