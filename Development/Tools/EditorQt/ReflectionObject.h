#pragma once

class ReflectionObject
{
public:
	enum{
		InvalidGroupID = -1
	};

	virtual dword GetFirstGroup( ) = 0;

	virtual dword GetNextGroup( ) = 0;

	virtual PropertyValue* GetFirstProperty( dword GroupIndex ) = 0;

	virtual PropertyValue* GetNextProperty( dword GroupIndex ) = 0;

	virtual RTTIObject* GetGroupMappedInstance( dword GroupIndex ) = 0;

	virtual const wchar* GetGroupName( dword GroupIndex ) = 0;
};