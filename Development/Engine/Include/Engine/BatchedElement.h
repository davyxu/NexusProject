#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Render
	*  @{
	*/

	/// 混合模式
	enum eBlendMode
	{
		BLEND_Opaque            =0,		///< 不透明
		BLEND_Masked            =1,		///< 蒙版
		BLEND_Translucent       =2,		///< 透明
		BLEND_Additive          =3,		///< 加亮
		BLEND_Modulate          =4,		///< 混合
		BLEND_MAX               =5,
	};

	/** 
		Element的顶点处理
	*/
	class ElementVertexShader : public RenderShader
	{
		DECLARE_RTTI_CLASS( ElementVertexShader, RenderShader, ClassFlag_Intrinsic| ClassFlag_Serializable )
		DECLARE_SHADER_METAINFO(ST_VertexShader, L"EngineShader.SimpleElementVertexShader.SF",L"Main")

	public:

		virtual void BindShaderParameter( eShaderType ShaderType, const ShaderParameterMap& ParameterMap );

		void SetParameter( const Matrix4& mat );

		virtual void Serialize( BinarySerializer& Ser );

	private:
		ShaderParameter mTransform;
	};

	/** 
		Element的像素处理
	*/
	class ElementPixelShader : public RenderShader
	{
		DECLARE_RTTI_CLASS( ElementPixelShader, RenderShader, ClassFlag_Intrinsic| ClassFlag_Serializable )
		DECLARE_SHADER_METAINFO(ST_PixelShader, L"EngineShader.SimpleElementPixelShader.SF",L"Main")

	public:

		virtual void BindShaderParameter( eShaderType ShaderType, const ShaderParameterMap& ParameterMap );

		void SetTexture( RenderTexture* NewTexture );

		virtual void Serialize( BinarySerializer& Ser );

	private:
		ShaderParameter mTexture;
	};


	/** 
		批量元素处理类
		@remark
			本类旨在合并DrawCall,减少顶点提交次数.需要工作在渲染线程
	*/
	class BatchedElements 
	{
	public:
		BatchedElements( );

		/** 
			添加一条3D线
			@param
				P1 顶点1
			@param
				P2 顶点2
			@param
				C 颜色
		*/
		void AddLine( const Vector3& P1, const Vector3& P2, const Color& C );

		/** 
			添加一条3D线,通过硬件在C1到C2颜色间插值
			@param
				P1 顶点1
			@param
				P2 顶点2
			@param
				C1 颜色1
			@param
				C2 颜色2
		*/
		void AddLine( const Vector3& P1, const Vector3& P2, const Color& C1, const Color& C2 );

		/** 
			添加一个多边形顶点
			@param
				Pos 顶点位置
			@param
				UV 顶点的UV
			@param
				C 顶点颜色
			@return
				返回顶点索引
		*/
		int AddTriangleVertex( const Vector3& Pos,const Vector2& UV , const Color& C );

		/** 
			添加一个多边形顶点索引
			@param
				V0,V1,V2 3个索引点
			@param
				TextureRes 纹理
			@param
				BlendMode 混合模式
		*/
		void AddTriangleIndex( int V0, int V1, int V2, RenderTexture* TextureRes, eBlendMode BlendMode );

		/** 
			绘制缓冲中各种图元
			@param
				Transform 变换
		*/
		void Draw( Matrix4& Transform );

		static void StaticInit( );
	private:
		struct SimpleElementVertex	
		{
			Vector3 mPosition;
			Vector2 mTextureCoordinate;
			dword	mColor;
			SimpleElementVertex( )
			{

			}

			SimpleElementVertex( const Vector3& Pos, dword C )
				: mPosition( Pos )
				, mColor( C )
			{

			}

			SimpleElementVertex( const Vector3& Pos, const Vector2& UV, dword C )
				: mPosition( Pos )
				, mTextureCoordinate( UV )
				, mColor( C )
			{

			}
		};

		struct BatchedMeshElement
		{
			int mMinVertex;
			
			int mMaxVertex;
			
			typedef growable_buffer<int,8> IndexBuffer;
			IndexBuffer mIndexData;

			RenderTexture* mTexture;
			eBlendMode mBlendMode;
		};

		// Line
		typedef growable_buffer<SimpleElementVertex,100> BatchedLineBuffer;
		BatchedLineBuffer mBatchedLineBuffer;	

		// Mesh
		typedef growable_buffer<SimpleElementVertex,100> BatchedMeshVertexBuffer;
		BatchedMeshVertexBuffer mBatchedMeshVertexBuffer;	

		typedef growable_buffer<BatchedMeshElement,100> BatchedMeshIndexBuffer;
		BatchedMeshIndexBuffer mBatchedMeshIndexBuffer;	

	};

	/** @} */
	/** @} */
}
