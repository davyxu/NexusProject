#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/


	class Matrix4;
	class Quaternion;

	/** 
		矢量3
	*/
	class Vector3
	{
	public:
		float x, y, z;

		/// 零
		static const Vector3 cZero;

		/// x=1
		static const Vector3 cXAxis;

		/// y=1
		static const Vector3 cYAxis;

		/// z=1
		static const Vector3 cZAxis;

		/// x,y,z=1
		static const Vector3 cOne;

		/// 构造
		Vector3( );

		/// 拷贝构造函数
		Vector3(const Vector3 &v);

		/// 3值初始化
		Vector3(float nx, float ny, float nz);

		/// 正矢量
		Vector3 operator + ( ) const;

		/// 负矢量
		Vector3 operator - ( ) const;

		/// 自加
		Vector3& operator += (const Vector3 &v);

		/// 自减
		Vector3& operator -= (const Vector3 &v);

		/// 缩放
		Vector3& operator *= ( float scalar );

		/// 自乘
		Vector3& operator *= ( const Vector3& vector );

		/// 缩放
		Vector3& operator /= ( float scalar );

		/// 自除
		Vector3& operator /= ( const Vector3& vector );

		/// 自乘矩阵
		Vector3& operator *= ( const Matrix4& matrix );

		/// 自乘四元数
		Vector3& operator *= ( const Quaternion& quaternion );

		/// 加矢量
		Vector3 operator + ( const Vector3& vector ) const;

		/// 减矢量
		Vector3 operator - ( const Vector3& vector ) const;

		/// 缩放
		Vector3 operator * ( float scalar ) const;

		/// 乘法,非点乘
		Vector3 operator * ( const Vector3& vector ) const;

		/// 缩放
		Vector3 operator / ( float scalar ) const;

		/// 除法
		Vector3 operator / ( const Vector3& vector ) const;

		/// 乘矩阵
		Vector3 operator * ( const Matrix4& matrix ) const;

		/// 乘四元数
		Vector3 operator * ( const Quaternion& quaternion ) const;

		/// 赋值
		Vector3 &operator = (const Vector3 &v);

		/// 等于判断
		bool operator == (const Vector3 &v) const;

		/// 不等判断
		bool operator != (const Vector3 &v) const;

		float operator | ( const Vector3& v) const;

		bool Equal( const Vector3& vector, float margin = Math::cEpsilon ) const;

		/// 单位化
		Vector3& Normalize( );

		/// 求模
		float Magnitude( ) const;

		/// 未开方模
		float MagnitudeSquared( ) const;

		/// 取3轴最大数
		float GetMax( ) const;

		/// 取3轴最小数
		float GetMin( ) const;

		/// 设外值最大数
		Vector3& MakeMax(const Vector3& p);

		/// 设外值最小数
		Vector3& MakeMin(const Vector3& p);

		Vector3& TransformCoord(const Vector3& v, const Matrix4 &mat);

		Vector3 Perpendicular(void) const;

		Vector3 RandomDeviant(float angle,const Vector3& up = Vector3::cZero ) const;

		/// 点乘
		static float DotProduct(const Vector3& v1, const Vector3& v2);

		/// 叉乘
		static Vector3 CrossProduct(const Vector3& v1, const Vector3& v2);

		/// 2点间距离
		static float DistanceTo(const Vector3& v1, const Vector3& v2);

		/// 2点间平方距离
		static float DistanceToSqare(const Vector3& v1, const Vector3& v2);

		/// 线性插值
		static Vector3 Lerp(const Vector3& v1,const Vector3& v2,float s);

		/// 计算3点确定的切线
		static void BuildTangentSpaceVector(	   const Vector3& position1
												,  const Vector3& position2
												,  const Vector3& position3
												,	float u1
												,  float v1
												,  float u2
												,  float v2
												,  float u3
												,  float v3
												,  Vector3& OutTangent
												,  Vector3& OutBinormal );

		/// 投影
		Vector3 Project( const Vector3& vector1, const Vector3& vector2 );		
	};

	/** @} */
	/** @} */
}


