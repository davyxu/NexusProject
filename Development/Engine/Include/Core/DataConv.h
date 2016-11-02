#pragma once


namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Reflection
	*  @{
	*/

	// Type to enum, Type to type string
	template<typename T>
	struct typeconv
	{
		const wchar* value_typenameW( );
	};

	template<> struct typeconv<bool>	
	{ 
		const wchar* value_typenameW() { return L"bool";}  
	};

	template<> struct typeconv<dword>	
	{ 
		const wchar* value_typenameW() { return L"dword";} 
	};

	template<> struct typeconv<word>	
	{ 
		const wchar* value_typenameW() { return L"word";} 
	};

	template<> struct typeconv<WString>	
	{ 
		const wchar* value_typenameW() { return L"WString";}
	};

	template<> struct typeconv<int>		
	{ 
		const wchar* value_typenameW() { return L"int";}
	};

	template<> struct typeconv<Point>	
	{ 
		const wchar* value_typenameW() { return L"Point";}
	};

	template<> struct typeconv<Rect>	
	{ 
		const wchar* value_typenameW() { return L"Rect";}
	};

	template<> struct typeconv<float>	
	{ 
		const wchar* value_typenameW() { return L"float";}
	};

	template<> struct typeconv<Vector2>
	{ 
		const wchar* value_typenameW() { return L"Vector2";}
	};

	template<> struct typeconv<Vector3>
	{ 
		const wchar* value_typenameW() { return L"Vector3";}
	};

	template<> struct typeconv<Vector4>
	{ 
		const wchar* value_typenameW() { return L"Vector4";}
	};

	template<> struct typeconv<Matrix4>
	{ 
		const wchar* value_typenameW() { return L"Matrix4";}
	};

	template<> struct typeconv<Color>
	{ 
		const wchar* value_typenameW() { return L"Color";}
	};


	 WString value_tostringW( const bool& v );
	 WString value_tostringW( const dword& v );
	 WString value_tostringW( const word& v );
	 WString value_tostringW( const WString& v );
	 WString value_tostringW( const int& v );
	 WString value_tostringW( const Point& v );
	 WString value_tostringW( const Rect& v );
	 WString value_tostringW( const float& v );
	 WString value_tostringW( const Vector2& v );
	 WString value_tostringW( const Vector3& v );
	 WString value_tostringW( const Vector4& v );
	 WString value_tostringW( const Matrix4& v );
	 WString value_tostringW( const Color& v );
	 WString value_tostringW( const wchar* v );
	 WString value_tostringW( const Quaternion& v );


	bool value_parse( const wchar* str, bool& v );
	bool value_parse( const wchar* str, dword& v );
	bool value_parse( const wchar* str, word& v );
	bool value_parse( const wchar* str, WString& v );
	bool value_parse( const wchar* str, int& v );
	bool value_parse( const wchar* str, Point& v );
	bool value_parse( const wchar* str, Rect& v );
	bool value_parse( const wchar* str, float& v );
	bool value_parse( const wchar* str, Vector2& v );
	bool value_parse( const wchar* str, Vector3& v );
	bool value_parse( const wchar* str, Vector4& v );
	bool value_parse( const wchar* str, Matrix4& v );
	bool value_parse( const wchar* str, Color& v );


	template<typename T> T			value_default( );

	template<>	 bool		value_default( );
	template<>	 dword		value_default( );
	template<>	 word		value_default( );
	template<>	 WString	value_default( );
	template<>	 int		value_default( );
	template<>	 Point		value_default( );
	template<>	 Rect		value_default( );
	template<>	 float		value_default( );
	template<>	 Vector2	value_default( );
	template<>	 Vector3	value_default( );
	template<>	 Vector4	value_default( );
	template<>	 Matrix4	value_default( );
	template<>	 Color		value_default( );


	/** @} */
	/** @} */
}