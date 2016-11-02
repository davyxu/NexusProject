#include "D3D9PCH.h"



namespace NekoEngine
{
	dword Mapping(eVertexDataType ct)
	{
		switch(ct)
		{
		case VDT_Float1:
			return D3DDECLTYPE_FLOAT1;
		case VDT_Float2:
			return D3DDECLTYPE_FLOAT2;
		case VDT_Float3:
			return D3DDECLTYPE_FLOAT3;
		case VDT_Float4:
			return D3DDECLTYPE_FLOAT4;
		case VDT_Color:
			return D3DDECLTYPE_D3DCOLOR;
		}

		// to keep compiler happy
		return D3DDECLTYPE_FLOAT3;
	}

	dword Mapping(eVertexUsage usage)
	{
		switch (usage)
		{
		case VU_Position:
			return D3DDECLUSAGE_POSITION;
		case VU_Diffuse:
			return D3DDECLUSAGE_COLOR; // NB index will differentiate
		case VU_Normal:
			return D3DDECLUSAGE_NORMAL;
		case VU_TexCoord:
			return D3DDECLUSAGE_TEXCOORD;
		case VU_PositionRHW:
			return D3DDECLUSAGE_POSITIONT;
		case VU_Blend_Indices:
			return D3DDECLUSAGE_BLENDINDICES;
		case VU_Blend_Weights:
			return D3DDECLUSAGE_BLENDWEIGHT;
		case VU_Specular:
			return D3DDECLUSAGE_COLOR; // NB index will differentiate
		case VU_Binormal:
			return D3DDECLUSAGE_BINORMAL;
		case VU_Tangent:
			return D3DDECLUSAGE_TANGENT;
		}

		// to keep compiler happy
		return D3DDECLUSAGE_POSITION;
	}


	D3D9VertexFormat::D3D9VertexFormat(const VertexFormat& vfp)
		: mElementCount( 0 )
		, mDeclaration( null )
	{
		// 每个流单独计算偏移
		word Offset[VertexFormat::MaxStreamCount] = { 0 };

		for( dword i = 0;i< vfp.GetElementCount();i++ )
		{
			const VertexElement& VElement = vfp.GetElement( i );

			Assert( mElementCount < VertexFormat::MaxVertexElementCount );
			D3DVERTEXELEMENT9& D3DElement = VertexElements[mElementCount++];
			D3DElement.Stream			= VElement.mStreamIndex;
			D3DElement.Offset			= Offset[D3DElement.Stream];
			D3DElement.Type				= BYTE(Mapping(VElement.mDataType));
			D3DElement.Usage			= BYTE(Mapping(VElement.mUsage));
			D3DElement.Method			= D3DDECLMETHOD_DEFAULT;
			D3DElement.UsageIndex		= BYTE(VElement.mUsageIndex );
			Offset[D3DElement.Stream]	+= VElement.mStride;
		}

		// 末尾符
		Assert( mElementCount < VertexFormat::MaxVertexElementCount );
		D3DVERTEXELEMENT9& newElement = VertexElements[mElementCount++];
		newElement.Stream		= 0xff;
		newElement.Offset		= 0;
		newElement.Type			= D3DDECLTYPE_UNUSED;
		newElement.Method		= 0;
		newElement.Usage		= 0;
		newElement.UsageIndex = 0;

		Hash = CheckSum::MemCRC( GetVertexElements(), sizeof(D3DVERTEXELEMENT9) * mElementCount ); 
	}

	D3D9VertexFormat::D3D9VertexFormat()
		: mDeclaration( null )
	{

	}
	D3D9VertexFormat::~D3D9VertexFormat()
	{
		SafeRelease( mDeclaration );
	}

	bool D3D9VertexFormat::operator == (const D3D9VertexFormat& Other)
	{
		// same number and matching element
		return	mElementCount == Other.mElementCount && 
			!::memcmp(Other.GetVertexElements(), GetVertexElements(), sizeof(D3DVERTEXELEMENT9) * mElementCount );
	}


	D3DVERTEXELEMENT9* D3D9VertexFormat::GetVertexElements() 
	{ 
		return &VertexElements[0];
	}
	const D3DVERTEXELEMENT9* D3D9VertexFormat::GetVertexElements() const
	{ 
		return &VertexElements[0];
	}


	typedef dynamic_hash<D3D9VertexFormat, RHIVertexFormatRef> VertexDeclarationMap;
	VertexDeclarationMap GVertexFormatMap;

	RHIVertexFormatRef D3D9RHI::CreateVertexFormat(const VertexFormat& Parameter)
	{
		D3D9VertexFormat* VertexFormatPtr = new D3D9VertexFormat( Parameter );
		RHIVertexFormatRef* Value = GVertexFormatMap.find( *VertexFormatPtr );
		if ( Value == null )
		{
			GVertexFormatMap.set(*VertexFormatPtr, VertexFormatPtr );
			return VertexFormatPtr;
		}
		else
		{
			delete VertexFormatPtr;
		}

		return *Value;
	}

	void RHIUnloadVertexFormat( )
	{
		for ( VertexDeclarationMap::iterator it(GVertexFormatMap);it;++it)
		{
			RHIVertexFormatRef& Element = it.get_value();
			Element.Release();
		}

		GVertexFormatMap.clear();
			
	}



	void D3D9VertexFormat::Commit()
	{
		Assert( IsInRenderingThread() );

		if ( mDeclaration == null )
		{
			VD3DHR(GDirect3DDevice->CreateVertexDeclaration(VertexElements,&mDeclaration));
		}

		if ( mDeclaration != null )
			VD3DHR(GDirect3DDevice->SetVertexDeclaration( mDeclaration ));
	}


}