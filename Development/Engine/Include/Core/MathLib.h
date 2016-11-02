#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/

	/** 
		基础数学库封装
	*/
	class Math
	{
	public:
		/** 
			弧度转角度
			@param
				radian 弧度
			@return
				角度
		*/
		static float ToDegree( float radian );

		/// 计算反余弦
		static float ArcCos( float r );

		/// 计算反正弦
		static float ArcSin( float r );

		/// 计算反正切
		static float ArcTan( float r );

		/// 计算反正切双参数
		static float ArcTan2( float y, float x );

		/// 计算正闲
		static float Sin( float r );

		/// 计算余弦
		static float Cos( float r );

		/// 计算正切
		static float Tan( float r );

		/// 计算余切
		static float Cot( float r );

		/// 角度转弧度
		static float ToRadian( float degree );

		/// 计算平方根
		static float Sqrt( float r );

		/// 计算平方根倒数
		static float InvSqrt(float fValue);

		/// 计算平方根倒数
		static float Rsq( float r );

		/// 计算平方根倒数
		static float Apx_rsq( float r );

		/// 以10为底取Log
		static float Ln(float r);

		/// 计算余数
		static float Fmod(float x,float y);	

		/// 计算指数
		static float Pow( float x, float y );

		/// 计算指数
		static dword Pow( dword x, dword y );

		/// 设置随机数种子
		static void RandomSeed( dword seed );

		/// 伪随机数
		static dword Random( );

		/// 随机数，范围[0,maxvalue]
		static dword Random( dword maxvalue );

		/// 随机数，范围[minvalue,maxvalue]
		static dword RandomRange( dword minvalue, dword maxvalue );

		/// 随机数，范围[minvalue,maxvalue]
		static float RandomFloat( float minvalue, float maxvalue );

		/// 符号随机数 -1或1
		static long RandomSign( );

		/// 对称随机数 [-1,1]
		static float RandomSymmetric();

		/// 得到target的最小2次幂
		static dword NearToPower2( dword target );

		/// result = log2(arg) arg的2的幂
		static dword CeilLogTwo( dword Arg );

		/** 
			判断两个数是否相等
			@param
				Magin 判定精度
		*/
		static bool Equal( float A, float B, float Magin = Math::cEpsilon );

		/** 
			将值舍入到最接近的整数或指定的小数位数。
		*/
		static int Round( float Number );

		/// 取浮点数的整数部分
		static int Floor( float Number );

		/// 取浮点数的小数部分
		static int Ceil(float Number);

		/// Pi = 3.141592654
		static const float cPi;

		/// 2 Pi = 6.283185308
		static const float c2Pi;

		/// 0.5 Pi = 1.570796327
		static const float cHalfPi;

		/// Pi / 180
		static const float cDeg2Rad;

		/// 180 / Pi
		static const float cRad2Deg;

		/// 自然对数底数 2.718281828
		static const float cE;

		/// 最大浮点数 3.402823466e+38
		static const float cMaxFloat;

		/// 最小浮点数 -3.402823466e+38
		static const float cMinFloat;

		/// 最小精度 0.00001
		static const float cEpsilon;

		/// 最大无符号整型 4294967295
		static const dword cMaxDowrd;

		/// 最大有符号整型 2147483647
		static const dword cMaxInt;

		/// 取绝对值
		template< class T > static T Abs( const T A )
		{
			return (A>=(T)0) ? A : -A;
		}

		/** 
			取整数符号
			@param
				A 数
			@return
				正数为1，负数为-1，0为0
		*/
		static int Sign( int A )
		{
			return (A>0) ? 0 : ((A<0) ? -1 : 0);
		}

		/** 
			取浮点数符号
			@param
				A 数
			@return
				正数为1，负数为-1，0为0
		*/
		static float Sign( const float A )
		{
			return ( A>0.0f ) ? 1.0f : ((A<0.0f) ? -1.0f : 0.0f );
		}

		/// 取A,B中最大者返回
		template< class T > static T Max( const T A, const T B )
		{
			return (A>=B) ? A : B;
		}

		/// 取A,B中最小者返回
		template< class T > static T Min( const T A, const T B )
		{
			return (A<=B) ? A : B;
		}

		/// 取A,B,c中最大者返回
		template< class T > static T Max3( const T A, const T B, const T C )
		{
			return Max ( Math::Max( A, B ), C );
		}

		/// 取A,B,C中最小者返回
		template< class T > static T Min3( const T A, const T B, const T C )
		{
			return Min ( Math::Min( A, B ), C );
		}

		/// 计算A的平方
		template< class T > static T Square( const T A )
		{
			return A*A;
		}

		/// 返回X被限定在Min及Max之间的值
		template< class T > static T Clamp( const T X, const T Min, const T Max )
		{
			return X<Min ? Min : X<Max ? X : Max;
		}

		/// 返回被对齐的值
		template< class T > static T Align( const T Ptr, dword Alignment )
		{
			return (T)(((dword)Ptr + Alignment - 1) & ~(Alignment-1));
		}

		/// 交换两值
		template< class T > static void Swap( T& A, T& B )
		{
			const T Temp = A;
			A = B;
			B = Temp;
		}

		/// 线性插值
		template< class T > static T Lerp( T& A, T& B, float S )
		{
			return (T)(A + S * (B-A));
		}
		
	};

	
	class Guid
	{
	public:
		dword A,B,C,D;

		Guid();

		Guid( dword InA, dword InB, dword InC, dword InD );

		bool IsValid() const;

		friend bool operator==(const Guid& X, const Guid& Y);
		friend bool operator!=(const Guid& X, const Guid& Y);

		friend dword GetTypeHash(const Guid& guid)
		{
			return CheckSum::MemCRC(&guid,sizeof(Guid));
		}

		static Guid Create();

		static const Guid cNull;
	};


	class MathTable
	{
	public:
		// Constants.
		enum {ANGLE_SHIFT 	= 2};		// Bits to right-shift to get lookup value.
		enum {ANGLE_BITS	= 14};		// Number of valid bits in angles.
		enum {NUM_ANGLES 	= 16384}; 	// Number of angles that are in lookup table.
		enum {ANGLE_MASK    =  (((1<<ANGLE_BITS)-1)<<(16-ANGLE_BITS))};

		// Basic math functions.
		static float SinTab( int i ) ;
		static float CosTab( int i ) ;
		static float SinFloat( float F ) ;
		static float CosFloat( float F ) ;

		static void StaticInit( );

		static int ReduceAngle( int Angle );;

	private:
		// Tables.
		static float	mTrigFLOAT		[NUM_ANGLES];
		static bool		mInitialized;
	};

	/** @} */
	/** @} */
}
