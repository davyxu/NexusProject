#include "EnginePCH.h"
#include "NekoEngine_System.h"


namespace NekoEngine
{
	bool SortPickResult( const WorldPick::PickResult& A, const WorldPick::PickResult& B )
	{
		return A.mDistance < B.mDistance;
	}

	WorldPick::WorldPick( GameWorld* World )
		: mWorld( World )
	{

	}


	bool WorldPick::Pick( const Ray& PickRay , bool SortResult )
	{
		mPickResult.clear();

		PickActor( PickRay, mWorld->GetRootActor() );

		if ( mPickResult.size() == 0 )
			return false;

		if ( SortResult )
			std::sort( mPickResult.begin(), mPickResult.end(), SortPickResult );

		return true;
	}

	bool WorldPick::Pick( const PlaneBoundedVolume& BoundedVolume )
	{
		mPickResult.clear();

		PickActor( BoundedVolume, mWorld->GetRootActor() );

		if ( mPickResult.size() == 0 )
			return false;

		return true;
	}

	void WorldPick::PickActor( const PlaneBoundedVolume& BoundedVolume, Actor* A )
	{
		if ( !A->GetVisible() )
			return;

		if ( A != mWorld->GetRootActor() )
		{
			// 在范围内
			if ( BoundedVolume.Inside( A->GetWorldBoundingBox() ) )
			{
				PickResult Result;
				Result.mActor = A;//@TODO 做一次引用
				Result.mDistance = 0.0f;
				mPickResult.push_back( Result );
			}
		}

		for (Actor::ChildActorList::iterator it = A->mChildActorList.begin();
			it != A->mChildActorList.end();
			++it)
		{
			Actor* A = *it;

			PickActor( BoundedVolume, A );
		}
	}

	void WorldPick::PickActor( const Ray& PickRay, Actor* A )
	{
		if ( A != mWorld->GetRootActor() && A->GetVisible() )
		{
			float nearest = -1.0f, tempdistance = 0.0f;

			Matrix4 InvTransform = A->GetFullTransform().Inverse();

			// Convert ray from world space to mesh space.
			Ray MeshSpaceRay = PickRay * InvTransform;


			if ( Intersection::RayVSAABox( MeshSpaceRay, A->GetBoundingBox(), tempdistance ) )
			{
				tempdistance = 0.0f;
				if ( A->Pick(MeshSpaceRay, tempdistance ) )
				{
					PickResult Result;
					Result.mActor = A;//@TODO 做一次引用
					Result.mDistance = tempdistance;
					mPickResult.push_back( Result );
				}

			}

		}

		for (Actor::ChildActorList::iterator it = A->mChildActorList.begin();
			it != A->mChildActorList.end();
			++it)
		{
			Actor* Child = *it;

			PickActor( PickRay, Child );
		}
	}

	Actor* WorldPick::GetActor( dword Index )
	{
		if ( Index >= mPickResult.size() )
			return null;

		return mPickResult[ Index ].mActor;
	}

	NekoEngine::dword WorldPick::GetCount()
	{
		return mPickResult.size();
	}

	float WorldPick::GetDistance( dword Index )
	{
		if ( Index >= mPickResult.size() )
			return null;

		return mPickResult[ Index ].mDistance;
	}


}