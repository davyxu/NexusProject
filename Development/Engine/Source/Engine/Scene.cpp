#include "EnginePCH.h"
#include "NekoEngine_System.h"

namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS(SceneListener)

	SceneMaster::SceneMaster( ) 
		: mCurrent( null )
		, mNextGoto( null )
	{
		mLogger.SetPrefix(L"SceneMaster");
	}

	SceneMaster::~SceneMaster()
	{
		Release();
	}

	void SceneMaster::Create(  )
	{
		// ´ÓRTTI´´½¨
		RTTIGroup* Group = RTTIObject::GetGroup( L"SceneListener" );

		StringConverter conv;

		for ( RTTIClass* ClassInfo = Group->GetFirstClass();
			ClassInfo != null;
			ClassInfo = Group->GetNextClass() )
		{
			SceneListener* Scene = RTTI_CAST( SceneListener, ClassInfo->CreateInstance( ) );
			Assert( Scene != null );
			mSceneMap[ ClassInfo->mClassName ] = Scene;

			Scene->OnCreate();
		}

		if ( mSceneMap.size() >= 2 )
			mIterator = mSceneMap.begin();
	}


	void SceneMaster::Update( float elapse )
	{
		if ( mNextGoto != null )
		{
			if ( mCurrent != null )
			{
				
				mLogger.Debug(L"%s	OnLeave start", RTTI_INSTANCE_STRING( mCurrent ) );
				mCurrent->OnLeave();
			}

			mLogger.Debug(L"%s	OnEnter start", RTTI_INSTANCE_STRING( mNextGoto ) );
			mNextGoto->OnEnter();

			mCurrent = mNextGoto;
			mNextGoto = null;
		}


		if ( mCurrent != null )
			mCurrent->OnUpdate( elapse );
	}

	void SceneMaster::Destroy( )
	{
		if ( mCurrent != null )
		{
			mLogger.Debug(L"%s	OnLeave start", RTTI_INSTANCE_STRING( mCurrent ) );
			mCurrent->OnLeave();
		}

		for ( SceneMap::iterator it = mSceneMap.begin();
			it != mSceneMap.end();
			++it )
		{
			it->second->OnDestroy();
		}
	}

	void SceneMaster::Release()
	{
		for ( SceneMap::iterator it = mSceneMap.begin();
			it != mSceneMap.end();
			++it )
		{
			SceneListener* Listener = it->second;
			delete Listener;
		}

		mSceneMap.clear();		
	}

	

	void SceneMaster::Goto( const NameRef& SceneClassName )
	{
		SceneMap::iterator it = mSceneMap.find( SceneClassName );
		if ( it == mSceneMap.end() )
			return;

		mNextGoto = it->second;
		mIterator = it;
	}

	void SceneMaster::GotoPrev( )
	{
		if ( mSceneMap.size() < 2)
			return;

		--mIterator;

		if ( mIterator == mSceneMap.end() )
			mIterator = mSceneMap.begin();

		Goto( mIterator->first );
	}

	void SceneMaster::GotoNext( )
	{
		if ( mSceneMap.size() < 2)
			return;

		++mIterator;

		if ( mIterator == mSceneMap.end() )
			mIterator = mSceneMap.begin();

		Goto( mIterator->first );
	}

	const wchar* SceneMaster::GetCurrentSceneName()
	{
		if ( mCurrent == null )
			return L"No scene";

		return RTTI_INSTANCE_STRING( mCurrent );
	}

	SceneListener* SceneMaster::Get( const NameRef& SceneClassName )
	{
		SceneMap::iterator it = mSceneMap.find( SceneClassName );
		if ( it == mSceneMap.end() )
			return null;

		return it->second;
	}


}