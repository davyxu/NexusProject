#include "CorePCH.h"
#include "NekoCore_Reflection.h"
#include "Core/Property3.h"



namespace NekoEngine
{
	WString GPropertyNullString;

	PropertyCollection::PropertyCollection()
		: mIterator( null )
	{

	}

	PropertyCollection::~PropertyCollection()
	{
		for ( PropertyMap::iterator it(mPropertyMap);it;++it)
		{
			delete it.get_value();
		}

		mPropertyMap.clear();

		delete mIterator;


		for ( dword i = 0;i< mGroupArray.size();i++)
		{
			PropertyGroup* Group = mGroupArray[i];
			delete Group;
		}

		mGroupArray.clear();
	}

	PropertyValue* PropertyCollection::Add( PropertyValue* Property, PropertyGroup* Group )
	{
		Assert( Property != null );

		// 重名属性
		PropertyValue* MetaInfo = Query( Property->GetName() );
		Assert( MetaInfo == null && "Duplicate property name");
		if ( MetaInfo != null )
			delete MetaInfo;

		Property->mCollection = this;

		mPropertyMap.set( Property->GetName(), Property );

		if ( Group != null )
		{
			Group->AddProperty( Property );
			Property->SetGroup( Group );
		}

		return Property;
	}

	bool PropertyCollection::Set( RTTIObject* Instance, const wchar* Name, PropertyValue& PV )
	{
		return DoAction( Instance, Name, &PV, false );
	}


	bool PropertyCollection::Get( RTTIObject* Instance, const wchar* Name, PropertyValue& PV )
	{
		return DoAction( Instance, Name, &PV, true );
	}


	bool PropertyCollection::ToString( RTTIObject* Instance, const wchar* Name, WString& Value )
	{
		return DoAction(Instance,Name, &PropertyStringValue( Value ), true );
	}

	bool PropertyCollection::Parse( RTTIObject* Instance, const wchar* Name, const wchar* Value )
	{
		return DoAction(Instance,Name, &PropertyStringValue( Value ), false );
	}

	bool PropertyCollection::DoAction( RTTIObject* Instance, const wchar* Name, PropertyValue* PV, bool IsGet )
	{
		PropertyValue* MetaInfo = Query( Name );
		if ( MetaInfo == null )
			return false;

		return MetaInfo->AccessValue( Instance, PV, IsGet );
	}

	PropertyValue* PropertyCollection::Query( const wchar* Name )
	{
		PropertyValue** Result = mPropertyMap.find( Name );
		if ( Result == null )
			return null;

		return *Result;
	}

	PropertyValue* PropertyCollection::GetFirstProperty()
	{
		delete mIterator;
		mIterator = new PropertyMap::iterator( mPropertyMap );

		return GetNextProperty();
	}

	PropertyValue* PropertyCollection::GetNextProperty()
	{
		if ( *mIterator == false )
			return null;

		PropertyValue* Property = mIterator->get_value();

		++(*mIterator);

		return Property;
	}


	void PropertyCollection::SerializeConfig( RTTIObject* Instance, PropertySerializer& Ser )
	{
		// 没有分组时, 需要外部设置分组
		if ( mGroupArray.size() == 0 )
		{
			for ( PropertyMap::iterator it(mPropertyMap);it;++it)
			{
				PropertyValue* PV = it.get_value();

				PV->SerializeConfig( Instance, Ser, false );
			}
		}
		else
		{
			// 分组写入
			for ( dword GroupIndex = 0; GroupIndex < mGroupArray.size(); GroupIndex++ )
			{
				PropertyGroup* Group = mGroupArray[GroupIndex];

				Ser.SerializeSection( Group->GetName() );

				for ( dword PVIndex = 0; PVIndex < Group->GetPropertyCount(); PVIndex++ )
				{
					PropertyValue* PV = Group->GetPropery( PVIndex );

					PV->SerializeConfig( Instance, Ser, false );
				}
			}
		}
	}

	bool PropertyValue::ToString( RTTIObject* Instance, WString& Value )
	{
		return AccessValue( Instance, &PropertyStringValue( Value ), true );
	}

	bool PropertyValue::Parse( RTTIObject* Instance, const wchar* Value )
	{
		return AccessValue( Instance, &PropertyStringValue( Value ), false );
	}

	bool PropertyValue::MatchType( PropertyValue* PV )
	{
		return StringHelper::Compare( mTypeName, PV->mTypeName );
	}

	PropertyGroup* PropertyCollection::AddGroup( const WString& Name )
	{
		PropertyGroup* Group = new PropertyGroup( Name );

		mGroupArray.push_back( Group );

		return Group;
	}

	PropertyGroup* PropertyCollection::QueryGroup( const WString& Name )
	{
		for ( dword i = 0;i< mGroupArray.size();i++)
		{
			PropertyGroup* Group = mGroupArray[i];
			if ( Group->GetName() == Name )
			{
				return Group;
			}
		}

		return null;
	}

	PropertyGroup* PropertyCollection::GetGroup( dword Index )
	{
		return mGroupArray[ Index ];
	}
}


