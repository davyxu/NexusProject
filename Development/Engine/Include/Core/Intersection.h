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
		图元穿插判定
	*/
	class Intersection
	{
	public:

		/// 交叉中的多边形类型
		enum ePrimitiveType
		{
			TriangleList,
			TriangleStrip,
		};

		/** 
			光线穿平面
			@param
				ray 光线
			@param
				plane 平面
			@param
				distance 返回的光线上点的距离
		*/
		static bool RayVSPlane( const Ray& ray, const Plane& plane, float& distance );

		/** 
			光线穿三角形
			@param
				ray 光线
			@param
				triangle 三角形
			@param
				distance 返回的光线上点的距离
		*/
		static bool RayVSTriangle( const Ray& ray, const Triangle& triangle, float& distance );

		/** 
			光线穿盒子
			@param
				ray 光线
			@param
				box 盒子
			@param
				distance 返回的光线上点的距离
		*/
		static bool RayVSBox( const Ray& ray, const Box& box, float& distance );

		/** 
			光线穿轴对称绑定盒
			@param
				ray 光线
			@param
				aabb 轴对称绑定盒
			@param
				distance 返回的光线上点的距离
		*/
		static bool RayVSAABox( const Ray& ray, const AABB& aabb, float& distance );

		/** 
			光线穿模型
			@param
				TRay 光线
			@param
				Distance 返回的光线上点的距离
			@param
				VertexSize 一个顶点大小
			@param
				PType 模型多边形类型
			@param
				PrimitiveCount 多边形数量
			@param
				VertexBuffer 顶点缓冲，包含n个顶点序列
			@param
				IndexBuffer 索引缓冲，word类型
			@return
				是否穿插到
		*/
		static bool RayVSMesh(	const Ray& TRay, 
								float& Distance,
								dword VertexSize,
								ePrimitiveType PType,
								dword PrimitiveCount,
								const _byte* VertexBuffer,
								const _byte* Indexbuffer );

		/** 
			轴对称绑定盒互相穿插
			@param
				box1 盒子1
			@param
				box2 盒子2
			@return
				是否穿插到
		*/
		static bool AABoxVSAABox( const AABB& box1, const AABB& box2 );

		/** 
			球穿球
			@param
				sphere1 球1
			@param
				sphere2 球2
			@return
				是否穿插到
		*/
		static bool SphereVSSphere( const Sphere& sphere1, const Sphere& sphere2 );

		/** 
			静态轴对称绑定盒穿平面
			@param
				box 盒子
			@param
				plane 平面
			@return
				是否穿插到
		*/
		static bool StaticAABoxVSPlane( const AABB& box, const Plane& plane );

		/** 
			动态轴对称绑定盒穿平面
			@param
				box 盒子
			@param
				plane 平面
			@return
				是否穿插到
		*/
		static bool DynamicAABoxVSPlane( const AABB& box, const Plane& plane, const Vector3& direction, float& distance );

		/** 
			静态球穿平面
			@param
				sphere 球
			@param
				plane 平面
			@return
				是否穿插到
		*/
		static bool StaticSphereVSPlane( const Sphere& sphere, const Plane& plane );

		/** 
			动态球穿平面
			@param
				sphere 球
			@param
				plane 平面
			@return
				是否穿插到
		*/
		static bool DynamicSphereVSPlane( const Sphere& sphere, const Plane& plane, const Vector3& direction, float& distance );

		/** 
			矩形穿模型
			@param
				ScreenArea 屏幕区域
			@param
				Cam 摄像机
			@param
				LocalToWorld 本地坐标到世界的转换矩阵
			@param
				BackDistance 距离
			@param
				VertexSize 顶点大小
			@param
				PType 多边形类型
			@param
				PrimitiveCount 多边形数量
			@param
				VertexBuffer 顶点缓冲
			@param
				IndexBuffer 索引缓冲
			@return
				是否穿插到
		*/
		static bool RectVSMesh( const FRect& ScreenArea, 
								Camera* Cam, 
								const Matrix4& LocalToWorld, 
								float BackDistance, 
								dword VertexSize, 
								ePrimitiveType PType, 
								dword PrimitiveCount, 
								const _byte* VertexBuffer, 
								const _byte* IndexBuffer );


	private:
		typedef bool (*IterateVertexCallback)( void* UserData, const Vector3& P1, const Vector3& P2, const Vector3& P3 );

		static void IterateMeshVertex( dword VertexSize, ePrimitiveType PrimitiveType, dword PrimitiveCount, const _byte* VertexBuffer, const _byte* IndexBuffer, IterateVertexCallback Func, void* UserData );
	};

	/** @} */
	/** @} */
}
