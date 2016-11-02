#include "EnginePCH.h"
#include "NekoEngine_System.h"

namespace NekoEngine
{
	Tickable::TickableList Tickable::mTickableList;


	Tickable::Tickable()
	{
		mTickableList.push_back( this );
	}

	Tickable::~Tickable()
	{
		mTickableList.remove( this );
	}

	void Tickable::StaticTick( float Elapse )
	{
		for ( TickableList::iterator it = mTickableList.begin();
			it != mTickableList.end();
			++it)
		{
			Tickable* Obj = *it;

			if ( Obj->IsTickable() )
				Obj->Tick( Elapse );
		}
	}

}