#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Model
	*  @{
	*/


	/** 
		模型用材质顶点Shader
	*/
	class ModelMaterialVertexShader : public MaterialVertexShader
	{
		DECLARE_RTTI_CLASS( ModelMaterialVertexShader, MaterialVertexShader, ClassFlag_Intrinsic| ClassFlag_Serializable )
		DECLARE_SHADER_METAINFO(ST_VertexShader, L"EngineShader.ModelMaterialVertexShader.SF",L"Main")
	public:

		/**
			设置Shader参数
			@param
				LocalToScreen 投射矩阵
			@param
				LocalToWorld 世界矩阵
			@param
				ViewPosition 摄像机位置
			@param
				WorldToLocal 世界逆矩阵
			@return
				
		*/
		void SetParameter( const Matrix4& LocalToScreen, const Matrix4& LocalToWorld, const Vector3& ViewPosition, const Matrix4& WorldToLocal  );

		/// 骨骼变换矩阵
		void SetBoneLocalTransform( const Matrix4* BoneLocalTransform, dword NumBone );

		virtual void Serialize( BinarySerializer& Ser );

	protected:
		/// 预处理Shader宏
		virtual void PreprocessMacro( MaterialInstance* Instance , ShaderCompilerEnvironment& CompilerEnvironment);

		/// 绑定模型材质参数
		virtual void BindMaterialParameter(  MaterialInstance* Instance, eShaderType ShaderType, const ShaderParameterMap& ParameterMap );

	private:
		ShaderParameter mTransform;
		ShaderParameter mViewPosition;
		ShaderParameter mLightPosition;
		ShaderParameter mWorldTransform;
		ShaderParameter mBoneLocalTransform;
	};

	/** @} */
	/** @} */	
}
