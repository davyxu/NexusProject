#include "EnginePCH.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// RenderQueueGroup
	//////////////////////////////////////////////////////////////////////////

	struct DepthSortDescendingLess 
	{
		Camera* mCamera;

		DepthSortDescendingLess(Camera* Cam )
			: mCamera( Cam )
		{

		}

		bool operator( )( GeometryComponent* A, GeometryComponent* B )
		{
			float DepthA = A->GetViewDepthSquared( mCamera );
			float DepthB = B->GetViewDepthSquared( mCamera );

			// 从远排到近
			return DepthA > DepthB;
		}
	};

	RenderQueueGroup::RenderQueueGroup( dword GroupID )
		: mGroupID( GroupID )
	{

	}

	void RenderQueueGroup::Add( GeometryComponent* Component )
	{
		Assert( Component != null );

		if ( Component->IsEnableDepthWrite( ) )
		{
			if ( Component->IsTransparent() )
				mTransparentRenderList.push_back( Component );
			else
				mOpaqueRenderList.push_back( Component );
		}
		else
		{
			mSolidRenderList.push_back( Component );
		}
		
	}

	void RenderQueueGroup::Clear()
	{
		mOpaqueRenderList.clear();
		mTransparentRenderList.clear();
		mSolidRenderList.clear();

	}

	void RenderQueueGroup::Draw( SceneView* RV )
	{
		// 绘制不透明体
		DrawRenderList( RV, mOpaqueRenderList );

		if ( mTransparentRenderList.size() > 0 )
		{
			// 透明排序
			std::stable_sort( mTransparentRenderList.begin(), mTransparentRenderList.end(), DepthSortDescendingLess( RV->GetCamera() ) );

			DrawRenderList( RV, mTransparentRenderList );
		}
		

		if ( mSolidRenderList.size() > 0 )
		{
			std::stable_sort( mSolidRenderList.begin(), mSolidRenderList.end(), DepthSortDescendingLess( RV->GetCamera() ) );

			DrawRenderList( RV, mSolidRenderList );
		}
		
	}



	void RenderQueueGroup::DrawRenderList( SceneView* RV, RenderList& RList )
	{
		for ( RenderList::iterator it = RList.begin();
			it != RList.end();
			++it )
		{
			GeometryComponent* Component = *it;
			Component->Draw( RV );
		}

	}



	//////////////////////////////////////////////////////////////////////////
	// RenderQueue
	//////////////////////////////////////////////////////////////////////////

	RenderQueue::RenderQueue()
		: mNeedUpdateGroupOrder( false )
	{

	}

	RenderQueue::~RenderQueue( )
	{
		for ( RenderGroupMap::iterator it = mRenderGroupMap.begin();
			it != mRenderGroupMap.end();
			++it )
		{
			RenderQueueGroup* Group = it->second;
			delete Group;
		}

		mRenderGroupMap.clear();

		mRenderGroupArray.clear();
	}

	void RenderQueue::Add( GeometryComponent* Component )
	{
		RenderQueueGroup* Group = GetRenderGroup( Component->GetRenderQueueGroup() );

		Assert( Group != null );

		Group->Add( Component );
	}

	RenderQueueGroup* RenderQueue::GetRenderGroup( dword GroupID )
	{
		RenderGroupMap::iterator it = mRenderGroupMap.find( GroupID );
		if ( it == mRenderGroupMap.end() )
		{
			RenderQueueGroup* Group = new RenderQueueGroup( GroupID );
			mRenderGroupMap.insert( std::pair<dword, RenderQueueGroup*>( GroupID, Group ) );

			mNeedUpdateGroupOrder = true;

			return Group;
		}

		return it->second;
	}

	void RenderQueue::Clear()
	{
		for ( RenderGroupMap::iterator it = mRenderGroupMap.begin();
			it != mRenderGroupMap.end();
			++it )
		{
			RenderQueueGroup* Group = it->second;
			Group->Clear();
		}
	}

	struct RenderGroupSort
	{
		bool operator( )( RenderQueueGroup* A, RenderQueueGroup* B )
		{
			return A->GetGroupID() < B->GetGroupID();			
		}
	};

	void RenderQueue::Draw( SceneView* RV )
	{
		if ( mNeedUpdateGroupOrder )
		{
			mRenderGroupArray.resize( mRenderGroupMap.size() );
			dword Index = 0;
			for ( RenderGroupMap::iterator it = mRenderGroupMap.begin();
				it != mRenderGroupMap.end();
				++it )
			{
				RenderQueueGroup* Group = it->second;
				mRenderGroupArray[ Index++ ] = Group;
			}

			std::stable_sort( mRenderGroupArray.begin(), mRenderGroupArray.end(), RenderGroupSort( ) );

			mNeedUpdateGroupOrder = false;
		}

		for ( dword i = 0;i< mRenderGroupArray.size();i++)
		{
			RenderQueueGroup* Group = mRenderGroupArray[i];
			Group->Draw( RV );
		}
	}


}