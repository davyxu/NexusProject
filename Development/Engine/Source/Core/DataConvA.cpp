#include "CorePCH.h"
#include "NekoCore_Reflection.h"

namespace NekoEngine
{
	static const dword typeconv_buffersize = 512;

	template<>
	AString value_tostringA( const bool& v )
	{ 
		return (v ? "true":"false" );
	}

	template<>
	AString value_tostringA( const dword& v )
	{ 
		char buff[typeconv_buffersize];
		::_ultoa(v, buff, 10 ); 
		return buff;
	}

	template<>
	AString value_tostringA( const AString& v )
	{ 
		return v;
	}

	template<>
	AString value_tostringA( const WString& v )
	{ 
		return StringConverter().UnicodeToAnsi( v.c_str() );
	}

	template<>
	AString value_tostringA( const int& v )
	{ 
		char buff[typeconv_buffersize];
		::_itoa_s(v, buff, typeconv_buffersize, 10 );
		return buff;
	}

	template<>
	AString value_tostringA( const Point& v )
	{ 
		char buff[typeconv_buffersize];
		::sprintf(buff,"%d,%d", v.x, v.y);
		return buff;
	}

	template<>
	AString value_tostringA( const Rect& v )
	{ 
		char buff[typeconv_buffersize];
		::sprintf(buff,"%d,%d,%d,%d", v.left, v.top, v.right, v.bottom );
		return buff;
	}

	template<>
	AString value_tostringA( const float& v )
	{ 
		char buff[typeconv_buffersize];
		::sprintf(buff,"%f", v );
		return buff;
	}

	template<>
	AString value_tostringA( const Vector2& v )
	{ 
		char buff[typeconv_buffersize];
		::sprintf(buff,"%f,%f", v.x, v.y );
		return buff;
	}

	template<>
	AString value_tostringA( const Vector3& v )
	{ 
		char buff[typeconv_buffersize];
		::sprintf(buff,"%f,%f,%f", v.x, v.y, v.z );
		return buff;
	}

	template<>
	AString value_tostringA( const Vector4& v )
	{ 
		char buff[typeconv_buffersize];
		::sprintf(buff,"%f,%f,%f,%f", v.x, v.y, v.z, v.w );
		return buff;
	}

	template<>
	AString value_tostringA( const Matrix4& v )
	{ 
		char buff[typeconv_buffersize];
		::sprintf(buff,
			"%f,%f,%f,%f\n"
			"%f,%f,%f,%f\n"
			"%f,%f,%f,%f\n"
			"%f,%f,%f,%f\n",
			v.m11, v.m12, v.m13, v.m14,
			v.m21, v.m22, v.m23, v.m24,
			v.m31, v.m32, v.m33, v.m34,
			v.m41, v.m42, v.m43, v.m44);
		return buff;
	}

	template<>
	AString value_tostringA( const Color& v )
	{ 
		dword hexcolor = v.ToDWORD();
		char buff[typeconv_buffersize];
		::sprintf(buff,"0x%x", hexcolor );
		return buff;
	}


	template<>
	bool value_parse(const AString& str, bool& v )
	{
		if ( str == "true" )
		{
			v = true;
			return true;
		}
		else if ( str == "false" )
		{
			v = false;
			return true;
		}

		return false;
	}

	template<>
	bool value_parse(const AString& str, dword& v )
	{
		v = ::strtoul( str.c_str(), null, 10 );

		return true;
	}

	template<>
	bool value_parse(const AString& str, AString& v )
	{
		v = str;

		return true;
	}

	template<>
	bool value_parse(const AString& str, WString& v )
	{
		v = StringConverter().AnsiToUnicode(str.c_str());

		return true;
	}

	template<>
	bool value_parse(const AString& str, int& v )
	{
		v = ::atoi( str.c_str() );

		return true;
	}

	template<>
	bool value_parse(const AString& str, Point& v )
	{
		return ::sscanf( str.c_str(), "%d,%d", &v.x, &v.y ) == 2;
	}

	template<>
	bool value_parse(const AString& str, Rect& v )
	{
		return ::sscanf( str.c_str(), "%d,%d,%d,%d", &v.left, &v.top, &v.right, &v.bottom  ) == 4;
	}

	template<>
	bool value_parse(const AString& str, float& v )
	{
		v = (float)::atof( str.c_str() );

		return true;
	}

	template<>
	bool value_parse(const AString& str, Vector2& v )
	{
		return ::sscanf( str.c_str(),"%f,%f", &v.x, &v.y  ) == 2;
	}

	template<>
	bool value_parse(const AString& str, Vector3& v )
	{
		return ::sscanf( str.c_str(),"%f,%f,%f", &v.x, &v.y, &v.z  ) == 3;
	}

	template<>
	bool value_parse(const AString& str, Vector4& v )
	{
		return ::sscanf( str.c_str(),"%f,%f,%f,%f", &v.x, &v.y, &v.z, &v.w  ) == 4;
	}

	template<>
	bool value_parse(const AString& str, Matrix4& v )
	{
		return ::sscanf( str.c_str(),"%f,%f,%f,%f\n"
			"%f,%f,%f,%f\n"
			"%f,%f,%f,%f\n"
			"%f,%f,%f,%f\n",
			&v.m11, &v.m12, &v.m13, &v.m14,
			&v.m21, &v.m22, &v.m23, &v.m24,
			&v.m31, &v.m32, &v.m33, &v.m34,
			&v.m41, &v.m42, &v.m43, &v.m44  ) == 16;
	}

	template<>
	bool value_parse(const AString& str, Color& v )
	{
		const char* s = str.c_str();

		const char* parsetype = "%x";

		// ºÊ»›’˚–Œ
		if ( s[0] != '0' && (s[1] != 'x' || s[1] != 'X') )
			parsetype = "%d";

		dword hexcolor = -1;
		if ( ::sscanf( s , parsetype, &hexcolor) != 1 )
			return false;

		v = hexcolor;

		return true;
	}



	template<>	bool		value_defaultA( ){ return false; }
	template<>	dword		value_defaultA( ){ return 0; }
	template<>	AString	value_defaultA( ){ return ""; }
	template<>	int		value_defaultA( ){ return 0; }
	template<>	Point		value_defaultA( ){ return Point::cZero; }
	template<>	Rect		value_defaultA( ){ return Rect::cNullRect; }
	template<>	float		value_defaultA( ){ return 0.0f; }
	template<>	Vector2	value_defaultA( ){ return Vector2::cZero; }
	template<>	Vector3	value_defaultA( ){ return Vector3::cZero; }
	template<>	Vector4	value_defaultA( ){ return Vector4::cZero; }
	template<>	Matrix4	value_defaultA( ){ return Matrix4::cZero; }
	template<>	Color		value_defaultA( ){ return Color::cWhite; }
}