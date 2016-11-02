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
		四元数
	*/
	class Quaternion
	{
	public:
		float	x,y,z,w;

		/// 单元四元数
		const static Quaternion cIdentity;

		/// 零四元数
		const static Quaternion cZero;

		/// 构造
		inline Quaternion( );

		/// 赋值构造
		inline Quaternion( float xx, float yy, float zz, float ww );

		/// 加
		inline Quaternion operator + ( ) const;

		/// 减
		inline Quaternion operator - ( ) const;

		/// 自乘
		Quaternion& operator *= ( const Quaternion& quaternion );

		/// 自除
		Quaternion& operator /= ( const Quaternion& quaternion );

		/// 乘
		Quaternion operator * ( const Quaternion& quaternion ) const;

		/// 乘
		Vector3 operator* (const Vector3& v) const;

		/// 除
		Quaternion operator / ( const Quaternion& quaternion ) const;

		/// 点乘
		float operator | ( const Quaternion& v ) const;

		/// 相等
		bool operator == ( const Quaternion& quaternion) const;

		/// 单位化（初始化）
		Quaternion& Identity( );

		/// 单位化
		Quaternion& Normalize( );

		/// 取反
		Quaternion Inverse () const;

		/// 模
		float Magnitude( ) const;

		/// 不开方模
		float MagnitudeSquared( ) const;

		Quaternion& FromMatrix( const Matrix4& m );

		/// 绕X轴转
		Quaternion& RotationX( float radian );
		
		/// 绕Y轴转
		Quaternion& RotationY( float radian );

		/// 绕Z轴转
		Quaternion& RotationZ( float radian );
		
		/// 绕轴转
		Quaternion& RotationAxis( const Vector3& axis, float radian );
		
		/// 求一个四元数使得一个矢量乘以它等于另一个矢量
		Quaternion& Rotation( const Vector3& srcvector, const Vector3& desvector );

		/// 取出旋转轴和旋转角
		void GetAxisAndAngle( Vector3& axis, float& radian ) const;

		/// 获取本地空间的X轴分量
		Vector3 GetXAxis( ) const;

		/// 获取本地空间的Y轴分量
		Vector3 GetYAxis( ) const;

		/// 获取本地空间的Z轴分量
		Vector3 GetZAxis( ) const;

		/// 3轴构造
		Quaternion& FromAxes (const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis);

		/// 求点积
		static float DotProduct( const Quaternion& quaternion1, const Quaternion& quaternion2 );

		/// 平滑曲线插值
		static Quaternion Slerp( const Quaternion& quaternion1, const Quaternion& quaternion2, float factor );
		
	};

	//----------------------------------------------------------------------------
	// Quaternion Implementation
	//----------------------------------------------------------------------------

	Quaternion::Quaternion( )
	{
	}

	Quaternion::Quaternion( float xx, float yy, float zz, float ww )
		: x( xx ), y( yy ), z( zz ), w( ww )
	{
	}

	Quaternion Quaternion::operator + ( ) const
	{
		return Quaternion( x, y, z, w );
	}

	Quaternion Quaternion::operator - ( ) const
	{
		return Quaternion( -x, -y, -z, w );
	}

	/** @} */
	/** @} */
}
