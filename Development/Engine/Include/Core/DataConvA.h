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
	struct typeconvA
	{
		const char* value_typenameA( );
	};

	template<> struct typeconvA<bool>	
	{ 
		const char* value_typenameA() { return "bool";}  
	};

	template<> struct typeconvA<dword>	
	{ 
		const char* value_typenameA() { return "dword";} 
	};

	template<> struct typeconvA<AString>	
	{ 
		const char* value_typenameA() { return "string";}
	};

	template<> struct typeconvA<WString>	
	{ 
		const char* value_typenameA() { return "string";}
	};

	template<> struct typeconvA<int>		
	{ 
		const char* value_typenameA() { return "integer";}
	};

	template<> struct typeconvA<Point>	
	{ 
		const char* value_typenameA() { return "integer2";}
	};

	template<> struct typeconvA<Rect>	
	{ 
		const char* value_typenameA() { return "integer4";}
	};

	template<> struct typeconvA<float>	
	{ 
		const char* value_typenameA() { return "float";}
	};

	template<> struct typeconvA<Vector2>
	{ 
		const char* value_typenameA() { return "float2";}
	};

	template<> struct typeconvA<Vector3>
	{ 
		const char* value_typenameA() { return "float3";}
	};

	template<> struct typeconvA<Vector4>
	{ 
		const char* value_typenameA() { return "float4";}
	};

	template<> struct typeconvA<Matrix4>
	{ 
		const char* value_typenameA() { return "matrix4";}
	};

	template<> struct typeconvA<Color>
	{ 
		const char* value_typenameA() { return "color";}
	};

	

	template<typename T> AString value_tostringA( const T& v );

	template<>	 AString value_tostringA( const bool& v );
	template<>	 AString value_tostringA( const dword& v );
	template<>	 AString value_tostringA( const AString& v );
	template<>	 AString value_tostringA( const WString& v );
	template<>	 AString value_tostringA( const int& v );
	template<>	 AString value_tostringA( const Point& v );
	template<>	 AString value_tostringA( const Rect& v );
	template<>	 AString value_tostringA( const float& v );
	template<>	 AString value_tostringA( const Vector2& v );
	template<>	 AString value_tostringA( const Vector3& v );
	template<>	 AString value_tostringA( const Vector4& v );
	template<>	 AString value_tostringA( const Matrix4& v );
	template<>	 AString value_tostringA( const Color& v );


	template<typename T> bool value_parse( const AString& str, T& v );

	template<>  bool value_parse( const AString& str, bool& v );
	template<>  bool value_parse( const AString& str, dword& v );
	template<>  bool value_parse( const AString& str, AString& v );
	template<>  bool value_parse( const AString& str, WString& v );
	template<>  bool value_parse( const AString& str, int& v );
	template<>  bool value_parse( const AString& str, Point& v );
	template<>  bool value_parse( const AString& str, Rect& v );
	template<>  bool value_parse( const AString& str, float& v );
	template<>  bool value_parse( const AString& str, Vector2& v );
	template<>  bool value_parse( const AString& str, Vector3& v );
	template<>  bool value_parse( const AString& str, Vector4& v );
	template<>  bool value_parse( const AString& str, Matrix4& v );
	template<>  bool value_parse( const AString& str, Color& v );


	template<typename T> T			value_defaultA( );

	template<>	 bool		value_defaultA( );
	template<>	 dword		value_defaultA( );
	template<>	 AString	value_defaultA( );
	template<>	 int		value_defaultA( );
	template<>	 Point		value_defaultA( );
	template<>	 Rect		value_defaultA( );
	template<>	 float		value_defaultA( );
	template<>	 Vector2	value_defaultA( );
	template<>	 Vector3	value_defaultA( );
	template<>	 Vector4	value_defaultA( );
	template<>	 Matrix4	value_defaultA( );
	template<>	 Color		value_defaultA( );


	/** @} */
	/** @} */
}