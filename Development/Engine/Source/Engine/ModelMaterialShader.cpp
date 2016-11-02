#include "EnginePCH.h"
#include "NekoEngine_Model.h"


namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS( ModelMaterialVertexShader);


	void ModelMaterialVertexShader::PreprocessMacro( MaterialInstance* Instance , ShaderCompilerEnvironment& CompilerEnvironment )
	{
		if ( Instance->CheckObjectPropertyT(L"UseTangentSpace", true ) )
			CompilerEnvironment.mMacroDefineMap.set("TANGENT_SPACE", "1");

		if ( Instance->CheckObjectPropertyT(L"UseSkeletonMesh", true ) )
			CompilerEnvironment.mMacroDefineMap.set("USE_SKIN", "1");
	}


	void ModelMaterialVertexShader::BindMaterialParameter( MaterialInstance* Instance, eShaderType ShaderType, const ShaderParameterMap& ParameterMap )
	{
		mTransform.Bind( ParameterMap, "WorldViewProjectionTransform" );
		mWorldTransform.Bind( ParameterMap, "WorldTransform" );
		mViewPosition.Bind( ParameterMap, "ViewPosition" );
		mLightPosition.Bind( ParameterMap, "LightPosition" );

		mBoneLocalTransform.Bind( ParameterMap, "BoneLocalTransform" );
	}

	void ModelMaterialVertexShader::SetParameter( const Matrix4& LocalToScreen, const Matrix4& LocalToWorld, const Vector3& ViewPosition, const Matrix4& WorldToLocal )
	{
		mShaderRHI->SetContantParameter( mTransform, LocalToScreen );
		mShaderRHI->SetContantParameter( mViewPosition, ViewPosition * WorldToLocal);
		mShaderRHI->SetContantParameter( mWorldTransform, (const float*)&LocalToWorld, 3 );
		mShaderRHI->SetContantParameter( mLightPosition, GEngine->mTestPoint * WorldToLocal );// 灯光需要转换为对象坐标系
	}

	void ModelMaterialVertexShader::SetBoneLocalTransform( const Matrix4* BoneLocalTransform, dword NumBone )
	{
		mShaderRHI->SetContantParameter( mBoneLocalTransform, (const float*)BoneLocalTransform, NumBone * 4);
	}

	void ModelMaterialVertexShader::Serialize( BinarySerializer& Ser )
	{
		Super::Serialize( Ser );

		Ser << mTransform
			<< mViewPosition
			<< mLightPosition
			<< mWorldTransform 
			<< mBoneLocalTransform;
	}


}