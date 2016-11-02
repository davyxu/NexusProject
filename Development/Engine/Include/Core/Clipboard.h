#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Platform
	*  @{
	*/


	/** 
		¼ôÌù°å·â×°
	*/
	class Clipboard
	{
	public:
		
		/// ¿½±´×Ö·û´®µ½¼ôÌù°å
		static bool Copy( const wchar* string );

		/// ´Ó¼ôÌù°åÈ¡µÃ×Ö·û´®
		static bool Paste( WString& string );
	};

};