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
		多边形拓扑结构
	*/
	enum ePrimitiveTopology
	{
		PT_PointList = 1,
		PT_LineList,
		PT_LineStrip,
		PT_TriangleList,
		PT_TriangleStrip,
	};

	/** 
		渲染结果
	*/
	struct RenderStatus 
	{
		dword mNumPrimitives;
		dword mNumVertices;
	};


	/** 
		像素格式
	*/
	enum ePixelFormat
	{
		PF_Unknown				= 0,
		PF_R8G8B8               = 20,
		PF_A8R8G8B8             = 21,
		PF_X8R8G8B8             = 22,
		PF_A8L8					= 51,
	};

	/** 
		RHI表面基类
	*/
	class RHISurfaceBase : public RefObject
	{
	public:
		virtual ~RHISurfaceBase( ){ }

		/** 
			保存表面为文件
		*/
		virtual void Save( const wchar* FileName ) = 0;

		/** 
			将表面提交为渲染目标
		*/
		virtual void CommitAsRenderTarget( dword Index ) = 0;

		/** 
			获取宽度
		*/
		virtual dword GetWidth( ) = 0;

		/** 
			获取高度
		*/
		virtual dword GetHeight( ) = 0;
	};

	/** 
		RHI表面
	*/
	class RHISurface : public RHISurfaceBase
	{
	public:
		virtual ~RHISurface( ){ }

		/** 
			锁定表面数据
			@param
				DestStride 返回跨度
			@param
				RectToLock 需要锁定的范围
		*/
		virtual void* Lock( dword& DestStride, Rect* RectToLock ) = 0;

		/** 
			解锁表面
		*/
		virtual void Unlock( ) = 0;
	};

	typedef TRefPtr<RHISurface> RHISurfaceRef;

	/** 
		RHI纹理
	*/
	class RHITexture : public RHISurfaceBase
	{
	public:
		virtual ~RHITexture( ){ }

		/** 
			提交到GPU
		*/
		virtual void Commit( dword SamplerIndex ) = 0;

		/** 
			锁定纹理
			@param
				MipIndex 锁定层次
			@param
				NeedWriteData 锁定后需要写入数据
			@param
				DestStride 返回跨度
			@param
				RectToLock 需要锁定的范围	
		*/
		virtual void* Lock( dword MipIndex,bool NeedWriteData,dword& DestStride, const Rect* RectToLock = null ) = 0;

		/** 
			解锁表面
		*/
		virtual void Unlock( dword MipIndex ) = 0;
	};

	typedef TRefPtr<RHITexture> RHITextureRef;


	/** 
		RHI顶点格式
	*/
	class RHIVertexFormat : public RefObject
	{
	public:
		virtual ~RHIVertexFormat( ){ }

		/** 
			提交
		*/
		virtual void Commit() = 0;
	};
	typedef TRefPtr<RHIVertexFormat> RHIVertexFormatRef;


	/** 
		RHI顶点缓冲
	*/
	class RHIVertexBuffer : public RefObject
	{
	public:
		virtual ~RHIVertexBuffer( ){ }

		/** 
			提交
			@param
				StreamIndex 流索引
			@param
				Stride 顶点大小
		*/
		virtual void Commit( dword StreamIndex, dword Stride  ) = 0;

		/** 
			锁定
			@param
				Offset 偏移
			@param
				Size 锁定大小
		*/
		virtual void* Lock( dword Offset, dword Size ) = 0;

		/** 
			解锁
		*/
		virtual void Unlock( ) = 0;

		/** 
			获得缓冲大小
		*/
		virtual dword GetSize( ) = 0;
	};

	typedef TRefPtr<RHIVertexBuffer> RHIVertexBufferRef;

	/** 
		RHI索引缓冲
	*/
	class RHIIndexBuffer : public RefObject
	{
	public:
		virtual ~RHIIndexBuffer( ){ }

		/** 
			提交
			@param
				StreamIndex 流索引
			@param
				Stride 顶点大小
		*/
		virtual void Commit(  ) = 0;

		/** 
			锁定
			@param
				Offset 偏移
			@param
				Size 锁定大小
		*/
		virtual void* Lock( dword Offset, dword Size ) = 0;

		/** 
			解锁
		*/
		virtual void Unlock( ) = 0;

		/** 
			获得缓冲大小
		*/
		virtual dword GetSize( ) = 0;
	};

	typedef TRefPtr<RHIIndexBuffer> RHIIndexBufferRef;

	/** 
		RHI渲染状态
	*/
	class RHIRenderState : public RefObject 
	{
	public:
		/** 
			提交
		*/
		virtual void Commit( ) = 0;
	};

	/** 
		RHI采样器状态
	*/
	class RHISamplerState: public RHIRenderState
	{
	public:
		virtual void Commit( dword SamplerIndex ) = 0;
	};
	typedef TRefPtr<RHISamplerState> RHISamplerStateRef;

	/** 
		RHI光栅化状态
	*/
	class RHIRasterizerState: public RHIRenderState{};
	typedef TRefPtr<RHIRasterizerState> RHIRasterizerStateRef;

	/** 
		RHI深度状态
	*/
	class RHIDepthState: public RHIRenderState {};
	typedef TRefPtr<RHIDepthState> RHIDepthStateRef;

	/** 
		RHI模板状态
	*/
	class RHIStencilState: public RHIRenderState{};
	typedef TRefPtr<RHIStencilState> RHIStencilStateRef;

	/** 
		RHI混合状态
	*/
	class RHIBlendState : public RHIRenderState{};
	typedef TRefPtr<RHIBlendState> RHIBlendStateRef;


	/** 
		Shader类型
	*/
	enum eShaderType
	{
		ST_VertexShader,
		ST_PixelShader,
		ST_Max,
	};

	class RenderTexture;

	/** 
		RHI视频设备
	*/
	class RHIVideoDevice 
	{
	public:
		/// 释放
		virtual void Release( ) = 0;

		/// 获取窗口句柄
		virtual handle GetWindowHandle() = 0;

		/// 窗口宽度
		virtual dword GetWidth( ) = 0;

		/// 窗口高度
		virtual dword GetHeight( ) = 0;

		/// 重新修改大小
		virtual void Resize( dword Width, dword Height, bool bInIsFullscreen, void* NewWindowHandle = null ) = 0;		

		/// 绘制到屏幕
		virtual void Present( ) = 0;

		/// 更新设备
		virtual void UpdateDevice( ) = 0;
	};

	
	/** 
		RHI Shader
	*/
	class RHIShader : public RefObject
	{
	public:
		virtual ~RHIShader( ){ }

		/** 
			提交
			@remark
				必须在渲染线程调用
		*/
		virtual void Commit() = 0;

		/** 
			设置Shader常量
			@param
				Parameter 保存有寄存器索引变量
			@param
				NewValue 常量指针
			@param
				NumVector4 有多少个Vector4
		*/
		virtual void SetContantParameter( const ShaderParameter& Parameter, const float* NewValue, dword NumVector4 ) = 0;

		/** 
			获取Shader类型
		*/
		eShaderType GetType( ){ return mShaderType; }

		/// 模板自动匹配设置Shader常量
		template<class TParameterType>
		void SetContantParameter( const ShaderParameter& Parameter, const TParameterType& Value )
		{
			if ( Parameter.IsBound() )
			{
				enum { RegisterSize = sizeof(float) * 4 };
				enum { NumRegistersPerElement = (sizeof(TParameterType) + RegisterSize - 1) / RegisterSize };
				Assert(NumRegistersPerElement <= Parameter.GetNumRegisters());

				SetContantParameter( Parameter, (float*)&Value, NumRegistersPerElement );
			}
		}

		/** 
			设置纹理及采样器
			@param
				Parameter 保存有寄存器索引变量
			@param
				NewSamplerState 采样器状态
			@param
				NewTexture 纹理
		*/
		virtual void SetTextureParameter( const ShaderParameter& Parameter, RHISamplerState* NewSamplerState, RHITexture* NewTexture ) = 0;

		/** 
			设置纹理
			@param
				Parameter 保存有寄存器索引变量
			@param
				NewTexture 纹理
			@remark
				没有纹理时使用白纹理及对应采样器
		*/
		virtual void SetTextureParameter( const ShaderParameter& Parameter, RenderTexture* NewTexture );

		/** 
			编译后的代码指针
		*/
		virtual _byte* GetCodePointer( );

		/** 
			编译后的代码大小
		*/
		virtual dword GetCodeSize( );

	protected:
		eShaderType			mShaderType;

		typedef std::vector<_byte> ShaderCodeBuffer;
		ShaderCodeBuffer	mCodeBuffer;
	};


	typedef TRefPtr<RHIShader> RHIShaderRef;

	/** 
		Shader编译输出
	*/
	struct ShaderCompilerOutput
	{
		/// 名称对应寄存器
		ShaderParameterMap	mParameterMap;

		/// 编译中发生的错误
		WString				mErrors;

		/// 编译好生成的Shader对象
		RHIShaderRef		mShaderRHI;
	};


	class TextureMipMap;

	extern RenderStatus GRenderStatus;

	/** 
		渲染硬件抽象接口
		@remark
			以下接口必须保证始终在渲染线程使用
	*/
	class IRenderHardware
	{
	public:
		/** 
			创建一个硬件视频设备
			@param
				window 窗口句柄
			@param
				Width,Height 窗口尺寸
			@param
				isfullscn 是否全屏
			@return
				创建好的视频设备句柄
		*/
		virtual RHIVideoDevice*			CreateVideoDevice( void* window, dword Width, dword Height, bool isfullscn ) = 0;

		/** 
			绘制调试字体绘制,没有处理层次关系
		*/
		virtual void					DrawDebugText(int x, int y, dword color,const wchar* format,...) = 0;

		/** 
			清理当前绘制目标
			@param
				bClearColor 是否使用清除颜色
			@param
				c 清除颜色
			@param
				bClearDepth 是否清除深度缓冲
			@param
				Depth 深度
			@param
				bClearStencil 是否清理模板缓冲
			@param
				Stencil 模板
		*/
		virtual void					Clear(	bool bClearColor,
												dword c,
												bool bClearDepth,
												float Depth,
												bool bClearStencil,
												dword Stencil) = 0;

		/** 
			根据当前设置的顶点缓冲,绘制图元
			@param
				PrimitiveType 图元拓扑类型
			@param
				IndexBuffer 索引缓冲,可以为空
			@param
				NumVertices 顶点数
		*/
		virtual void					Draw(	ePrimitiveTopology PrimitiveType, 
												RHIIndexBuffer* IndexBuffer, 
												dword NumVertices ) = 0;

		/** 
			使用带索引的用户缓冲进行图元绘制
			@param
				PrimitiveType 图元拓扑类型
			@param
				NumPrimitives 多边形数
			@param
				NumVertices 顶点数
			@param
				UserVertexStream 用户顶点缓冲内存
			@param
				VertexStreamStride 用户顶点缓冲顶点大小
			@param
				UserIndexBuffer 用户索引缓冲
			@param
				IndexStreamStride 索引缓冲跨度
		*/
		virtual void					DrawIndexedUserBuffer(  ePrimitiveTopology PrimitiveType, 
																dword NumPrimitives, 
																dword NumVertices, 
																const void* UserVertexStream, 
																dword VertexStreamStride, 
																const void* UserIndexBuffer, 
																dword IndexStreamStride ) = 0;

		/** 
			使用用户缓冲进行图元绘制
			@param
				PrimitiveType 图元拓扑类型
			@param
				NumPrimitives 多边形数
			@param
				UserVertexStream 用户顶点缓冲内存
			@param
				VertexStreamStride 用户顶点缓冲顶点大小
		*/
		virtual void					DrawUserBuffer( ePrimitiveTopology PrimitiveType,
														dword NumPrimitives, 
														const void* UserVertexStream, 
														dword VertexStreamStride ) = 0;

		/** 
			创建2D纹理
			@param
				SizeX,SizeY 纹理尺寸
			@param
				MipCount 层次
			@param
				Format 格式
			@return
				创建好的纹理
		*/
		virtual RHITextureRef			CreateTexture2D( dword SizeX, dword SizeY, dword MipCount, ePixelFormat Format) = 0;

		/** 
			创建顶点格式对象
			@param
				Parameter 创建参数
			@return
				创建好的顶点格式
		*/
		virtual RHIVertexFormatRef		CreateVertexFormat( const VertexFormat& Parameter ) = 0;


		/** 
			创建顶点缓冲
			@param
				Size 缓冲大小
			@param
				IsDynamic 是否动态
			@return
				创建好的顶点缓冲
		*/
		virtual RHIVertexBufferRef		CreateVertexBuffer(dword Size,bool IsDynamic) = 0;

		/** 
			创建索引缓冲
			@param
				Size 缓冲大小
			@param
				Stride 跨度大小(使用sizeof(word)或者dword)
			@param
				IsDynamic 是否动态
			@return
				创建好的索引缓冲
		*/
		virtual RHIIndexBufferRef		CreateIndexBuffer(dword Size,dword Stride, bool IsDynamic) = 0;

		/// 创建光栅化状态
		virtual RHIRasterizerStateRef	CreateRasterizerState(const RasterizerStateInitializer& Initializer) = 0;

		/// 创建深度状态
		virtual RHIDepthStateRef		CreateDepthState(const DepthStateInitializer& Initializer) = 0;

		/// 创建模板状态
		virtual RHIStencilStateRef		CreateStencilState(const StencilStateInitializer& Initializer) = 0;

		/// 创建混合状态
		virtual RHIBlendStateRef		CreateBlendState(const BlendStateInitializer& Initializer) = 0;

		/// 创建采样器状态
		virtual RHISamplerStateRef		CreateSamplerState(const SamplerStateInitializer& Initializer) = 0;

		/** 
			编译Shader源码
			@param
				SourceFile 源文件位置
			@param
				EntryName 入口函数名
			@param
				ShaderType 类型
			@param
				Environment 编译时使用的宏
			@param
				CompileOutput 编译输出
			@return
				编译是否成功
			@remark
				无需在渲染线程调用
		*/
		virtual bool					CompileShader(  const wchar* SourceFile, 
														const wchar* EntryName,	
														eShaderType ShaderType,	
														ShaderCompilerEnvironment& Environment,	
														ShaderCompilerOutput& CompileOutput ) = 0;

		/** 
			从编译好的Shader缓冲创建Shader对象
			@param
				ShaderType 类型
			@param
				Code 代码缓冲
			@param
				CodeSize 代码大小
			@return
				创建好的Shader对象
			@remark
				无需在渲染线程调用
		*/
		virtual RHIShaderRef			CreateShader( eShaderType ShaderType, void* Code, dword CodeSize ) = 0;

		/** 
			反编译Shader代码
		*/
		virtual AString					DisassembleCode( void* Code ) = 0;

		/** 
			设置绘制区域,同SetViewport
		*/
		virtual void					SetDrawArea( dword X, dword Y, dword Width, dword Height ) = 0;

		/** 
			创建可以作为目标的纹理
			@param
				SizeX,SizeY 纹理尺寸
			@param
				Format 格式
			@return
				创建好的纹理
		*/
		virtual RHITextureRef			CreateTargetableTexture2D( dword SizeX, dword SizeY, ePixelFormat Format) = 0;

		/** 
			将索引指定的目标重新设为后备缓冲
			@param
				Index 渲染目标索引
		*/
		virtual void					RestoreRenderTarget( dword Index ) = 0;

		/// 开始绘制
		virtual void					BeginDraw( ) = 0;

		/// 结束绘制
		virtual void					EndDraw( ) = 0;

		/** 
			锁定渲染目标表面
			@remark
				本操作很慢
		*/
		virtual void*					LockRenderTargetSurface( dword& DestStride, Rect* RectToLock = null ) = 0;

		/// 解锁表面
		virtual void					UnlockRenderTargetSurface( ) = 0;

		virtual RHISurfaceRef			CreateTargetableSurface( dword SizeX, dword SizeY, ePixelFormat Format, bool Readable ) = 0;

		virtual dword					GetBitsPerPixel( ePixelFormat Format ) = 0;
	};

	/// RHI层初始化
	extern void RHIInitialize( );

	/// RHI层析构
	extern void RHIFinalize( );

	extern IRenderHardware* RHI;

	/** 
		DDS纹理文件参数
	*/
	struct DDSTextureParameter 
	{
		dword			mSizeX;
		dword			mSizeY;
		ePixelFormat	mPixelFormat;

		DDSTextureParameter( )
			: mSizeX( 0 )
			, mSizeY( 0 )
			, mPixelFormat( PF_Unknown )
		{

		}
	};

	/** 
		加载一个DDS文件到缓冲
		@param
			FileName 文件名
		@param
			MipData 缓冲
		@param
			Parameter 读取后的参数
		@return
			是否读取成功
	*/
	extern bool	DDSCodec_Load( const wchar* FileName, TextureMipMap& MipData, DDSTextureParameter& Parameter );

	/** 
		从DataStream加载DDS到缓冲
		@param
			Stream 流
		@param
			MipData 缓冲
		@param
			Parameter 读取后的参数
		@return
			是否读取成功
	*/
	extern bool DDSCodec_Load( DataStream* Stream, TextureMipMap& MipData, DDSTextureParameter& Parameter );

	/** @} */
	/** @} */
}
