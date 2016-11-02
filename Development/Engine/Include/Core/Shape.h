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
		光线
	*/
	class Ray
	{
	public:
		/// 构造函数
		Ray( );

		/** 
			构造函数
			@param
				origin 光线发射点
			@param
				direction 方向
		*/
		Ray( const Vector3& origin, const Vector3& direction );

		/** 
			构造函数
			@param
				x 屏幕坐标
			@param
				y 屏幕坐标
			@param
				viewtransform 观察矩阵
			@param
				projectiontransform 投射矩阵
		*/
		Ray( float x, float y, const Matrix4& viewtransform, const Matrix4& projectiontransform );

		/// 自我变换空间
		Ray& operator *= ( const Matrix4& matrix );

		/// 变换空间
		Ray  operator * ( const Matrix4& matrix ) const;

		/// 根据距离，确定光线上的一个点
		Vector3 DistanceVector( float distance ) const;

		/// 取光线方向
		const Vector3& GetDirection( ) const{ return mDirection; }

		/// 取光线位置
		const Vector3& GetOrigin( ) const { return mOrigin; }

	private:
		Vector3		mOrigin;
		Vector3		mDirection;

	};

	/** 
		轴对称绑定盒
	*/
	class AABB
	{
	public:
		/// 空盒子min = 0, max=0
		const static AABB cNullBox;

		/// 初始化的盒子 min=maxfloat, max = minfloat
		const static AABB cInitBox;

		/// 最大盒子 min=minfloat, max = maxfloat
		const static AABB cMaxBox;

		Vector3		mMinVector;
		Vector3		mMaxVector;

		/// 构造函数
		AABB( );


		/// 赋值构造函数
		AABB( const Vector3& minvector, const Vector3& maxvector );

		/// 添加盒子
		AABB& operator += ( const AABB& box );

		/// 添加一个点
		AABB& operator += ( const Vector3& vector );

		/// 变换空间
		AABB& operator *= ( const Matrix4& matrix );

		/// 相等
		bool operator == ( const AABB& box ) const;

		/// 不等
		bool operator != ( const AABB& box ) const;

		/// 添加盒子
		AABB operator + ( const AABB& box ) const;

		/// 添加点
		AABB operator + ( const Vector3& vector ) const;

		/// 变换矩阵
		AABB operator * ( const Matrix4& matrix ) const;

		/// 点是否在盒子中
		bool VectorInBox( const Vector3& vector ) const;

		/// 取中心
		Vector3 GetCenter( ) const;

		/// 取大小
		Vector3 GetSize( ) const;

		/// 取半大小
		Vector3 GetHalfSize( ) const;

	};

	
	/** 
		盒子
	*/
	class Box
	{
	public:

		/// 盒子枚举定义
		enum eBoxEnum
		{ 
			MaxPoints = 8,											///< 盒子拥有的最大点
			MaxPointConnectSide = 3,								///< 盒子中每点连接的边数
			MaxNoneSharedPoint = MaxPoints * MaxPointConnectSide,	///< 非共享点数=24
			MaxSides = MaxNoneSharedPoint /2,						///< 盒子的边数=12
		};

		Vector3		mCornerPoint[MaxPoints];

		/// 构造函数
		Box( );

		/// 拷贝构造函数
		Box( const AABB& box );

		/// 变换
		Box& operator *= ( const Matrix4& matrix );

		/// 变换
		Box operator * ( const Matrix4& matrix ) const;

		/// 获取绘制边的索引
		static dword GetSideIndex( dword Index );

		union NeighbourIndex
		{
			struct  
			{
				dword mNeighbourX;
				dword mNeighbourY;
				dword mNeighbourZ;
			};

			dword mNeighbourPoint[3];
		};

		/// 取得相邻点的索引
		static const NeighbourIndex& GetNeighbourIndex( dword Index );
	};

	/** 
		球
	*/
	class Sphere
	{
	public:
		Vector3		mCenter;
		float		mRadius;

		/// 构造函数
		Sphere( );

		/// 赋值构造函数
		Sphere( const Vector3& center, float radius );

		/// 变换
		Sphere& operator *= ( const Matrix4& matrix );

		/// 变换
		Sphere operator * ( const Matrix4& matrix ) const;
	};




	/** 
		三角形
	*/
	class Triangle
	{
	public:
		union
		{
			struct
			{
				Vector3 mPoint[3];
			};


			struct
			{
				Vector3 mP1,mP2,mP3;
			};
		};
		
		/// 构造函数
		Triangle( );

		/// 赋值构造
		Triangle(const Vector3& p1,const Vector3& p2,const Vector3& p3);

		/// 叉积
		static inline float CrossProduct3(const Vector3& p1,const Vector3& p2, const Vector3& p0 );

		/// 判断一点是否在三角形内
		bool Inside(float x,float y);


		/** 
			射线穿插三角形
			@remark
				根据重心坐标（U,V）,我们可以很容易的算出各种相关量比如纹理坐标和交点的差值颜色，假设以纹理坐标为例设V1,V2,V3的纹理坐标分别为T1(tu1,tv1),T2(tu2,tv2),T3(tu3,tv3)则交点的坐标为		
			@par
				IntersectPointTexture = T1 + U(T2-T1) + V(T3-T1)
			@param
				ray 光线
			@param
				distance 穿插到的距离
			@param
				tU 返回三角形上的坐标
			@param
				tV 返回三角形上的坐标
			@return
				是否穿插到

		*/
		bool Intersect( const Ray& ray,float& distance,float* tU = null,float* tV = null);

		/// 三角形面积
		float Area( );


		
	};

	/** 
		平面三角形
	*/
	class Triangle2D
	{
	public:
		union
		{
			struct
			{
				Vector2 mPoint[3];
			};


			struct
			{
				Vector2 mP1,mP2,mP3;
			};
		};

		/// 构造函数
		Triangle2D( );

		/// 赋值构造函数
		Triangle2D(const Vector2& p1,const Vector2& p2,const Vector2& p3);

		/// 判断一个点是否在三角形内
		bool Inside( const Vector2& p );

		/** 
			判断一个点是否在三角形内
			@remark
				来源:IndieLib
		*/
		bool Inside2( const Vector2& p );

		/// 计算面积
		float Area( );

		/// 叉乘
		static float CrossProduct3(const Vector2& p1,const Vector2& p2, const Vector2& p0 );
		
	};

	/** 
		平面线段
	*/
	class Segement2D
	{
	public:
		/// 判断4点构成的两条线段是否相交
		static bool Intersect( const Vector2& p1, const Vector2& p2,const Vector2& p3, const Vector2& p4 );

		/// 取得2条线段的交叉点
		static Vector2 GetCrossPoint(const Vector2& p1, const Vector2& p2, const Vector2& q1, const Vector2& q2);
	};

	/** 
		任意多边形

		顺序必须按照以下描述
			0   1
			-----
			|   |
			|   |
			-----
			3   2
	*/
	class Quadrangle
	{
	public:

		union
		{
			struct
			{
				Vector2 mPoint[4];
			};


			struct
			{
				Vector2 mP1,mP2,mP3,mP4;
			};
		};

		Quadrangle( );

		Quadrangle( Vector2* pointlist );

		Quadrangle( const Vector2& p1, const Vector2& p2, const Vector2& p3,const Vector2& p4 );

		/// 适用于任何凸4边形，使用线段相交判断，速度稍慢，100%准确
		bool Inside( const Vector2& p );
	};

	/** 
		平面
	*/
	class Plane
	{
	public:
		union
		{
			struct
			{
				Vector3 n;
				float d;
			};
			struct
			{
				float a, b, c, d;	/// ax + by + cz + d = 0
			};
		};

		Plane( ){}

		Plane(float InA,float InB,float InC,float InD) :a(InA),b(InB),c(InC),d(InD){}

		Plane( const Vector3& position, const Vector3& normal );

		Plane( const Vector3& vector1, const Vector3& vector2, const Vector3& vector3 );

		Plane operator + ( ) const;

		Plane operator - ( ) const;

		Vector3 Normal( ) const;

		float Distance( const Vector3& vector ) const;

		// This gives you the element of the input vector that is perpendicular 
		// to the normal of the plane. You can get the element which is parallel
		// to the normal of the plane by subtracting the result of this method
		// from the original vector, since parallel + perpendicular = original.
		Vector3 ProjectVector(const Vector3& p) const;


		enum eSide
		{
			NoSide,
			PositiveSide,
			NegativeSide,
			BothSide,
		};
		
		Plane::eSide GetSide( const Vector3& Center, const Vector3& HalfSize ) const;

		static bool Intersect(const Ray& ray, const Plane& plane, float& distance);

		static bool IntersectResult(const Ray& ray, const Plane& plane,Vector3& out);
		
		static const Plane cXYPlane;
		static const Plane cXZPlane;
		static const Plane cYZPlane;

	};

	enum eFrustumPlane
	{
		FrustumPlane_Front = 0,
		FrustumPlane_Back,
		FrustumPlane_Left,
		FrustumPlane_Right,
		FrustumPlane_Top,
		FrustumPlane_Bottom,
		MaxFrustumPlane,
	};

	/**
		6平面构造的体积体，类似于视锥
	*/
	class PlaneBoundedVolume
	{
	public:
		Plane		mPlanes[6];

		Vector3		mVectors[8];


		PlaneBoundedVolume( ){ }

		PlaneBoundedVolume(	const Ray& RayTopLeft, 
					const Ray& RayTopRight, 
					const Ray& RayBottomLeft, 
					const Ray& RayBottomRight, 
					float FrontDistance, 
					float BackDistance );

		bool Inside( const AABB& box ) const;
		bool Inside( const Box& box ) const;
	};


	/** 
		视锥
	*/
	class Frustum : public PlaneBoundedVolume
	{
	public:
		Frustum( );

		Frustum( const Matrix4& viewtransform, const Matrix4& projectiontransform );

		float Depth( ) const;
	};

	/** @} */
	/** @} */
}
