#include "CorePCH.h"

#include <limits>
#include <stdlib.h>

#include "NekoCore_Math.h"

#include <ObjBase.h>

namespace NekoEngine
{
	const float Math::cPi		=  3.141592654f;
	const float Math::cHalfPi	=  Math::cPi / 2.0f;
	const float Math::c2Pi		=  6.283185308f;
	const float Math::cDeg2Rad = cPi / float(180.0);
	const float Math::cRad2Deg = float(180.0) / cPi;
	const float Math::cE			=  2.718281828f;
	const float Math::cMaxFloat	=  3.402823466e+38f;
	const float Math::cMinFloat	= -3.402823466e+38f;
	const float Math::cEpsilon	=  0.00001f;
	const dword Math::cMaxDowrd = 0xffffffffU;
	const dword Math::cMaxInt = 0xffffffffU >>1;



	static dword globalseed = rand();

	void Math::RandomSeed( dword seed )
	{

		globalseed = seed;
	}

	dword Math::Random( )
	{
		return ( ( globalseed = globalseed * 214013L + 2531011L ) >> 16 ) & 0x7fff;
	}

	dword Math::Random( dword maxvalue )
	{
		if ( maxvalue == 0 )
			return 0;

		return (dword) ( ( Random( ) / 32767.0f ) * (float) ( maxvalue - 1 ) + 0.5f );
	}

	dword Math::RandomRange( dword minvalue, dword maxvalue )
	{
		if ( maxvalue > minvalue )
			return Random( maxvalue - minvalue + 1 ) + minvalue;
		else if ( maxvalue < minvalue )
			return Random( minvalue - maxvalue + 1 ) + maxvalue;
		else
			return minvalue;
	}

	float Math::RandomFloat( float minvalue, float maxvalue )
	{
		if ( maxvalue > minvalue )
			return Random( 10001 ) * 0.0001f * ( maxvalue - minvalue ) + minvalue;
		else if ( maxvalue < minvalue )
			return Random( 10001 ) * 0.0001f * ( minvalue - maxvalue ) + maxvalue;
		else
			return minvalue;
	}

	long Math::RandomSign( )
	{
		return Random( 2 ) == 0 ? -1 : 1;
	}

	//-----------------------------------------------------------------------
	float Math::RandomSymmetric ()
	{
		return 2.0f * RandomFloat( 0.0f, 1.0f ) - 1.0f;
	}



	float Math::ToDegree(float radian)
	{
		return radian * cRad2Deg;
	}
	
	float Math::ToRadian(float degree)
	{
		return degree * cDeg2Rad;
	}

#pragma warning( push )
#pragma warning( disable: 4035 ) 


	float Math::ArcCos( float r )
	{
		// return half_pi + arctan( r / -sqr( 1.f - r * r ) );

		float asm_one = 1.f;
		float asm_half_pi = cPi * 0.5f;
		__asm {
			fld r // r0 = r
				fld r // r1 = r0, r0 = r
				fmul r // r0 = r0 * r
				fsubr asm_one // r0 = r0 - 1.f
				fsqrt // r0 = sqrtf( r0 )
				fchs // r0 = - r0
				fdiv // r0 = r1 / r0
				fld1 // {{ r0 = atan( r0 )
				fpatan // }}
				fadd asm_half_pi // r0 = r0 + pi / 2
		} // returns r0

	}

	float Math::ArcSin( float r ) 
	{
		// return arctan( r / sqr( 1.f - r * r ) );

		const float asm_one = 1.f;
		__asm {
			fld r // r0 = r
				fld r // r1 = r0, r0 = r
				fmul r // r0 = r0 * r
				fsubr asm_one // r0 = r0 - 1.f
				fsqrt // r0 = sqrtf( r0 )
				fdiv // r0 = r1 / r0
				fld1 // {{ r0 = atan( r0 )
				fpatan // }}
		} // returns r0
	}

	float Math::ArcTan( float r )
	{
		__asm {
			fld r // r0 = r
				fld1 // {{ r0 = atan( r0 )
				fpatan // }}
		} // returns r0

	}

	float Math::ArcTan2( float y, float x )
	{
		return ::atan2f( y, x );
	}

	float Math::Sin( float r ) 
	{
		__asm {
			fld r // r0 = r
				fsin // r0 = sinf( r0 )
		} // returns r0
	}

	float Math::Cos( float r )
	{
		__asm {
			fld r // r0 = r
				fcos // r0 = cosf( r0 )
		} // returns r0
	}

	float Math::Tan( float r )
	{
		// return sin( r ) / cos( r );
		__asm {
			fld r // r0 = r
				fsin // r0 = sinf( r0 )
				fld r // r1 = r0, r0 = r
				fcos // r0 = cosf( r0 )
				fdiv // r0 = r1 / r0
		} // returns r0
	}

	float Math::Cot( float r )
	{
		// return sin( r ) / cos( r );
		__asm {
			fld r // r0 = r
				fcos // r0 = sinf( r0 )
				fld r // r1 = r0, r0 = r
				fsin // r0 = cosf( r0 )
				fdiv // r0 = r1 / r0
		} // returns r0
	}

	// returns a for a * a = r
	float Math::Sqrt( float r )
	{
		__asm {
			fld r // r0 = r
				fsqrt // r0 = sqrtf( r0 )
		} // returns r0
	}

	// returns 1 / a for a * a = r
	// -- Use this for Vector normalisation!!!
	float Math::Rsq( float r )
	{
		__asm {
			fld1 // r0 = 1.f
				fld r // r1 = r0, r0 = r
				fsqrt // r0 = sqrtf( r0 )
				fdiv // r0 = r1 / r0
		} // returns r0
	}

	// returns 1 / a for a * a = r
	// Another version
	float Math::Apx_rsq( float r )
	{
		const float asm_dot5 = 0.5f;
		const float asm_1dot5 = 1.5f;

		__asm {
			fld r // r0 = r
				fmul asm_dot5 // r0 = r0 * .5f
				mov eax, r // eax = r
				shr eax, 0x1 // eax = eax >> 1
				neg eax // eax = -eax
				add eax, 0x5F400000 // eax = eax & MAGICAL NUMBER
				mov r, eax // r = eax
				fmul r // r0 = r0 * r
				fmul r // r0 = r0 * r
				fsubr asm_1dot5 // r0 = 1.5f - r0
				fmul r // r0 = r0 * r
		} // returns r0
	}

	/* very MS-specific, commented out for now
	Finally the best InvSqrt implementation?
	Use for vector normalisation instead of 1/length() * x,y,z
	*/

	float Math::InvSqrt(float Value)
	{
		return 1.0f /sqrtf( Value );
	}

	// returns a random number
	//inline_ float asm_rand()
	//{
	//	static unsigned __int64 q = time( 0 );

	//	_asm {
	//		movq mm0, q

	//			// do the magic MMX thing
	//			pshufw mm1, mm0, 0x1E
	//			paddd mm0, mm1

	//			// move to integer memory location and free MMX
	//			movq q, mm0
	//			emms
	//	}
	//}

	//// returns the maximum random number
	//inline_ float asm_rand_max()
	//{
	//	return 9223372036854775807.0f;
	//}

	// returns log2( r ) / log2( e )
	float Math::Ln( float r )
	{    
		const float asm_e = 2.71828182846f;
		const float asm_1_div_log2_e = .693147180559f;
		const float asm_neg1_div_3 = -.33333333333333333333333333333f;
		const float asm_neg2_div_3 = -.66666666666666666666666666667f;
		const float asm_2 = 2.f;

		int log_2 = 0;

		__asm {
			// log_2 = ( ( r >> 0x17 ) & 0xFF ) - 0x80;
			mov eax, r
				sar eax, 0x17
				and eax, 0xFF
				sub eax, 0x80
				mov log_2, eax

				// r = ( r & 0x807fffff ) + 0x3f800000;
				mov ebx, r
				and ebx, 0x807FFFFF
				add ebx, 0x3F800000
				mov r, ebx

				// r = ( asm_neg1_div_3 * r + asm_2 ) * r + asm_neg2_div_3;   // (1)
				fld r
				fmul asm_neg1_div_3
				fadd asm_2
				fmul r
				fadd asm_neg2_div_3
				fild log_2
				fadd
				fmul asm_1_div_log2_e
		}


	}

	int Math::Round( float Number )
	{
		__asm cvtss2si eax,[Number]
	}

	int Math::Floor( float Number )
	{
		const DWORD mxcsr_floor = 0x00003f80;
		const DWORD mxcsr_default = 0x00001f80;

		__asm ldmxcsr [mxcsr_floor]		// Round toward -infinity.
		__asm cvtss2si eax,[Number]
		__asm ldmxcsr [mxcsr_default]	// Round to nearest
	}


#pragma warning( pop )


	float Math::Fmod(float x,float y)
	{
		return ::fmodf( x, y );
	}

	float Math::Pow( float x, float y )
	{
		return ::pow( x, y );
	}

	dword Math::Pow( dword x, dword y )
	{
		dword ret = 1;
		for (dword i = 1;i< y;i++)
			ret *= x;

		return ret;
	}

	dword Math::NearToPower2( dword target )
	{
		dword size = 1;

		for(;size <= target;size <<= 1);
	
		return size;
	}

	static dword GLogs[257];
	bool GLogInitialized = false;


	dword Math::CeilLogTwo( dword Arg )
	{
		if( --Arg == cMaxDowrd )
		{
			return 0;
		}

		if (!GLogInitialized)
		{
			for( INT i=0,e=-1,c=0; i<=256; i++ )
			{
				GLogs[i] = e+1;
				if( !i || ++c>=(1<<e) )
				{
					c=0, e++;
				}
			}

			GLogInitialized = true;
		}


		dword Shift = Arg<=0x10000 ? (Arg<=0x100?0:8) : (Arg<=0x1000000?16:24);
		return Shift + GLogs[Arg>>Shift];
	}

	bool Math::Equal( float A, float B, float Magin /*= Math::cEpsilon */ )
	{
		return Math::Abs( A - B ) < Magin;
	}



	int Math::Ceil(float Number)
	{
		return int(ceilf(Number));
	}

	NekoEngine::Guid Guid::Create()
	{
		Guid ID;
		::CoCreateGuid( (GUID*)&ID );

		return ID;
	}

	Guid::Guid() 
		: A( 0 )
		, B( 0 )
		, C( 0 )
		, D( 0 )
	{

	}

	Guid::Guid( dword InA, dword InB, dword InC, dword InD ) 
		: A(InA), B(InB), C(InC), D(InD)
	{

	}

	bool Guid::IsValid() const
	{
		return (A | B | C | D) != 0;
	}

	bool operator==( const Guid& X, const Guid& Y )
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
	}

	bool operator!=( const Guid& X, const Guid& Y )
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
	}

	const Guid Guid::cNull = Guid( 0, 0, 0, 0 );

	float MathTable::mTrigFLOAT[];
	bool MathTable::mInitialized = false;

	void MathTable::StaticInit()
	{
		// Init base angle table.
		for( int i=0; i<NUM_ANGLES; i++ )
			mTrigFLOAT[i] = Math::Sin((float)i * 2.f * Math::cPi / (float)NUM_ANGLES);

		mInitialized = true;
	}

	float MathTable::SinTab( int i )
	{
		Assert( mInitialized );

		return mTrigFLOAT[((i>>ANGLE_SHIFT)&(NUM_ANGLES-1))];
	}

	float MathTable::CosTab( int i ) 
	{
		Assert( mInitialized );

		return mTrigFLOAT[(((i+16384)>>ANGLE_SHIFT)&(NUM_ANGLES-1))];
	}

	float MathTable::SinFloat( float F ) 
	{
		Assert( mInitialized );

		return SinTab(int((F*65536.f)/(2.f*Math::cPi)));
	}

	float MathTable::CosFloat( float F ) 
	{
		Assert( mInitialized );

		return CosTab(int((F*65536.f)/(2.f*Math::cPi)));
	}

	int MathTable::ReduceAngle( int Angle )
	{
		return Angle & ANGLE_MASK;
	}

	
}

