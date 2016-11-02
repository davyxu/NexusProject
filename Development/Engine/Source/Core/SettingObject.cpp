#include "CorePCH.h"
#include "NekoCore_Reflection.h"

namespace NekoEngine
{
	void SettingObject::BindMember( const NameRef& objname, void* instancePtr, void* dataPtr, SettingProxy* proxy )
	{
		proxy->mOffset = dword(dataPtr) - dword(instancePtr);

		MemberList& memberlist = mSettingMap[ objname ];
		memberlist[ proxy->mName ] = proxy;
	}

	bool SettingObject::SetMember( const NameRef& objname, void* instancePtr, const char* k, const char* v )
	{
		MemberList& memberlist = mSettingMap[ objname ];

		MemberList::iterator it = memberlist.find( NameRef( k, NameBuild_Find ) );
		if ( it == memberlist.end() )
			return false;

		SettingProxy* proxy = it->second;

		void* newptr = (void*)(dword(instancePtr) + proxy->mOffset);

		return it->second->Parse( newptr , v );
	}

	SettingObject::~SettingObject()
	{
		for ( SettingMap::iterator bit = mSettingMap.begin();
			bit != mSettingMap.end();
			++bit )
		{
			MemberList& memberlist = bit->second;

			for ( MemberList::iterator it = memberlist.begin();
				it != memberlist.end();
				++it )
			{
				delete it->second;
			}

			memberlist.clear();
		}

		mSettingMap.clear();
	}

}