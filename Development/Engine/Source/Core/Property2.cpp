#include "CorePCH.h"
#include "NekoCore_Reflection.h"

namespace NekoEngine
{
	ObjectPropertySet::ObjectPropertySet()
		: mEmptyProperty( null )
	{

	}

	ObjectPropertySet::~ObjectPropertySet( )
	{
		for ( PropertyMap::iterator it = mPropertyMap.begin();
			it != mPropertyMap.end();
			++it )
		{
			delete ( it->second );
		}

		mPropertyMap.clear();
	}


	ObjectProperty* ObjectPropertySet::Add( ObjectProperty* prop )
	{
		mPropertyMap.insert( std::pair<NameRef, ObjectProperty*>( prop->GetNameRef(), prop ) );
		prop->SetDefault();
		return prop;
	}

	void ObjectPropertySet::Remove( const NameRef& name )
	{
		PropertyMap::iterator it = mPropertyMap.find( name );
		if ( it == mPropertyMap.end() )
			return;

		delete (it->second);

		mPropertyMap.erase( it );
	}

	ObjectProperty* ObjectPropertySet::Query( const NameRef& name )
	{
		PropertyMap::iterator it = mPropertyMap.find( name );
		if ( it == mPropertyMap.end() )
			return null;

		return it->second;
	}

	ObjectProperty* ObjectPropertySet::GetFirst()
	{
		mIterator = mPropertyMap.begin();

		return GetNext();
	}

	ObjectProperty* ObjectPropertySet::GetNext()
	{
		if ( mIterator == mPropertyMap.end() )
			return null;

		ObjectProperty* prop = mIterator->second;
		++mIterator;
		return prop;
	}

}

