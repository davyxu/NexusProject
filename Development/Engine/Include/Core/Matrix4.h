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
		4x4矩阵
	*/
	class Matrix4
	{
	private:

		friend class Matrix3;
		friend class Quaternion;
		union 
		{
			struct
			{
				float m11, m12, m13, m14;
				float m21, m22, m23, m24;
				float m31, m32, m33, m34;
				float m41, m42, m43, m44;
			};
			float m[4][4];
		};

	public:

		const static Matrix4 cZero;

		const static Matrix4 cIdentity;

		/// 默认构造
		inline Matrix4( );
		
		/// 16值构造
		inline Matrix4( float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33 );

		/// 取矩阵元素
		inline float operator ( ) ( dword row, dword column ) const;
		
		/// 取矩阵元素 引用
		inline float& operator ( ) ( dword row, dword column );
		
		/// 负
		inline Matrix4 operator - ( ) const;

		/// 取行
		inline Vector4 GetRow( dword row ) const;

		inline Vector3 GetAxis( dword row ) const;

		inline void SetRow( dword index, float x, float y, float z, float w);

		inline void SetAxis( dword index, const Vector3& v );
		
		/// 取列
		inline Vector4 GetColumn( dword column ) const;

		/// 布尔比较
		inline bool operator == ( const Matrix4& matrix);

		inline bool operator != ( const Matrix4& matrix);

		/// 自加
		Matrix4& operator += ( const Matrix4& matrix );
		
		/// 自减
		Matrix4& operator -= ( const Matrix4& matrix );
		
		/// 自乘
		Matrix4& operator *= ( const Matrix4& matrix );
		
		/// 乘法因子
		Matrix4& operator *= ( float scalar );
		
		/// 除法因子
		Matrix4& operator /= ( float scalar );

		/// 加矩阵
		Matrix4 operator + ( const Matrix4& matrix ) const;
		
		/// 减矩阵
		Matrix4 operator - ( const Matrix4& matrix ) const;
		
		/// 乘矩阵
		Matrix4 operator * ( const Matrix4& matrix ) const;
		
		/// 乘因子
		Matrix4 operator * ( float scalar ) const;
		
		/// 除因子
		Matrix4 operator / ( float scalar ) const;

		/// 从四元数构造
		Matrix4& FromQuaternion(const Quaternion& quat);

		/// 单位化
		Matrix4 Identity( ) const;
		
		/// 转置
		Matrix4 Transpose( ) const;
		
		/// 求逆
		Matrix4 Inverse( ) const;
	

		/// 位移
		Matrix4& Translation( float x, float y, float z );
		
		/// 位移 矢量
		Matrix4& Translation( const Vector3& vector );

		/// 只设置偏移
		Matrix4& SetTranslation( const Vector3& vector );

		/// 缩放
		Matrix4& Scale( float x, float y, float z );
		
		/// 缩放 矢量
		Matrix4& Scale( const Vector3& vector );
		
		/// 缩放 因子
		Matrix4& Scale( const Vector3& vector, float factor );

		/// 绕X轴旋转
		Matrix4& RotationX( float radian );
		
		/// 绕Y轴旋转
		Matrix4& RotationY( float radian );
		
		/// 绕Z轴旋转
		Matrix4& RotationZ( float radian );
		
		/// 绕轴旋转
		Matrix4& RotationAxis( const Vector3& axis, float radian );
		
		/// 构建一个矩阵让src旋转到dest
		Matrix4& Rotation( const Vector3& srcvector, const Vector3& desvector );
		
		/// 从四元数旋转
		Matrix4& Rotation( const Quaternion& rotation );

		
		/// 投射到XY平面的矩阵
		Matrix4& ProjectXY( );
		
		/// 投射到YZ平面的矩阵
		Matrix4& ProjectYZ( );
		
		/// 投射到ZX平面的矩阵
		Matrix4& ProjectZX( );
		
		/// 投射矢量到任意平面
		Matrix4& Project( const Vector3& vector );

		/// 构建一个反射矩阵,以XY平面为镜像面
		Matrix4& RefectionXY( );
		
		/// 构建一个反射矩阵,以YZ平面为镜像面
		Matrix4& RefectionYZ( );
		
		/// 构建一个反射矩阵,以ZX平面为镜像面
		Matrix4& RefectionZX( );
		
		/// 构建一个反射矩阵,投射矢量到任意平面
		Matrix4& Refection( const Vector3& vector );

		/// 用一个3X3矩阵和一个矢量创建
		Matrix4& FromTranslationRotation( const Vector3& translation, const Matrix3& rotation );
		
		/// 3个变换构造矩阵
		Matrix4& FromTransform( const Vector3& translation, const Quaternion& rotation, const Vector3& scaling );

		/// 不拷贝便宜
		Matrix4& FromMatrix3( const Matrix3& Mat );

		/// 取得3X3矩阵和位移因子
		void ToTranslationRotation( Vector3& translation, Matrix3& rotation ) const;

		/// 用一个四元数和一个矢量创建
		Matrix4& FromTranslationRotation( const Vector3& translation, const Quaternion& rotation );
		
		/// 取得四元数和位移因子
		void ToTranslationRotation( Vector3& translation, Quaternion& rotation ) const;

		Vector4 TransformNormal( const Vector3& v );

		Vector4 TransformCoord( const Vector4& v );

		/// 正交左手矩阵
		static Matrix4 OrthoLH( float width, float height, float znear, float zfar );

		/// 正交左手投射矩阵，与viewport对应的实际查看窗口矩阵 ( 2D使用时zear=0 zfar=1)
		static Matrix4 OrthoOffCenterLH( float left, float right, float bottom, float top, float znear, float zfar );

		/// 正交右手矩阵
		static Matrix4 OrthoRH( float width, float height, float znear, float zfar );

		/// 正交右手投射矩阵
		static Matrix4 OrthoOffCenterRH( float left, float right, float bottom, float top, float znear, float zfar );

		/// 透视左手投射矩阵
		static Matrix4 PerspectiveLH( float width, float height, float znear, float zfar );

		/// 透视左手投射矩阵,基于视锥模型
		static Matrix4 PerspectiveFovLH( float fovy, float aspect, float znear, float zfar );

		/// 透视左手投射矩阵
		static Matrix4 PerspectiveOffCenterLH( float left, float right, float bottom, float top, float znear, float zfar );

		/// 透视右手投射矩阵
		static Matrix4 PerspectiveRH( float width, float height, float znear, float zfar );

		/// 透视右手投射矩阵,基于视锥模型
		static Matrix4 PerspectiveFovRH( float fovy, float aspect, float znear, float zfar );

		/// 透视右手投射矩阵
		static Matrix4 PerspectiveOffCenterRH( float left, float right, float bottom, float top, float znear, float zfar );

		/// 左手看矩阵
		static Matrix4 LookAtLH( const Vector3& eye, const Vector3& lookat, const Vector3& upaxis );
		
		/// 右手看矩阵
		static Matrix4 LookAtRH( const Vector3& eye, const Vector3& lookat, const Vector3& upaxis );

		Matrix4& FromRotator( const Rotator& Rot );

		Rotator GetRotator() const;
	};

	//----------------------------------------------------------------------------
	// nMatrix4 Implementation
	//----------------------------------------------------------------------------

	Matrix4::Matrix4( )
	{
	}

	Matrix4::Matrix4( float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33 )
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

	float Matrix4::operator ( ) ( dword row, dword column ) const
	{
		Assert( row < 4 && column < 4 );

			return m[ row ][ column ];
	}

	float& Matrix4::operator ( ) ( dword row, dword column )
	{
		Assert( row < 4 && column < 4 );

			return m[ row ][ column ];
	}


	Matrix4 Matrix4::operator - ( ) const
	{
		return Matrix4( -m[0][0], -m[0][1], -m[0][2], -m[0][3], -m[1][0], -m[1][1], -m[1][2], -m[1][3],
			-m[2][0], -m[2][1], -m[2][2], -m[2][3], -m[3][0], -m[3][1], -m[3][2], -m[3][3] );
	}

	Vector4 Matrix4::GetRow( dword row ) const
	{
		Assert( row < 4 );

			return Vector4( m[ row ][0], m[ row ][1], m[ row ][2], m[ row ][3] );
	}

	Vector3 Matrix4::GetAxis( dword row ) const
	{
		Assert( row < 4 );

		return Vector3( m[ row ][0], m[ row ][1], m[ row ][2] );
	}

	void Matrix4::SetRow( dword index, float x, float y, float z, float w)
	{
		Assert( index < 4 );
		m[index][0] = x;
		m[index][1] = y;
		m[index][2] = z;
		m[index][3] = w;
	}

	void Matrix4::SetAxis( dword index, const Vector3& v )
	{
		Assert( index < 4 );
		m[index][0] = v.x;
		m[index][1] = v.y;
		m[index][2] = v.z;
	}


	Vector4 Matrix4::GetColumn( dword column ) const
	{
		Assert( column < 4 );

			return Vector4( m[0][ column ], m[1][ column ], m[2][ column ], m[3][ column ] );
	}


	/** @} */
	/** @} */
}

