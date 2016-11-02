#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Render
	*  @{
	*/


	/** 
		需要提交到渲染线程的参数
	*/
	struct RenderFontTextureParameter 
	{
		/// GDI+的位图对象
		handle	mGDIPlusBitmap;

		/// GDI+的图元绘制对象
		handle	mGDIGraphics;

		/// 字模在纹理上的位置
		Point	mCharPos;

		/// 字模尺寸
		Point	mCharSize;
	};

	/** 
		字体纹理
	*/
	class RenderFontTexture: public RenderTexture
	{
	public:
		RenderFontTexture::RenderFontTexture( dword TextureSize );

		virtual void InitRHI( );

		void SyncData( const RenderFontTextureParameter& Parameter );

		void WaitSync( );

	private:
		dword mTextureSize;		
		RenderCommandFence	mSyncFence;
	};


	/** 
		字体类
		@remark
			管理一种字体固定尺寸的纹理缓冲
	*/
	class RenderFont
	{
	public:

		/** 
			构造函数
			@param
				fontname 字体名称
			@param
				filename 字体文件,默认为空
			@param
				size 字体尺寸
			@param
				border 字体是否有边
			@param
				bold 是否加厚
			@param
				italic 斜体
			@param
				underline 是否有下划线
		*/
		RenderFont( const wchar* fontname, 
					const wchar* filename, 
					dword size, 
					dword border, 
					bool bold, 
					bool italic, 
					bool underline );

		~RenderFont( );

		/// 获得字体名称
		const wchar*	GetFontName( ) const;

		/// 获得字体尺寸
		dword		GetFontSize( ) const;

		dword		GetFontBorder( ) const;
		bool		IsBlod( ) const;
		bool		IsItalic( ) const;
		bool		IsUnderline( ) const;

		/** 
			取得一个字的宽度
		*/
		dword GetCharacterWidth( wchar character );

		/** 
			取得一个字符串宽度
		*/
		dword GetStringWidth( const wchar* string );		

		void  SetFontColor( dword color );
		dword GetFontColor( ) const;
		void  SetBorderColor( dword color );
		dword GetBorderColor( ) const;

		/** 
			取得字模对应的所在纹理
			@param
				character 字
			@param
				uv 字的纹理坐标
			@param
				uvsize 纹理坐标大小
			@param
				charsize 字大小
		*/
		RenderFontTexture* GetFontTexture( wchar character, Vector2& uv, Vector2& uvsize, Vector2& charsize );

		static void StaticInit( );

		static void StaticExit( );

		void Save( const wchar* FileName );

	private:
		/** 
			缓存一个字
		*/
		void CacheChar( dword code );		

		/** 
			分配一个纹理
		*/
		RenderFontTexture* AllocTexture( );

		/** 
			取得字写入位置
		*/
		bool GetCharWritePos( dword CharWidth, dword CharHeight, Point& CharWritePos );
	private:
		enum { _TEXTURE_SIZE = 512 };		
		enum{ cGlyphMargin = 2 };

		typedef std::vector< RenderFontTexture* > TextureArray;

		struct FontCode
		{
			dword	mTextureIndex;
			dword	mWidth;
			dword	mHeight;
			int		mStartX;
			int		mStartY;
		};

		WString			mFontName;
		dword			mFontSize;
		bool			mBlod;
		bool			mItalic;
		bool			mUnderline;

		handle			mOldFont;
		handle			mGDIFont;
		handle			mOldMemBitmap;
		handle			mGDIMemBitmap;
		handle			mGDIMemDC;

		dword			mMaxCharWidth;
		dword			mMaxCharHeight;

		dword			mFontColor;
		dword			mBorderColor;


		FontCode		mCodeList[65536];
		TextureArray	mFontTextureArray;

		int				mPrintHeaderX;
		int				mPrintHeaderY;
		int				mMaxLineHeight;

	};

	/** @} */
	/** @} */
}
