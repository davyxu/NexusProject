#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Render
	*  @{
	*/

	class RenderFont;

	/** 
		画布类
	*/
	class Canvas
	{
	public:
		/// 画布元素类型
		enum eElementType
		{
			ET_Line,		///< 线
			ET_Triangle,	///< 三角形
			ET_MAX,
		};

		/** 
			构造
			@param
				vp 输入一个视口
			@remark
				Canvas默认总是有一层2D矩阵，绘制2D图元时，无需压入矩阵
		*/
		Canvas( const Viewport& vp );

		virtual ~Canvas( );

		/** 
			绘制一条平面线条
		*/
		void DrawLine2D( const Vector2& P1, const Vector2& P2, const Color& C );

		/** 
			绘制一个空心矩形
		*/
		void DrawRect2D( const Vector2& P1, const Vector2& P2, const Color& C );

		/** 
			绘制一条3D线条
		*/
		void DrawLine( const Vector3& P1, const Vector3& P2, const Color& C );

		/** 
			绘制一条3D线条,通过硬件在C1到C2颜色间插值
			@param
				P1 顶点1
			@param
				P2 顶点2
			@param
				C1 颜色1
			@param
				C2 颜色2
		*/
		void DrawLine( const Vector3& P1, const Vector3& P2, const Color& C1, const Color& C2 );

		/** 
			绘制一个纹理面
			@param
				Pos 左上角
			@param
				Size 大小
			@param
				UV 图片纹理坐标左上角
			@param
				SizeUV 图片纹理坐标大小偏移
			@param
				C 颜色
			@param
				InTexture 纹理
			@param
				Mode 混合模式
		*/
		void DrawTile(	const Vector2& Pos, 
						const Vector2& Size, 
						const Vector2& UV, 
						const Vector2& SizeUV, 
						const Color& C, 
						RenderTexture* InTexture, 
						eBlendMode Mode = BLEND_Opaque );

		/** 
			绘制一个字符串
			@param
				Font 字体对象
			@param
				Pos 字符串左上角
			@param
				C 颜色
			@param
				Text 字符串
		*/
		void DrawString( RenderFont* Font, const Vector2& Pos, const Color& C, const wchar* Text );


		/** 
			压入一个新的变换矩阵
			@remark
				这个操作将刷新之前的缓冲
		*/
		void PushTransform( const Matrix4& mat );
		
		/** 
			弹出之前的矩阵
			@remark
				这个操作将刷新之前的缓冲
		*/
		void PopTransform( );

		/** 
			取图元绘制的完整变换
		*/
		const Matrix4& GetFullTransform( );

		/** 
			将缓冲中的内容提交到渲染线程
			@remark
				逻辑中避免使用Flush，应用Pop代替
		*/
		void Flush( );
		
	private:

		BatchedElements* GetBatcher( eElementType ElementType, RenderTexture* InTexture = null, eBlendMode BlendMode = BLEND_MAX);
		
		typedef std::vector<Matrix4> TransformStack;

		TransformStack mTransformStack;

		float mViewportWidth;
		float mViewportHeight;

		// 用于脏变量判定
		BatchedElements* mBatchedElements;
		RenderTexture* mTexture;
		eElementType mElementType;
		eBlendMode mBlendMode;
	};

	/** @} */
	/** @} */
}
