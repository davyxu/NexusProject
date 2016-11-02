#include "EnginePCH.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{
	VertexElement::VertexElement( word StreamIndex,  eVertexDataType DataType, eVertexUsage Usage, word UsageIndex) 
		: mDataType( DataType )
		, mUsage( Usage )
		, mStreamIndex( StreamIndex )
		, mUsageIndex( UsageIndex )
	{
		Assert( mStreamIndex < VertexFormat::MaxStreamCount)
		mStride = VertexFormat::GetElementStride( DataType );
	}

	VertexFormat::VertexFormat( )
	{

	}


	VertexFormat::~VertexFormat()
	{
		Clear();
	}

	VertexFormat::VertexFormat( const VertexFormat& vfp )
		: mVertexElementArray( vfp.mVertexElementArray )
	{
		
	}

	void VertexFormat::Clear( )
	{
		mVertexElementArray.clear();
	}

	void VertexFormat::Add( word StreamIndex, eVertexDataType DataType, eVertexUsage Usage, word UsageIndex )
	{
		mVertexElementArray.push_back(  VertexElement( StreamIndex, DataType, Usage, UsageIndex ) );
	}

	void VertexFormat::Add( const VertexElement& Element )
	{
		mVertexElementArray.push_back( Element );
	}

	word VertexFormat::GetElementStride( eVertexDataType type )
	{
		switch ( type )
		{
		case VDT_Float1:
			return sizeof(float);
		case VDT_Float2:
			return sizeof(float) * 2;
		case VDT_Float3:
			return sizeof(float) * 3;
		case VDT_Float4:
			return sizeof(float) * 4;
		case VDT_Color:
			return sizeof(dword);
		}

		return sizeof(float) * 3;
	}

	const VertexElement& VertexFormat::GetElement( dword i ) const
	{
		Assert( i < mVertexElementArray.size() )

		return mVertexElementArray[i];
	}

	dword VertexFormat::GetElementCount() const
	{
		return mVertexElementArray.size();
	}

	bool VertexFormat::operator==( const VertexFormat& vfp ) const
	{		
		if ( vfp.mVertexElementArray.size() != mVertexElementArray.size() )
			return false;

		return ::memcmp( &vfp.mVertexElementArray[0], &mVertexElementArray[0], sizeof(VertexElement)* mVertexElementArray.size() ) == 0;

	}

	NekoEngine::dword VertexFormat::GetTotalStride( word StreamIndex ) const
	{
		dword Stride = 0;
		for ( dword i = 0;i<mVertexElementArray.size();i++)
		{
			const VertexElement& Element = mVertexElementArray[i];
			Stride += Element.mStride;
		}

		return Stride;
	}


	NekoEngine::dword VertexFormat::GetElementOffset( eVertexUsage Usage, word UsageIndex /*= 0 */ ) const
	{
		dword ElementOffset = 0;
		for ( dword i = 0;i<mVertexElementArray.size();i++)
		{
			const VertexElement& Element = mVertexElementArray[i];

			if ( Element.mUsage == Usage && Element.mUsageIndex == UsageIndex )
				return ElementOffset;

			ElementOffset += Element.mStride;
		}

		return -1;
	}
}