#include "CorePCH.h"
#include "NekoCore_Reflection.h"

namespace NekoEngine
{
	void EnumObject::AddMember( const char* k, dword v )
	{
		mValueMapping[NameRef(k)] = v;
	}

	bool EnumObject::GetValue( const char* k, dword& v )
	{
		ValueMapping::iterator it = mValueMapping.find( NameRef(k, NameBuild_Find ) );
		if ( it == mValueMapping.end() )
			return false;

		v = it->second;

		return true;
	}



	void EnumObject::StaticExit()
	{
		// 注意，如果枚举对象在DLL中，这里释放可能出问题
		for ( EnumObjectList::iterator it = mEnumObjectList.begin();
			it != mEnumObjectList.end();
			++it )
		{
			it->second->ReleaseRTTIObject();
		}

		mEnumObjectList.clear();

		RTTIObject::UnRegisterPackage("EnumObject");
	}

	EnumObject::EnumObjectList EnumObject::mEnumObjectList;

	bool EnumObject::GetEnumValue( const char* enumname, const char* k, dword& v )
	{
		NameRef refEnumName( enumname, NameBuild_Find );

		// 在已经有的列表中找
		EnumObjectList::iterator it = mEnumObjectList.find( refEnumName );

		EnumObject* enumobj;

		if ( it == mEnumObjectList.end() )
		{
			// 新创建
			enumobj = (EnumObject*)RTTIObject::CreateInstance( refEnumName, NameRef("EnumObject",NameBuild_Find) );

			// 加入创建列表
			mEnumObjectList[ refEnumName ] = enumobj;
		}
		else
		{
			enumobj = it->second;
		}

		// 枚举不存在
		if ( enumobj == null )
			return false;

		// 取值
		bool r = enumobj->GetValue( k, v );

		return r;
	}






}