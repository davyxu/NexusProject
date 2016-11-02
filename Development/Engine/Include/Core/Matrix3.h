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
		3x3矩阵
	*/
	class Matrix3
	{
	public:
		friend class Matrix4;
		union 
		{
			struct
			{
				float m11, m12, m13;
				float m21, m22, m23;
				float m31, m32, m33;
			};
			float m[3][3];
		};
		
		/// 零矩阵
		const static Matrix3 cZero;
		
		/// 单位阵
		const static Matrix3 cIdentity;

		/// 默认构造
		inline Matrix3( );

		/// 9值构造
		inline Matrix3( float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22 );

		/// 取矩阵元素
		inline float operator ( ) ( dword row, dword column ) const;
		
		/// 取矩阵元素 引用
		inline float& operator ( ) ( dword row, dword column );

		/// 负
		inline Matrix3 operator - ( ) const;

		/// 取行
		inline Vector3 GetRow( dword row ) const;

		/// 取列
		inline Vector3 GetColumn( dword column ) const;

		/// 自加
		Matrix3& operator += ( const Matrix3& matrix );

		/// 自减
		Matrix3& operator -= ( const Matrix3& matrix );

		/// 自乘
		Matrix3& operator *= ( const Matrix3& matrix );

		/// 乘法因子
		Matrix3& operator *= ( float scalar );

		/// 除法因子
		Matrix3& operator /= ( float scalar );

		/// 加矩阵
		Matrix3 operator + ( const Matrix3& matrix ) const;

		/// 减矩阵
		Matrix3 operator - ( const Matrix3& matrix ) const;

		/// 乘矩阵
		Matrix3 operator * ( const Matrix3& matrix ) const;

		/// 乘因子
		Matrix3 operator * ( float scalar ) const;

		/// matrix * vector [3x3 * 3x1 = 3x1]
		Vector3 operator* (const Vector3& rkPoint) const;

		/// 除因子
		Matrix3 operator / ( float scalar ) const;

		/// 单位化
		Matrix3 Identity( );

		/// 转置
		Matrix3 Transpose( );

		/// 求逆
		Matrix3 Inverse( );

		/// 缩放
		Matrix3& Scaling( float x, float y, float z );

		/// 缩放 矢量
		Matrix3& Scaling( const Vector3& vector );

		/// 缩放 因子
		Matrix3& Scaling( const Vector3& vector, float factor );

		/// 绕X轴旋转
		Matrix3& RotationX( float radian );

		/// 绕Y轴旋转
		Matrix3& RotationY( float radian );

		/// 绕Z轴旋转
		Matrix3& RotationZ( float radian );

		/// 绕轴旋转
		Matrix3& RotationAxis( const Vector3& axis, float radian );

		/// 投射到XY平面的矩阵
		Matrix3& ProjectXY( );

		/// 投射到YZ平面的矩阵
		Matrix3& ProjectYZ( );

		/// 投射到ZX平面的矩阵
		Matrix3& ProjectZX( );

		/// 投射矢量到任意平面
		Matrix3& Project( const Vector3& vector );

		/// 构建一个反射矩阵,以XY平面为镜像面
		Matrix3& RefectionXY( );

		/// 构建一个反射矩阵,以YZ平面为镜像面
		Matrix3& RefectionYZ( );

		/// 构建一个反射矩阵,以ZX平面为镜像面
		Matrix3& RefectionZX( );

		/// 构建一个反射矩阵,投射矢量到任意平面
		Matrix3& Refection( const Vector3& vector );

		/** 
			构建2D缩放矩阵
		*/
		Matrix3& Scaling2D( float x, float y );

		/** 
			构建2D缩放矩阵
		*/
		Matrix3& Scaling2D( const Vector2& vector );

		/** 
			构建2D缩放矩阵
		*/
		Matrix3& Translation2D( float x, float y);

		/** 
			构建2D偏移矩阵
		*/
		Matrix3& Translation2D( const Vector2& vector );

		/// 设置2D矩阵中平移部分
		Matrix3& SetTranslation2D( float x, float y );

		/// 设置2D矩阵中平移部分
		Matrix3& SetTranslation2D( const Vector2& vector );

		/// 偏移2D矩阵
		Matrix3& AddTranslation2D( const Vector2& vector );

		/// 四元数转矩阵
		Matrix3& FromQuaternion(const Quaternion& quat);
	};

	//----------------------------------------------------------------------------
	// nMatrix3 Implementation
	//----------------------------------------------------------------------------

	Matrix3::Matrix3( )
	{
	}

	Matrix3::Matrix3( float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22 )
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
	}

	float Matrix3::operator ( ) ( dword row, dword column ) const
	{
		Assert( row < 3 && column < 3 );

			return m[ row ][ column ];
	}

	float& Matrix3::operator ( ) ( dword row, dword column )
	{
		Assert( row < 3 && column < 3 );

			return m[ row ][ column ];
	}


	Matrix3 Matrix3::operator - ( ) const
	{
		return Matrix3( -m[0][0], -m[0][1], -m[0][2], -m[1][0], -m[1][1], -m[1][2], -m[2][0], -m[2][1], -m[2][2] );
	}

	Vector3 Matrix3::GetRow( dword row ) const
	{
		Assert( row < 3 );

			return Vector3( m[ row ][0], m[ row ][1], m[ row ][2] );
	}

	Vector3 Matrix3::GetColumn( dword column ) const
	{
		Assert( column < 3 );

			return Vector3( m[0][ column ], m[1][ column ], m[2][ column ] );
	}


	/** @} */
	/** @} */
}

