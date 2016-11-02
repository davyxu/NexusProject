#include "EnginePCH.h"
#include "NekoEngine_System.h"


namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// ManagedObject
	//////////////////////////////////////////////////////////////////////////

	ObjectManager* GObjectManager = null;

	IMPLEMENT_RTTI_CLASS( ManagedObject )

	dword GetObjectHash( const WString& Name )
	{
		return GetTypeHash(Name) & ( ObjectManager::OBJECT_HASH_BINS - 1);
	}


	ManagedObject::ManagedObject()
		: mHashNext( null )
		, mObjectIndex( -1 )
		, mIsDisposing( false )
		, mPackageResourceReady( false )
		, mSpawnType( OST_Unknown )
	{
		
	}

	ManagedObject::~ManagedObject()
	{
		
	}

	void ManagedObject::SetName( const WString& Name )
	{
		mName = Name;

		GObjectManager->RemoveNamedObject( this );
		GObjectManager->AddNamedObject( this );
	}


	bool ManagedObject::IsPackageResourceReady()
	{
		return mPackageResourceReady;
	}
	//////////////////////////////////////////////////////////////////////////
	// ObjectManager
	//////////////////////////////////////////////////////////////////////////
	
	IMPLEMENT_RTTI_CLASS( ObjectManager )

	ObjectManager::ObjectManager()
		: mContentPath( L"..\\Content" )
		, GObjectCount( 0 )
		, GPurgeObjectIndex( 0 )		
	{
		::memset(GObjectHash, 0, sizeof GObjectHash );

		GObjectLogger = new SimpleLog;
		GObjectLogger->SetPrefix(L"Object");
	}

	ObjectManager::~ObjectManager()
	{
		SafeDelete( GObjectLogger );
	}

	void ObjectManager::StaticInitProperty( )
	{
		PropertyCollection* PC = RTTI_CLASS_INFO(ObjectManager)->GetPropertyCollection( );

		PropertyGroup* Group = PC->AddGroup( L"ObjectManager" );

		PC->Add( PROPERTY_MEMBER_VAR( ObjectManager,WString, mContentPath )	, Group );		
	}

	void ObjectManager::SerializeConfig( PropertySerializer& Ser )
	{
		Super::SerializeConfig( Ser );

		Ser.SerializeSection(L"ObjectManager.Logger");
		GetLogger()->SerializeConfig( Ser );		
	}


	void ObjectManager::SetContentPath( const WString& Value )
	{
		mContentPath = Value;
	}

	ManagedObject* ObjectManager::ConstructObject( RTTIClass* ClassInfo, const WString& Name )
	{
		ManagedObject* Obj = RTTI_CAST(ManagedObject, ClassInfo->CreateInstance() );
		if ( Obj == null )
			return null;

		if ( !LinkObject( Obj, Name ) )
		{
			delete Obj;
			return null;
		}

		const wchar* TypeName = RTTI_INSTANCE_STRING( Obj );
		GObjectLogger->Debug(L"Construct Object [%s] %s", TypeName, Obj->GetName().c_str() );

		Obj->mSpawnType = OST_Constructed;

		return Obj;
	}

	bool ObjectManager::LinkObject( ManagedObject* Obj, const WString& Name )
	{
		if ( Name != L"" )
		{
			ManagedObject* ExistObj = FindObject( RTTI_INSTANCE_CLASS(Obj), Name );

			Assert( ExistObj == null && "Name duplicate" );

			Obj->mName = Name;
		}
		else
		{
			Obj->mName = MakeUniqueObjectName( RTTI_INSTANCE_CLASS(Obj) );
		}

		
		Obj->AddRef();

		AddNamedObject( Obj );

		return true;
	}


	void ObjectManager::DestructObject( ManagedObject* Obj )
	{
		const wchar* TypeName = RTTI_INSTANCE_STRING( Obj );
		GObjectLogger->Debug(L"Destruct Object [%s] %s", TypeName, Obj->GetName().c_str() );

		RemoveNamedObject( Obj );

		Obj->RemoveRef();

	}


	WString ObjectManager::MakeUniqueObjectName( RTTIClass* ClassInfo )
	{
		dword ClassNameIndex = ClassInfo->GetRTTIClassName().GetIndex();
		wchar Buffer[256];

		do 
		{
			// 序号从1开始， Actor_1 , Actor_2
			StringHelper::FormatBuffer( Buffer, 256, L"%s_%d", ClassInfo->GetRTTIClassString(), ++ClassInfo->mClassUnique );

		} while ( FindObject( ClassInfo, Buffer ) );

		return Buffer;
	}




	ManagedObject* ObjectManager::FindObject( RTTIClass* ClassInfo, const WString& Name )
	{
		dword ObjHash = GetObjectHash( Name );
		for ( ManagedObject* Obj = GObjectHash[ObjHash]; Obj != null; Obj = Obj->mHashNext )
		{
			if ( Obj->GetName() == Name && Obj->IsA( ClassInfo ) )
			{
				return Obj;
			}
		}

		return null;
	}

	ManagedObject* ObjectManager::FindFirstObjectByKind( RTTIClass* ClassInfo, const WString& Name )
	{
		dword ObjHash = GetObjectHash( Name );
		for ( ManagedObject* Obj = GObjectHash[ObjHash]; Obj != null; Obj = Obj->mHashNext )
		{
			if ( Obj->GetName() == Name && Obj->IsKindOf( ClassInfo ) )
			{
				return Obj;
			}
		}

		return null;
	}

	ManagedObject* ObjectManager::FindNextObjectByKind(  RTTIClass* ClassInfo, ManagedObject* KindOfObject )
	{
		if ( KindOfObject == null )
			return null;

		ManagedObject* NextObject = KindOfObject->mHashNext;
		if ( NextObject->GetName() == KindOfObject->GetName() &&
			 NextObject->IsKindOf( ClassInfo ) )
		{
			return NextObject;
		}

		return null;
	}


	void ObjectManager::CollectGarbage( eGarbageCollectPolicy Policy )
	{
		do 
		{
			ObjectList DeleteList;

			GPurgeObjectIndex = 0;

			while( GPurgeObjectIndex < GObjects.size() )
			{
				ManagedObject* Obj = GObjects[ GPurgeObjectIndex++ ];

				if ( Obj == null )
					continue;

				if ( Obj->GetRefCount() > 1 )
					continue;

				if ( Obj->IsDisposing() )
					continue;

				if ( Obj->mSpawnType == OST_Loaded && !Obj->IsResourceReady() )
					continue;
		
				const wchar* TypeName = RTTI_INSTANCE_STRING( Obj );

				Obj->BeginDispose( );

				Obj->mIsDisposing = true;

				GDisposingList.push_back( Obj );
			}


			// 释放所有时,强制同步等待渲染线程,加速卸载
			if ( Policy == GCP_Finalize )
				GRenderThread.Flush();

			for ( ObjectList::iterator it = GDisposingList.begin();
				it != GDisposingList.end(); )
			{
				ManagedObject* Obj = *it;

				dword RefCount = Obj->GetRefCount();

				// Dispose时,没有任何人有引用了
				Assert( RefCount == 1 );

				const wchar* TypeName = RTTI_INSTANCE_STRING( Obj );

				// 外部没有任何引用时,卸载
				if ( Obj->CanDestroyNow()  )
				{
					DestructObject( Obj );

					it = GDisposingList.erase( it );
				}
				else
				{
					++it;
				}
			}

			// 非退出方式, 直接出去
			if ( Policy != GCP_Finalize )
				break;

		// 直到所有对象释放
		} while ( GObjectCount >0 );

	}



	ManagedObject* ObjectManager::LoadObject( RTTIClass* ClassInfo, const WString& Name, bool Background )
	{
		ManagedObject* MO = FindObject( ClassInfo, Name );

		if ( MO != null )
			return MO;

		FileStream* Stream = new FileStream( ResourceLocation::Resovle( Name ).c_str() , FAM_Read );

		if ( !Stream->IsValid() )
		{
			//mLogger->Error(L"Can not open resource %s", Name.c_str() );

			delete Stream;
			return null;
		}

		BinarySerializer* Ser  = new BinarySerializer( Stream, true );

		Assert( ClassInfo != null );

		if ( ClassInfo->HasFlag( ClassFlag_Serializable ) )
		{
			Ser->SerializerFileHeader( 0 );

			MO = RTTI_CAST(ManagedObject, Ser->ReadObject( ) );

			LinkObject( MO, Name  );
		}
		else
		{
			MO = RTTI_CAST(ManagedObject, ConstructObject( ClassInfo, Name ) );
		}

		MO->mSpawnType = OST_Loaded;

		Assert( MO != null );

		ObjectSerializeObserver* Objserver = new ObjectSerializeObserver( MO, Ser );

		if ( GAsyncTaskThread.IsEnable() && Background )
		{
			Objserver->PostAsync();

			GLoadingList.insert( Objserver );
		}
		else
		{
			Objserver->StartSync();
		}

		return MO;
	}


	bool ObjectManager::SaveToFile( ManagedObject* MO, dword FileVersion )
	{
		Assert( MO != null );

		if ( MO == null )
			return false;

		FileStream* Stream = new FileStream( ResourceLocation::Resovle( MO->GetName() ).c_str() , FAM_Write );

		if ( !Stream->IsValid() )
		{
			//mLogger->Error(L"Can not open resource %s", Name.c_str() );

			delete Stream;
			return false;
		}

		BinarySerializer Ser( Stream, false );		

		if ( MO->GetClass()->HasFlag( ClassFlag_Serializable ) )
		{
			Ser.SerializerFileHeader( FileVersion );

			Ser.WriteObject( MO );
		}

		MO->mSpawnType = OST_Savable;		

		MO->Serialize( Ser );

		delete Stream;

		return true;
	}


	void ObjectManager::CheckResourceProcessing( )
	{
		if ( GLoadingList.size() == 0 )
			return;

		for ( ObjectSerializeList::iterator it = GLoadingList.begin();
			it != GLoadingList.end(); )
		{
			ObjectSerializeObserver* Observer = *it;

			if ( Observer->IsFinished() )
			{
				delete Observer;

				it = GLoadingList.erase( it );
			}
			else
			{
				++it;
			}
		}
	}




	//////////////////////////////////////////////////////////////////////////
	// ObjectSerializeObserver
	//////////////////////////////////////////////////////////////////////////

	ObjectSerializeObserver::ObjectSerializeObserver( ManagedObject* Obj, BinarySerializer* Ser )
		: mObject( Obj )
		, mSerializer( Ser )
	{
		
	}


	ObjectSerializeObserver::~ObjectSerializeObserver( )
	{
		delete mSerializer->GetStream();

		SafeDelete( mSerializer );
	}

	void ObjectSerializeObserver::StartSync( )
	{
		OnAsyncTaskProcess();
		OnAsyncTaskFinished();
		delete this;
	}

	void ObjectSerializeObserver::PostAsync( )
	{
		// 添加到后台加载
		GAsyncTaskThread.Add( this );
	}

	void ObjectSerializeObserver::OnAsyncTaskProcess()
	{
		mObject->Serialize( *mSerializer );
	}

	void ObjectSerializeObserver::OnAsyncTaskFinished()
	{
		mObject->PostLoad();

		mObject->MarkPackageResourceReady();

		GObjectManager->GetLogger()->Info(L"Load Resource [%s] %s", RTTI_INSTANCE_STRING(mObject), mObject->GetName().c_str() );
	}

	bool ObjectSerializeObserver::IsFinished()
	{
		return mObject->IsResourceReady();
	}



	void ObjectManager::AddNamedObject( ManagedObject* MO )
	{
		// 加入时保证没有重名
		Assert( FindObject( RTTI_INSTANCE_CLASS(MO), MO->mName ) == null );

		dword NewIndex;
		if ( GFreeObjectArray.size() > 0 )
		{
			NewIndex = GFreeObjectArray.back();

			GFreeObjectArray.pop_back();

			Assert( GObjects[NewIndex] == null );
		}
		else
		{
			GObjects.push_back( null );
			NewIndex = GObjects.size() - 1;
		}

		GObjects[ NewIndex ] = MO;
		MO->mObjectIndex = NewIndex;

		// Hash Object
		dword ObjHash = GetObjectHash( MO->mName );

		MO->mHashNext = GObjectHash[ ObjHash ];
		GObjectHash[ ObjHash ] = MO;

		GObjectCount++;
	}

	void ObjectManager::RemoveNamedObject( ManagedObject* MO )
	{
		if ( MO->mObjectIndex != -1 )
		{
			dword ObjHash = GetObjectHash( MO->mName );

			ManagedObject** Hash = &GObjectHash[ ObjHash ];

#if _DEBUG
			dword Removed = 0;
#endif

			while ( *Hash != null )
			{
				if( *Hash != MO )
				{
					Hash = &(*Hash)->mHashNext;
				}
				else
				{
					*Hash = (*Hash)->mHashNext;
#if _DEBUG
					// Verify that we find one and just one object in debug builds.		
					Removed++;
#else
					break;
#endif
				}
			}

#if _DEBUG
			Assert(Removed != 0);
			Assert(Removed == 1);
#endif

			GObjects[ MO->mObjectIndex ] = null;
			GObjectCount--;
			GFreeObjectArray.push_back( MO->mObjectIndex );

			MO->mObjectIndex = -1;
		}
	}


}
