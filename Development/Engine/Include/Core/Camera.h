#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/


#ifdef USE_R_HAND_COORDSYS
	const float CameraCoordFix = 1.0f;
	const Vector3 CameraStartPos = Vector3(10,50,50);
#else
	const Vector3 CameraStartPos = Vector3(10,-50,50);
	const float CameraCoordFix = -1.0f;
#endif

	class BinarySerializer;

	/** 
		摄像机包装类
		@remark
			通过宏切换左右手
	*/
	class Camera
	{
	public:
		Camera( );

		/** 
			设置 上 方向
			@param
				Value 向量
		*/
		void SetUp( const Vector3& Value );

		/** 
			设置 看点
			@param
				Value 3D空间点
		*/
		void SetLook( const Vector3& Value );

		/** 
			设置 摄像机位置
			@param
				Value 3D空间点
		*/
		void SetPosition( const Vector3& Value );

		/** 
			获取投射矩阵
			@return
				矩阵
		*/
		const Matrix4& GetProjectMatrix( ) const{ return mProjectMatrix; }

		/** 
			获取观察矩阵
			@return
				矩阵
		*/
		const Matrix4& GetViewMatrix( ) const;

		/** 
			获取上向量
			@return
				向量
		*/
		const Vector3& GetUp( ) const { return mUp; }

		/** 
			获取观察点
			@return
				3D空间点
		*/
		const Vector3& GetLook( ) const { return mLook; }

		/** 
			获取摄像机位置
			@return
				3D空间点
		*/
		const Vector3& GetPosition( ) const{ return mPosition;}

		/// 序列化
		virtual void Serialize( BinarySerializer& Ser );

		/** 
			获取摄像机位置朝向
			@remark
				ViewDir = Pos - Look
			@return
				3D空间点
		*/
		Vector3 GetViewDir( );

		/** 
			根据投射和观察矩阵构建视锥对象
			@return
				视锥
		*/
		Frustum CreateFrustum( )  const;

		/** 
			绕摄像机 X 轴转
			@param
				radian 弧度
		*/
		void Yaw(float radian);

		/** 
			绕摄像机 Y 轴转
			@param
				radian 弧度
		*/
		void Pitch(float radian);

		/** 
			绕摄像机 Z 轴转
			@param
				radian 弧度
		*/
		void Roll(float radian);

		/** 
			相对摄像机朝向的横向移动
			@param
				units 3D单位
		*/
		void Strafe(float units);

		/** 
			相对摄像机朝向的垂直移动
			@param
				units 3D单位
		*/
		void Fly(float units);

		/** 
			相对摄像机上方向的垂直移动
			@param
				units 3D单位
		*/
		void FlyStaticUp(float units);

		/** 
			距离不变，转换看点和位置	
			@param
				look 看点，3D空间点
		*/
		void LookAt( const Vector3& look );

		/** 
			相对于摄像机朝向的前后移动
			@param
				units 3D单位
		*/
		void Zoom(float units);

		/** 
			绕摄像机 上方向 转，左右翻滚
			@param
				radian 弧度
		*/
		void Phi( float radian );

		/** 
			绕摄像机 右方向 转，前后翻滚
			@param
				radian 弧度
		*/
		void Theta( float radian );

		/** 
			摄像机观察方向的半径收缩
			@param
				units 3D单位
		*/
		void Radius( float units );

		/** 
			更新摄像机矩阵
			@remark
				根据摄像机的上方向，观察方向等属性更新观察矩阵及投射矩阵
			@param
				vp 视口
		*/
		void Update( const Viewport& vp );

		/// 二进制序列化重载
		friend BinarySerializer& operator <<( NekoEngine::BinarySerializer& Ser, Camera& Data )
		{
			Data.Serialize( Ser );

			return Ser;
		}

	private:
		void UpdateViewMatrix( ) const;

	private:
		Vector3		mLook;
		Vector3		mPosition;
		Vector3		mUp;

		Matrix4		mProjectMatrix;

		mutable Matrix4	mViewMatrix;
		mutable bool	mViewMatrixDirty;		
	};

}
