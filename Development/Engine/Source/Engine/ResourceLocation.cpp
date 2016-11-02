#include "EnginePCH.h"
#include "NekoEngine_System.h"

namespace NekoEngine
{
	struct ResourceNameMapping
	{
		const wchar* mExtName;
	};


	ResourceNameMapping RESOURCE_NAME_MAPPING[RFT_MaxResourceType] =
	{
		{ L"" },
		{ L".SA" },
		{ L".SK" },
		{ L".SM" },
		{ L".DDS"},
		{ L".MI" },
		{ L".MT" },
		{ L".SF" },
	};


	WString ResourceLocation::GetExtension( eResourceType Type )
	{
		if ( Type >= RFT_MaxResourceType )
			return L"";

		return RESOURCE_NAME_MAPPING[ Type ].mExtName;
	}



	// EngineMaterial.DiffuseLighting.MT
	// EngineTexture.test.DDS
	WString ResourceLocation::Resovle(const WString& ResourceName )
	{
		wchar Buffer[2048];
		WString FilePath = StringHelper::FormatBuffer( Buffer, 256, L"%s\\%s", GObjectManager->GetContentPath().c_str(), ResourceName.c_str() );

		dword ExtDiv = FilePath.rfind( '.' );


		dword EndLength = ( ExtDiv == WString::npos  ? FilePath.length() : ExtDiv );

		for ( dword i = GObjectManager->GetContentPath().length();i<EndLength;i++)
		{
			if ( FilePath[ i ] == '.' )
				FilePath[ i ] = '\\';
		}

		return FilePath;
	}

	WString ResourceLocation::Build( const WString& FileName )
	{
		WString ResourceRelativeName = Path::RelativePathTo( GObjectManager->GetContentPath().c_str(), FileName.c_str(), true , false );

		dword ExtDiv = ResourceRelativeName.rfind( '.' );


		dword EndLength = ( ExtDiv == WString::npos  ? ResourceRelativeName.length() : ExtDiv );

		for ( dword i = 0;i<EndLength;i++)
		{
			if ( ResourceRelativeName[ i ] == '\\' )
				ResourceRelativeName[ i ] = '.';
		}

		return ResourceRelativeName;
	}


	eResourceType ResourceLocation::ParseType( const WString& ResourceName )
	{
		dword ExtDiv = ResourceName.rfind( '.' );
		if ( ExtDiv == WString::npos )
			return RFT_Unknown;

		WString TypeName = ResourceName.substr( ExtDiv, ResourceName.size() - ExtDiv );


		for( dword i = 0; i < RFT_MaxResourceType;i ++)
		{
			if (StringHelper::Compare( GetExtension( (eResourceType)i ).c_str(), TypeName.c_str(), 3 ) )
			{
				return eResourceType( i );
			}
		}

		return RFT_Unknown;
	}


	bool ResourceLocation::ModifyType( const WString& ResourceName, WString& OutResourceName, eResourceType Type  )
	{
		eResourceType RType = ParseType( ResourceName );
		if ( RType == RFT_Unknown )
			return false;

		dword ExtDiv = ResourceName.rfind( '.' );
		if ( ExtDiv == WString::npos )
			return RFT_Unknown;

		OutResourceName = ResourceName.substr( 0, ExtDiv ) + GetExtension( Type );

		return true;
	}

}