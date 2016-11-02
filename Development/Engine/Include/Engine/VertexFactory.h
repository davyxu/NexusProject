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
		顶点工厂
	*/
	class VertexFactory: public RenderResource
	{
	public:

		VertexFactory(  );

		virtual ~VertexFactory( );

		/** 
			提交顶点格式,顶点缓冲及索引缓冲
		*/
		void Commit( );

		RHIIndexBuffer*				GetIndexBufferRHI( ){ return mIndexStreamRHI; }

		virtual DataStream*		GetVertexStream( ) = 0;

		virtual DataStream*		GetIndexStream( ) = 0;

		virtual dword				GetVertexCount( ) = 0;

		virtual VertexFormat*		GetFormatData( ) = 0;

		virtual dword				GetIndexCount( ) = 0;

	protected:
		virtual void				InitRHI( );

		virtual void				ReleaseRHI();
		
	private:
		RHIIndexBufferRef		mIndexStreamRHI;
		RHIVertexFormatRef		mVertexFormatRHI;

		word					mStreamCount;

		RHIVertexBufferRef		mVertexBufferRHI;


	};

	/** @} */
	/** @} */
}
