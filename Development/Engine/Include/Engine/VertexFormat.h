#pragma once

namespace NekoEngine
{
	/** 
		顶点数据类型
	*/
	enum eVertexDataType
	{
		VDT_Float1 = 1,
		VDT_Float2,
		VDT_Float3,
		VDT_Float4,
		VDT_Color,
	};

	/** 
		顶点用途
	*/
	enum eVertexUsage
	{
		VU_Position = 1,
		VU_Normal,
		VU_Diffuse,
		VU_Specular,
		VU_TexCoord,
		VU_PositionRHW,
		VU_Blend_Indices,
		VU_Blend_Weights,
		VU_Binormal,
		VU_Tangent,
	};

	/** 
		顶点元素
	*/
	#pragma pack(push,1)
		struct VertexElement
		{
			eVertexDataType	mDataType;
			eVertexUsage	mUsage;
			word			mStreamIndex;
			word			mStride;		// 空间换速度
			word			mUsageIndex;

			VertexElement( ){}

			VertexElement( word StreamIndex,  eVertexDataType DataType, eVertexUsage Usage, word UsageIndex = 0 );

			friend BinarySerializer& operator <<( NekoEngine::BinarySerializer& Ser, VertexElement& Data )
			{
				if ( Ser.IsLoading() )
				{
					dword DataType, Usage;
					Ser << DataType << Usage;
					Data.mDataType = (eVertexDataType)DataType;
					Data.mUsage = (eVertexUsage)Usage;
				}
				else
				{
					Ser << (dword)Data.mDataType
						<< (dword)Data.mUsage;
				}
				
				Ser << Data.mStreamIndex
					<< Data.mStride
					<< Data.mUsageIndex;

				return Ser;
			}
		};

	#pragma pack(pop)

	/** 
		顶点格式,为创建硬件的参数
	*/
	class VertexFormat 
	{
	public:
		enum{ MaxVertexElementCount = 16,
			MaxStreamCount = 16,
		};

		VertexFormat( );

		~VertexFormat( );

		VertexFormat( const VertexFormat& vfp );

		bool operator == ( const VertexFormat& vfp  ) const;

		/** 
			添加顶点格式
			@remark
				自动计算offset
		*/
		void Add( word StreamIndex, eVertexDataType DataType, eVertexUsage Usage, word UsageIndex = 0  );

		void Add( const VertexElement& Element );

		/// 清空重新计算
		void Clear( );

		/** 
			获取顶点类型的跨度
		*/
		static word GetElementStride( eVertexDataType type );

		/** 
			取一个元素
		*/
		const VertexElement& GetElement( dword i ) const;

		/** 
			取元素数量
		*/
		dword GetElementCount( ) const;

		/** 
			取一共的跨度
		*/
		dword GetTotalStride( word StreamIndex ) const;

		/** 
			取某一个元素的偏移
		*/
		dword GetElementOffset( eVertexUsage Usage, word UsageIndex = 0 ) const;

		/// 二进制序列化操作符重载
		friend BinarySerializer& operator <<( NekoEngine::BinarySerializer& Ser, VertexFormat& Data )
		{
			Ser << Data.mVertexElementArray;

			return Ser;
		}

	private:
		typedef std::vector< VertexElement > VertexElementArray;
		VertexElementArray mVertexElementArray;
	};


}
