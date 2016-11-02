#include "WrapperPCH.h"
#include "PrivateInclude.h"
#include <vcclr.h>

namespace Wrapper
{
	const wchar_t* Utility::ToCharPtr( System::String^ str )
	{
		pin_ptr<const WCHAR> nativeString1  = PtrToStringChars( str );
		return (const wchar_t*)nativeString1;
	}

	#define TYPE_MAPPING( TType ) mTypeMapping->Add( #TType, TType::typeid )

	Utility::Utility()
		: mTypeMapping( gcnew Hashtable())
	{
		TYPE_MAPPING( float );
		TYPE_MAPPING( bool );
		TYPE_MAPPING( Vector3 );
	}

	Type^ Utility::ToManagedType( String^ TypeName )
	{
		return (Type^)mTypeMapping[TypeName];
	}


}
