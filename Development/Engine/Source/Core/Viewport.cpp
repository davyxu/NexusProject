#include "CorePCH.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{
	Viewport::Viewport(void) 
		: X(0)
		, Y(0)
		, Width( 0 )
		, Height( 0 )
		, NearZ( 0.1f )
		, FarZ( 1000.0f )// D3D的Viewport不使用这里的NearZ,FarZ，恒定0到1
	{

	}

	Vector3 Viewport::Project( const Vector3& objVec, const Camera& cam ) const
	{
		return Viewport::Project( objVec, cam.GetViewMatrix(), cam.GetProjectMatrix(), *this );
	}


	Vector3 Viewport::Project(	const Vector3& objVec, 
								const Matrix4& view,
								const Matrix4& proj,
								const Viewport& viewport)
	{
		Vector3 out;

		Vector4 a(objVec, 1.0f);
		//Vector4 b =  a * world * view * proj;
		Vector4 b =  a * view * proj;

		Vector4 c = b / b.w;

		out.x = viewport.X + viewport.Width  * ( 1 + c.x) *0.5f;
		out.y = viewport.Y + viewport.Height * ( 1 - c.y) * 0.5f;
		out.z = viewport.NearZ + c.z * (viewport.FarZ - viewport.NearZ);

		return out;
	}

	Vector3 Viewport::UnProject(const Vector2& screenVec,const Viewport& viewport,const Matrix4& proj,const Matrix4& InvView)
	{
		Vector3 vOrg;
		vOrg.x = (2 * screenVec.x/viewport.Width - 1)/proj.m11;
		vOrg.y = -(2 * screenVec.y/viewport.Height - 1)/proj.m22;
		vOrg.z = 1.0f;

		return vOrg * InvView;
	}

	Ray Viewport::UnProjectRay(const Vector2& screenVec,const Viewport& viewport,const Matrix4& proj,const Matrix4& InvView)
	{
		Vector3 vOrg;
		vOrg.x = (2 * screenVec.x/viewport.Width - 1)/proj.m11;
		vOrg.y = -(2 * screenVec.y/viewport.Height - 1)/proj.m22;
		vOrg.z = 1.0f;

		Vector3 Dir, Origin;
		Dir.x = vOrg.x * InvView.m11 + vOrg.y * InvView.m21 + vOrg.z * InvView.m31;
		Dir.y = vOrg.x * InvView.m12 + vOrg.y * InvView.m22 + vOrg.z * InvView.m32;
		Dir.z = vOrg.x * InvView.m13 + vOrg.y * InvView.m23 + vOrg.z * InvView.m33;
		Origin.x = InvView.m41;
		Origin.y = InvView.m42;
		Origin.z = InvView.m43;

		return Ray( Origin, Dir );
	}
}