#pragma once

namespace NekoEngine
{
	/** 
		D3D9实现的RHI接口层
	*/
	class D3D9RHI : public IRenderHardware
	{
	public:
		virtual RHIVideoDevice*			CreateVideoDevice( void* window, dword Width, dword Height,bool isfullscn );

		virtual void					DrawDebugText(int x, int y, dword color,const wchar* format,...);

		virtual void					Clear(bool bClearColor,dword c,bool bClearDepth,float Depth,bool bClearStencil,dword Stencil);

		virtual void					Draw( ePrimitiveTopology PrimitiveType, RHIIndexBuffer* IndexBuffer, dword NumVertices );

		virtual void					DrawIndexedUserBuffer( ePrimitiveTopology PrimitiveType, dword NumPrimitives, dword NumVertices, const void* UserVertexStream	, dword VertexStreamStride, const void* UserIndexBuffer	, dword IndexStreamStride );

		virtual void					DrawUserBuffer( ePrimitiveTopology PrimitiveType, dword NumPrimitives, const void* UserVertexStream, dword VertexStreamStride );

		virtual RHITextureRef			CreateTexture2D( dword SizeX, dword SizeY, dword MipCount, ePixelFormat Format);		

		virtual RHIVertexFormatRef		CreateVertexFormat(const VertexFormat& Parameter);
		
		virtual bool					CompileShader(	const wchar* SourceFile, const wchar* EntryName,	eShaderType ShaderType,	ShaderCompilerEnvironment& Environment,	ShaderCompilerOutput& CompileOutput );

		virtual RHIVertexBufferRef		CreateVertexBuffer(dword Size,bool IsDynamic);

		virtual RHIIndexBufferRef		CreateIndexBuffer(dword Size,dword Stride, bool IsDynamic);

		virtual RHIRasterizerStateRef	CreateRasterizerState(const RasterizerStateInitializer& Initializer);

		virtual RHIDepthStateRef		CreateDepthState(const DepthStateInitializer& Initializer);

		virtual RHIStencilStateRef		CreateStencilState(const StencilStateInitializer& Initializer);

		virtual RHIBlendStateRef		CreateBlendState(const BlendStateInitializer& Initializer);

		virtual RHISamplerStateRef		CreateSamplerState(const SamplerStateInitializer& Initializer);
		
		virtual RHIShaderRef			CreateShader( eShaderType ShaderType, void* Code, dword CodeSize );

		virtual AString					DisassembleCode( void* Code );

		virtual void					SetDrawArea( dword X, dword Y, dword Width, dword Height );

		virtual RHITextureRef			CreateTargetableTexture2D( dword SizeX, dword SizeY, ePixelFormat Format);

		virtual void					RestoreRenderTarget( dword Index );

		virtual void					BeginDraw( );

		virtual void					EndDraw( );

		virtual void*					LockRenderTargetSurface( dword& DestStride, Rect* RectToLock = null );

		virtual void					UnlockRenderTargetSurface( );

		virtual RHISurfaceRef			CreateTargetableSurface( dword SizeX, dword SizeY, ePixelFormat Format, bool Readable );

		virtual dword					GetBitsPerPixel( ePixelFormat Format );
	};
}