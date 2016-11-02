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
		顶点工厂
		整合模型顶点,索引及格式对象
	*/
	class MeshVertexFactory : public RTTIObject
							, public VertexFactory
	{
		DECLARE_RTTI_CLASS( MeshVertexFactory, RTTIObject, ClassFlag_Intrinsic| ClassFlag_Serializable )

	public:
		MeshVertexFactory( );

		~MeshVertexFactory( );

		/// 获取顶点数据
		virtual DataStream*		GetVertexStream( ){ return mVertexStream; }

		/// 获取索引数据
		virtual DataStream*		GetIndexStream( ){ return mIndexStream; }

		/// 获取顶点数量
		virtual dword				GetVertexCount( ){ return mVertexCount; }

		/// 获取格式
		virtual VertexFormat*		GetFormatData( ){ return mFormatData; }

		/// 获取索引数量
		virtual dword				GetIndexCount( ){ return mIndexCount; }

		/// 获取绑定盒
		AABB GetStaticBoundingBox( );

	public:
		virtual void Serialize( BinarySerializer& Ser );

		void InitData(	const WString& Name, 
					dword VertexCount, 
					HeapStream* VertexStream,
					dword IndexCount,
					HeapStream* IndexStream,
					dword MaterialIndex );
	private:
		friend class MeshResource;
		WString				mName;
		dword				mMaterialIndex;

		dword				mVertexCount;
		dword				mIndexCount;

		HeapStream*			mIndexStream;
		HeapStream*			mVertexStream;
		VertexFormat*		mFormatData;
		AABB				mCachedAABB;
	};

	/** 
		通用模型资源
	*/
	class MeshResource : public ManagedObject
	{
		DECLARE_RTTI_CLASS( MeshResource, ManagedObject, ClassFlag_Intrinsic| ClassFlag_Serializable )
	public:
		MeshResource( );

		virtual ~MeshResource( );

		
		///	添加子模型的顶点工厂
		dword AddVertexFactory( MeshVertexFactory* Factory );

		/// 工厂数量
		dword GetVertexFactoryCount( );

		/// 获取工厂
		MeshVertexFactory* GetVertexFactory( dword Index );
		
		/// 获取工厂,模板
		template<typename TClass>
		TClass* GetVertexFactory( dword Index )
		{
			return RTTI_CAST(TClass, GetVertexFactory( Index ) );
		}

		/// 获取顶点格式
		VertexFormat& GetVertexFormat( ){ return mVertexFormat;}

		/// 获取绑定盒
		AABB GetBoundingBox( );

	public:
		virtual void Serialize( BinarySerializer& Ser );

	protected:
		virtual void PostLoad( );

		virtual bool IsManualResourceReady();

		virtual void BeginDispose( );

		virtual bool CanDestroyNow( );

	private:
		void InitializeVertexFactory( );
		
	protected:
		friend class VertexFactory;
		typedef std::vector<MeshVertexFactory*> VertexFactoryList;
		VertexFormat			mVertexFormat;

		VertexFactoryList		mVertexFactoryList;			
		RenderCommandFence		mReadyFence;
		RenderCommandFence		mReleaseFence;
		AsyncTaskFence		mStreamingFence;
	};

}
