#pragma once

namespace NekoEngine
{
	
	/**
		材质实例
		@remark
			MaterialInstance仅在运行时使用,编辑器不使用\
			MaterialInstance对渲染状态进行封装,抽象
			MaterialInstance需要依赖MaterialDescriptor的名称及MaterialParameter来构建自身内容
			MaterialInstance使用时,通过材质名称找到对应的Pixel Shader文件进行编译,并且在运行时传入需要的MaterialParameter进行使用
			在MaterialDescriptor更新后,MaterialInstance需要进行挂接更新
	*/
	class MaterialInstance : public ManagedObject
	{
		DECLARE_RTTI_CLASS( MaterialInstance, ManagedObject, ClassFlag_Intrinsic| ClassFlag_Serializable )
	public:
		MaterialInstance();

		virtual ~MaterialInstance( );

		virtual void Serialize( BinarySerializer& Ser );

		void SetVector4( const wchar* Name, const Vector4& Value );

		void SetColor( const wchar* Name, const Color& C );

		MaterialParameter* GetShaderParameter( dword UniformIndex );

		dword GetShaderParameterCount( );

		MaterialParameter* SearchParameter( const wchar* Name  );

		// 将Material中Compiler编译出的参数列表引用到MI，关联材质名称
		void LinkMaterial( MaterialDescriptor* Mat );

		// 提供一个VS的类，并根据材质名称从MaterialShaderFactory加载Shader
		void LoadShader( RTTIClass* VertexShaderProvider );

		// 设置Shader参数及提交VS,PS
		void CommitShader( bool VertexShaderOnly );

		void CommitRenderState( );

		// 外部设置VS参数
		MaterialVertexShader* GetVertexShader( ){ return mShaderCollection.mVertexShader; }

		template<typename TClass>
		TClass* GetVertexShader( )
		{
			return RTTI_CAST(TClass, GetVertexShader() );
		}

		static void StaticInitProperty();

		bool IsTransparent() { return mBlendMode == BLEND_Translucent; }

		bool IsEnableDepthWrite() { return mEnableDepthWrite; }


	protected:
		virtual void PostLoad( );

		virtual bool IsManualResourceReady( );
		
	private:
		friend class MaterialDescriptor;
		WString						mMaterialName;

		// 用于Shader输入的参数列表
		MaterialParameterArray		mShaderParameterArray;
	
		// Shader指针
		MaterialShaderCollection	mShaderCollection;


		RenderCommandFence mParamterResourceFence;

		bool		mUseTangentSpace;
		bool		mUseSkeletonMesh;
		eBlendMode	mBlendMode;
		bool		mWireframe;
		bool		mTwoSided;
		bool		mEnableDepthWrite;
	};

	typedef TRefPtr<MaterialInstance> MaterialInstancePtr;

	
}
