#include "CorePCH.h"
#include "NekoCore_Reflection.h"

namespace NekoEngine
{
	static const dword typeconv_buffersize = 512;

	
	WString value_tostringW( const bool& v )
	{ 
		return (v ? L"true":L"false" );
	}

	
	WString value_tostringW( const dword& v )
	{ 
		wchar buff[typeconv_buffersize];
		::_ultow(v, buff, 10 ); 
		return buff;
	}

	
	WString value_tostringW( const word& v )
	{ 
		wchar buff[typeconv_buffersize];
		::_ultow(v, buff, 10 ); 
		return buff;
	}

	
	WString value_tostringW( const WString& v )
	{ 
		return v;
	}

	
	WString value_tostringW( const int& v )
	{ 
		wchar buff[typeconv_buffersize];
		::_itow_s(v, buff, typeconv_buffersize, 10 );
		return buff;
	}

	
	WString value_tostringW( const Point& v )
	{ 
		wchar buff[typeconv_buffersize];
		::swprintf(buff, typeconv_buffersize,L"%d,%d", v.x, v.y);
		return buff;
	}

	
	WString value_tostringW( const Rect& v )
	{ 
		wchar buff[typeconv_buffersize];
		::swprintf(buff, typeconv_buffersize,L"%d,%d,%d,%d", v.left, v.top, v.right, v.bottom );
		return buff;
	}

	
	WString value_tostringW( const float& v )
	{ 
		wchar buff[typeconv_buffersize];
		::swprintf(buff, typeconv_buffersize,L"%f", v );
		return buff;
	}

	
	WString value_tostringW( const Vector2& v )
	{ 
		wchar buff[typeconv_buffersize];
		::swprintf(buff, typeconv_buffersize,L"%f,%f", v.x, v.y );
		return buff;
	}

	
	WString value_tostringW( const Vector3& v )
	{ 
		wchar buff[typeconv_buffersize];
		::swprintf(buff, typeconv_buffersize,L"%f,%f,%f", v.x, v.y, v.z );
		return buff;
	}

	
	WString value_tostringW( const Vector4& v )
	{ 
		wchar buff[typeconv_buffersize];
		::swprintf(buff, typeconv_buffersize,L"%f,%f,%f,%f", v.x, v.y, v.z, v.w );
		return buff;
	}

	
	WString value_tostringW( const Matrix4& v )
	{ 
		wchar buff[typeconv_buffersize];
		::swprintf(buff, typeconv_buffersize,
			L"%f,%f,%f,%f\n"
			L"%f,%f,%f,%f\n"
			L"%f,%f,%f,%f\n"
			L"%f,%f,%f,%f\n",
			v.m11, v.m12, v.m13, v.m14,
			v.m21, v.m22, v.m23, v.m24,
			v.m31, v.m32, v.m33, v.m34,
			v.m41, v.m42, v.m43, v.m44);
		return buff;
	}

	
	WString value_tostringW( const Color& v )
	{ 
		wchar buff[typeconv_buffersize];
		::swprintf(buff, typeconv_buffersize,L"a=%f,r=%f,g=%f,b=%f", v.a, v.r, v.g, v.b );
		return buff;
	}

	WString value_tostringW( const Quaternion& v )
	{ 
		wchar buff[typeconv_buffersize];
		::swprintf(buff, typeconv_buffersize,L"x=%f,y=%f,z=%f,w=%f", v.x, v.y, v.z, v.w );
		return buff;
	}


	NekoEngine::WString value_tostringW( const wchar* v )
	{
		return v;
	}


	
	bool value_parse(const wchar* str, bool& v )
	{
		if ( StringHelper::CompareLowerCase(str, L"true") )
		{
			v = true;
			return true;
		}
		else if ( StringHelper::CompareLowerCase(str,L"false") )
		{
			v = false;
			return true;
		}

		return false;
	}

	
	bool value_parse(const wchar* str, dword& v )
	{
		v = ::wcstoul( str, null, 10 );

		return true;
	}

	
	bool value_parse(const wchar* str, word& v )
	{
		v = (word)::wcstoul( str, null, 10 );

		return true;
	}

	
	bool value_parse(const wchar* str, WString& v )
	{
		v = str;

		return true;
	}

	
	bool value_parse(const wchar* str, int& v )
	{
		v = ::_wtoi( str );

		return true;
	}

	
	bool value_parse(const wchar* str, Point& v )
	{
		return ::swscanf( str, L"%d,%d", &v.x, &v.y ) == 2;
	}

	
	bool value_parse(const wchar* str, Rect& v )
	{
		return ::swscanf( str, L"%d,%d,%d,%d", &v.left, &v.top, &v.right, &v.bottom  ) == 4;
	}

	
	bool value_parse(const wchar* str, float& v )
	{
		v = (float)::_wtof( str );

		return true;
	}

	
	bool value_parse(const wchar* str, Vector2& v )
	{
		return ::swscanf( str,L"%f,%f", &v.x, &v.y  ) == 2;
	}

	
	bool value_parse(const wchar* str, Vector3& v )
	{
		return ::swscanf( str,L"%f,%f,%f", &v.x, &v.y, &v.z  ) == 3;
	}

	
	bool value_parse(const wchar* str, Vector4& v )
	{
		return ::swscanf( str,L"%f,%f,%f,%f", &v.x, &v.y, &v.z, &v.w  ) == 4;
	}

	
	bool value_parse(const wchar* str, Matrix4& v )
	{
		return ::swscanf( str,L"%f,%f,%f,%f\n"
			L"%f,%f,%f,%f\n"
			L"%f,%f,%f,%f\n"
			L"%f,%f,%f,%f\n",
			&v.m11, &v.m12, &v.m13, &v.m14,
			&v.m21, &v.m22, &v.m23, &v.m24,
			&v.m31, &v.m32, &v.m33, &v.m34,
			&v.m41, &v.m42, &v.m43, &v.m44  ) == 16;
	}

	
	bool value_parse(const wchar* str, Color& v )
	{
		return ::swscanf( str,L"a=%f,r=%f,g=%f,b=%f", &v.a, &v.r, &v.g, &v.b  ) == 4;
	}


	template<>	bool		value_default( ){ return false; }
	template<>	dword		value_default( ){ return 0; }
	template<>	word		value_default( ){ return 0; }
	template<>	WString	value_default( ){ return L""; }
	template<>	int		value_default( ){ return 0; }
	template<>	Point		value_default( ){ return Point::cZero; }
	template<>	Rect		value_default( ){ return Rect::cNullRect; }
	template<>	float		value_default( ){ return 0.0f; }
	template<>	Vector2	value_default( ){ return Vector2::cZero; }
	template<>	Vector3	value_default( ){ return Vector3::cZero; }
	template<>	Vector4	value_default( ){ return Vector4::cZero; }
	template<>	Matrix4	value_default( ){ return Matrix4::cZero; }
	template<>	Color		value_default( ){ return Color::cBlack; }
}