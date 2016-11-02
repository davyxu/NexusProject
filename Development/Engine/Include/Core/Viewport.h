#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/


	class Camera;

	/** 
		视口
	*/
	class Viewport
	{
	public:
		dword					X,Y, Width,Height;
		float					NearZ, FarZ;

		Viewport( );

		/** 
			将空间中的点，根据摄像机设定，投射到屏幕
		*/
		Vector3 Project( const Vector3& objVec, const Camera& cam ) const;

		/** 
			将空间位置中的点投射到屏幕
		*/
		static Vector3 Project(	const Vector3& objVec,
								const Matrix4& view,
								const Matrix4& proj,
								const Viewport& viewport);
		
		/// 将屏幕点，反投到世界中
		static Vector3 UnProject(const Vector2& screenVec,const Viewport& viewport,const Matrix4& proj,const Matrix4& InvView);

		/// 将屏幕的一个点，投射到世界中，作为一个光线使用
		static Ray UnProjectRay(const Vector2& screenVec,const Viewport& viewport,const Matrix4& proj,const Matrix4& InvView);
	};

	/** @} */
	/** @} */

}
