#include "D3D9PCH.h"


namespace NekoEngine
{
	#define DDS_MAGIC 0x20534444 // "DDS "

	D3DFORMAT GetD3D9Format( const DDS_PIXELFORMAT& ddpf );

	void DDSCodec_GetSurfaceInfo( dword width, dword height, dword fmt, dword* pNumBytes, dword* pRowBytes, dword* pNumRows );

	void DDSCodec_NextMip( dword& width, dword& height );



	bool DDSCodec_Load( DataStream* Stream, TextureMipMap& MipData, DDSTextureParameter& Parameter )
	{
		dword FileSize = Stream->GetSize();

		// Need at least enough data to fill the header and magic number to be a valid DDS
		if( FileSize < (sizeof(DDS_HEADER)+sizeof( dword )) )
			return false;

		dword DDSMagic = 0;
		if ( !Stream->ReadTypes<dword>( DDSMagic ) || DDSMagic != DDS_MAGIC )
			return false;

		// Read the DDS header
		DDS_HEADER DDSHeader;
		if ( !Stream->ReadBuffer( &DDSHeader, sizeof( DDSHeader ) ) )
			return false;

		if (DDSHeader.dwCubemapFlags != 0
			|| (DDSHeader.dwHeaderFlags & DDS_HEADER_FLAGS_VOLUME) )
		{
			// For now only support 2D textures, not cubemaps or volumes
			return false;
		}


		// Verify header to validate DDS file
		if ( DDSHeader.dwSize != sizeof( DDS_HEADER ) ||
			 DDSHeader.ddspf.dwSize != sizeof( DDS_PIXELFORMAT ) )
			 return false;

		// Skip Header and magic number, that is texture data block,
		dword DataSize = Stream->GetRemainBytes();

		// Get Texture Create Format
		D3DFORMAT Format = GetD3D9Format( DDSHeader.ddspf );

		Parameter.mSizeX = DDSHeader.dwWidth;
		Parameter.mSizeY = DDSHeader.dwHeight;
		Parameter.mPixelFormat =  (ePixelFormat)Format;

		// Min mip is 1
		dword MipCount = DDSHeader.dwMipMapCount == 0 ? 1:DDSHeader.dwMipMapCount;

		_byte* RawData = MipData.Alloc( DataSize );

		// Out of Memory
		if ( RawData == null )
			return false;

		// Read the Real DDS Texture Data
		if ( !Stream->ReadBuffer( RawData, DataSize ) )
			return false;

		dword MipWidth = DDSHeader.dwWidth, 
			MipHeight = DDSHeader.dwHeight;

		dword NumBytes;

		_byte* SourceData = RawData;

		// 将Mip的每层指针及每层的长度记录下来
		// Source = <Mip0><Mip1><Mip2><Mip3> ...
		for ( dword MipIndex = 0; MipIndex < MipCount ; MipIndex++)
		{			
			DDSCodec_GetSurfaceInfo( MipWidth, MipHeight, Format, &NumBytes, null, null);

			MipData.SetMip( MipIndex,  SourceData, NumBytes );
			SourceData += NumBytes;

			DDSCodec_NextMip( MipWidth, MipHeight );
		}


		return true;
	}

	/// 加载DDS文件，并且读取到MipMapBuffer, 文件各种参数读取到DDSTextureParameter
	bool DDSCodec_Load( const wchar* FileName, TextureMipMap& MipData, DDSTextureParameter& Parameter )
	{
		FileStream DDSFile( FileName , FAM_Read );

		if ( !DDSFile.IsValid() )
			return false;

		return DDSCodec_Load( &DDSFile, MipData, Parameter );
	}

	UINT BitsPerPixel( D3DFORMAT fmt )
	{
		UINT fmtU = ( UINT )fmt;
		switch( fmtU )
		{
		case D3DFMT_A32B32G32R32F:
			return 128;

		case D3DFMT_A16B16G16R16:
		case D3DFMT_Q16W16V16U16:
		case D3DFMT_A16B16G16R16F:
		case D3DFMT_G32R32F:
			return 64;

		case D3DFMT_A8R8G8B8:
		case D3DFMT_X8R8G8B8:
		case D3DFMT_A2B10G10R10:
		case D3DFMT_A8B8G8R8:
		case D3DFMT_X8B8G8R8:
		case D3DFMT_G16R16:
		case D3DFMT_A2R10G10B10:
		case D3DFMT_Q8W8V8U8:
		case D3DFMT_V16U16:
		case D3DFMT_X8L8V8U8:
		case D3DFMT_A2W10V10U10:
		case D3DFMT_D32:
		case D3DFMT_D24S8:
		case D3DFMT_D24X8:
		case D3DFMT_D24X4S4:
		case D3DFMT_D32F_LOCKABLE:
		case D3DFMT_D24FS8:
		case D3DFMT_INDEX32:
		case D3DFMT_G16R16F:
		case D3DFMT_R32F:
			return 32;

		case D3DFMT_R8G8B8:
			return 24;

		case D3DFMT_A4R4G4B4:
		case D3DFMT_X4R4G4B4:
		case D3DFMT_R5G6B5:
		case D3DFMT_L16:
		case D3DFMT_A8L8:
		case D3DFMT_X1R5G5B5:
		case D3DFMT_A1R5G5B5:
		case D3DFMT_A8R3G3B2:
		case D3DFMT_V8U8:
		case D3DFMT_CxV8U8:
		case D3DFMT_L6V5U5:
		case D3DFMT_G8R8_G8B8:
		case D3DFMT_R8G8_B8G8:
		case D3DFMT_D16_LOCKABLE:
		case D3DFMT_D15S1:
		case D3DFMT_D16:
		case D3DFMT_INDEX16:
		case D3DFMT_R16F:
		case D3DFMT_YUY2:
			return 16;

		case D3DFMT_R3G3B2:
		case D3DFMT_A8:
		case D3DFMT_A8P8:
		case D3DFMT_P8:
		case D3DFMT_L8:
		case D3DFMT_A4L4:
			return 8;

		case D3DFMT_DXT1:
			return 4;

		case D3DFMT_DXT2:
		case D3DFMT_DXT3:
		case D3DFMT_DXT4:
		case D3DFMT_DXT5:
			return  8;

			// From DX docs, reference/d3d/enums/d3dformat.asp
			// (note how it says that D3DFMT_R8G8_B8G8 is "A 16-bit packed RGB format analogous to UYVY (U0Y0, V0Y1, U2Y2, and so on)")
		case D3DFMT_UYVY:
			return 16;

			// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directshow/htm/directxvideoaccelerationdxvavideosubtypes.asp
		case MAKEFOURCC( 'A', 'I', '4', '4' ):
		case MAKEFOURCC( 'I', 'A', '4', '4' ):
			return 8;

		case MAKEFOURCC( 'Y', 'V', '1', '2' ):
			return 12;

//#if !defined(D3D_DISABLE_9EX)
//		case D3DFMT_D32_LOCKABLE:
//			return 32;
//
//		case D3DFMT_S8_LOCKABLE:
//			return 8;
//
//		case D3DFMT_A1:
//			return 1;
//#endif // !D3D_DISABLE_9EX

		default:
			Assert( FALSE ); // unhandled format
			return 0;
		}
	}


	void DDSCodec_GetSurfaceInfo( dword width, dword height, dword fmt, dword* pNumBytes, dword* pRowBytes, dword* pNumRows )
	{
		dword numBytes = 0;
		dword rowBytes = 0;
		dword numRows = 0;

		// From the DXSDK docs:
		//
		//     When computing DXTn compressed sizes for non-square textures, the 
		//     following formula should be used at each mipmap level:
		//
		//         max(1, width ?4) x max(1, height ?4) x 8(DXT1) or 16(DXT2-5)
		//
		//     The pitch for DXTn formats is different from what was returned in 
		//     Microsoft DirectX 7.0. It now refers the pitch of a row of blocks. 
		//     For example, if you have a width of 16, then you will have a pitch 
		//     of four blocks (4*8 for DXT1, 4*16 for DXT2-5.)"

		if( fmt == D3DFMT_DXT1 || fmt == D3DFMT_DXT2 || fmt == D3DFMT_DXT3 || fmt == D3DFMT_DXT4 || fmt == D3DFMT_DXT5 )
		{
			int numBlocksWide = 0;
			if( width > 0 )
				numBlocksWide = max( 1, width / 4 );
			int numBlocksHigh = 0;
			if( height > 0 )
				numBlocksHigh = max( 1, height / 4 );
			int numBytesPerBlock = ( fmt == D3DFMT_DXT1 ? 8 : 16 );
			rowBytes = numBlocksWide * numBytesPerBlock;
			numRows = numBlocksHigh;
		}
		else
		{
			dword bpp = BitsPerPixel( (D3DFORMAT)fmt );
			rowBytes = ( width * bpp + 7 ) / 8; // round up to nearest byte
			numRows = height;
		}
		numBytes = rowBytes * numRows;
		if( pNumBytes != NULL )
			*pNumBytes = numBytes;
		if( pRowBytes != NULL )
			*pRowBytes = rowBytes;
		if( pNumRows != NULL )
			*pNumRows = numRows;
	}

	void DDSCodec_NextMip( dword& width, dword& height )
	{
		width = width >> 1;
		height = height >> 1;
		if( width == 0 )
			width = 1;
		if( height == 0 )
			height = 1;
	}

	static UINT BitsPerPixel( DXGI_FORMAT fmt )
	{
		switch( fmt )
		{
		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
		case DXGI_FORMAT_R32G32B32A32_UINT:
		case DXGI_FORMAT_R32G32B32A32_SINT:
			return 128;

		case DXGI_FORMAT_R32G32B32_TYPELESS:
		case DXGI_FORMAT_R32G32B32_FLOAT:
		case DXGI_FORMAT_R32G32B32_UINT:
		case DXGI_FORMAT_R32G32B32_SINT:
			return 96;

		case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
		case DXGI_FORMAT_R16G16B16A16_UNORM:
		case DXGI_FORMAT_R16G16B16A16_UINT:
		case DXGI_FORMAT_R16G16B16A16_SNORM:
		case DXGI_FORMAT_R16G16B16A16_SINT:
		case DXGI_FORMAT_R32G32_TYPELESS:
		case DXGI_FORMAT_R32G32_FLOAT:
		case DXGI_FORMAT_R32G32_UINT:
		case DXGI_FORMAT_R32G32_SINT:
		case DXGI_FORMAT_R32G8X24_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			return 64;

		case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		case DXGI_FORMAT_R10G10B10A2_UNORM:
		case DXGI_FORMAT_R10G10B10A2_UINT:
		case DXGI_FORMAT_R11G11B10_FLOAT:
		case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R8G8B8A8_UINT:
		case DXGI_FORMAT_R8G8B8A8_SNORM:
		case DXGI_FORMAT_R8G8B8A8_SINT:
		case DXGI_FORMAT_R16G16_TYPELESS:
		case DXGI_FORMAT_R16G16_FLOAT:
		case DXGI_FORMAT_R16G16_UNORM:
		case DXGI_FORMAT_R16G16_UINT:
		case DXGI_FORMAT_R16G16_SNORM:
		case DXGI_FORMAT_R16G16_SINT:
		case DXGI_FORMAT_R32_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R32_UINT:
		case DXGI_FORMAT_R32_SINT:
		case DXGI_FORMAT_R24G8_TYPELESS:
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		case DXGI_FORMAT_R8G8_B8G8_UNORM:
		case DXGI_FORMAT_G8R8_G8B8_UNORM:
		case DXGI_FORMAT_B8G8R8A8_UNORM:
		case DXGI_FORMAT_B8G8R8X8_UNORM:
		case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
		case DXGI_FORMAT_B8G8R8A8_TYPELESS:
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		case DXGI_FORMAT_B8G8R8X8_TYPELESS:
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			return 32;

		case DXGI_FORMAT_R8G8_TYPELESS:
		case DXGI_FORMAT_R8G8_UNORM:
		case DXGI_FORMAT_R8G8_UINT:
		case DXGI_FORMAT_R8G8_SNORM:
		case DXGI_FORMAT_R8G8_SINT:
		case DXGI_FORMAT_R16_TYPELESS:
		case DXGI_FORMAT_R16_FLOAT:
		case DXGI_FORMAT_D16_UNORM:
		case DXGI_FORMAT_R16_UNORM:
		case DXGI_FORMAT_R16_UINT:
		case DXGI_FORMAT_R16_SNORM:
		case DXGI_FORMAT_R16_SINT:
		case DXGI_FORMAT_B5G6R5_UNORM:
		case DXGI_FORMAT_B5G5R5A1_UNORM:
			return 16;

		case DXGI_FORMAT_R8_TYPELESS:
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_R8_UINT:
		case DXGI_FORMAT_R8_SNORM:
		case DXGI_FORMAT_R8_SINT:
		case DXGI_FORMAT_A8_UNORM:
			return 8;

		case DXGI_FORMAT_R1_UNORM:
			return 1;

		case DXGI_FORMAT_BC1_TYPELESS:
		case DXGI_FORMAT_BC1_UNORM:
		case DXGI_FORMAT_BC1_UNORM_SRGB:
			return 4;

		case DXGI_FORMAT_BC2_TYPELESS:
		case DXGI_FORMAT_BC2_UNORM:
		case DXGI_FORMAT_BC2_UNORM_SRGB:
		case DXGI_FORMAT_BC3_TYPELESS:
		case DXGI_FORMAT_BC3_UNORM:
		case DXGI_FORMAT_BC3_UNORM_SRGB:
		case DXGI_FORMAT_BC4_TYPELESS:
		case DXGI_FORMAT_BC4_UNORM:
		case DXGI_FORMAT_BC4_SNORM:
		case DXGI_FORMAT_BC5_TYPELESS:
		case DXGI_FORMAT_BC5_UNORM:
		case DXGI_FORMAT_BC5_SNORM:
		case DXGI_FORMAT_BC6H_TYPELESS:
		case DXGI_FORMAT_BC6H_UF16:
		case DXGI_FORMAT_BC6H_SF16:
		case DXGI_FORMAT_BC7_TYPELESS:
		case DXGI_FORMAT_BC7_UNORM:
		case DXGI_FORMAT_BC7_UNORM_SRGB:
			return 8;

		default:
			Assert( FALSE ); // unhandled format
			return 0;
		}
	}




	static void GetSurfaceInfo( UINT width, UINT height, DXGI_FORMAT fmt, UINT* pNumBytes, UINT* pRowBytes, UINT* pNumRows )
	{
		UINT numBytes = 0;
		UINT rowBytes = 0;
		UINT numRows = 0;

		bool bc = true;
		int bcnumBytesPerBlock = 16;
		switch (fmt)
		{
		case DXGI_FORMAT_BC1_TYPELESS:
		case DXGI_FORMAT_BC1_UNORM:
		case DXGI_FORMAT_BC1_UNORM_SRGB:
			bcnumBytesPerBlock = 8;
			break;

		case DXGI_FORMAT_BC2_TYPELESS:
		case DXGI_FORMAT_BC2_UNORM:
		case DXGI_FORMAT_BC2_UNORM_SRGB:
		case DXGI_FORMAT_BC3_TYPELESS:
		case DXGI_FORMAT_BC3_UNORM:
		case DXGI_FORMAT_BC3_UNORM_SRGB:
		case DXGI_FORMAT_BC4_TYPELESS:
		case DXGI_FORMAT_BC4_UNORM:
		case DXGI_FORMAT_BC4_SNORM:
		case DXGI_FORMAT_BC5_TYPELESS:
		case DXGI_FORMAT_BC5_UNORM:
		case DXGI_FORMAT_BC5_SNORM:
		case DXGI_FORMAT_BC6H_TYPELESS:
		case DXGI_FORMAT_BC6H_UF16:
		case DXGI_FORMAT_BC6H_SF16:
		case DXGI_FORMAT_BC7_TYPELESS:
		case DXGI_FORMAT_BC7_UNORM:
		case DXGI_FORMAT_BC7_UNORM_SRGB:
			break;

		default:
			bc = false;
			break;
		}

		if( bc )
		{
			int numBlocksWide = 0;
			if( width > 0 )
				numBlocksWide = max( 1, width / 4 );
			int numBlocksHigh = 0;
			if( height > 0 )
				numBlocksHigh = max( 1, height / 4 );
			rowBytes = numBlocksWide * bcnumBytesPerBlock;
			numRows = numBlocksHigh;
		}
		else
		{
			UINT bpp = BitsPerPixel( fmt );
			rowBytes = ( width * bpp + 7 ) / 8; // round up to nearest byte
			numRows = height;
		}
		numBytes = rowBytes * numRows;
		if( pNumBytes != NULL )
			*pNumBytes = numBytes;
		if( pRowBytes != NULL )
			*pRowBytes = rowBytes;
		if( pNumRows != NULL )
			*pNumRows = numRows;
	}

	//--------------------------------------------------------------------------------------
#define ISBITMASK( r,g,b,a ) ( ddpf.dwRBitMask == r && ddpf.dwGBitMask == g && ddpf.dwBBitMask == b && ddpf.dwABitMask == a )

	//--------------------------------------------------------------------------------------
	D3DFORMAT GetD3D9Format( const DDS_PIXELFORMAT& ddpf )
	{
		if( ddpf.dwFlags & DDS_RGB )
		{
			switch (ddpf.dwRGBBitCount)
			{
			case 32:
				if( ISBITMASK(0x00ff0000,0x0000ff00,0x000000ff,0xff000000) )
					return D3DFMT_A8R8G8B8;
				if( ISBITMASK(0x00ff0000,0x0000ff00,0x000000ff,0x00000000) )
					return D3DFMT_X8R8G8B8;
				if( ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0xff000000) )
					return D3DFMT_A8B8G8R8;
				if( ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0x00000000) )
					return D3DFMT_X8B8G8R8;
				if( ISBITMASK(0x3ff00000,0x000ffc00,0x000003ff,0xc0000000) )
					return D3DFMT_A2R10G10B10;
				if( ISBITMASK(0x000003ff,0x000ffc00,0x3ff00000,0xc0000000) )
					return D3DFMT_A2B10G10R10;
				if( ISBITMASK(0x0000ffff,0xffff0000,0x00000000,0x00000000) )
					return D3DFMT_G16R16;
				if( ISBITMASK(0xffffffff,0x00000000,0x00000000,0x00000000) )
					return D3DFMT_R32F; // D3DX writes this out as a FourCC of 114
				break;

			case 24:
				if( ISBITMASK(0x00ff0000,0x0000ff00,0x000000ff,0x00000000) )
					return D3DFMT_R8G8B8;
				break;

			case 16:
				if( ISBITMASK(0x0000f800,0x000007e0,0x0000001f,0x00000000) )
					return D3DFMT_R5G6B5;
				if( ISBITMASK(0x00007c00,0x000003e0,0x0000001f,0x00008000) )
					return D3DFMT_A1R5G5B5;
				if( ISBITMASK(0x00007c00,0x000003e0,0x0000001f,0x00000000) )
					return D3DFMT_X1R5G5B5;
				if( ISBITMASK(0x00000f00,0x000000f0,0x0000000f,0x0000f000) )
					return D3DFMT_A4R4G4B4;
				if( ISBITMASK(0x00000f00,0x000000f0,0x0000000f,0x00000000) )
					return D3DFMT_X4R4G4B4;
				if( ISBITMASK(0x000000e0,0x0000001c,0x00000003,0x0000ff00) )
					return D3DFMT_A8R3G3B2;
				break;
			}
		}
		else if( ddpf.dwFlags & DDS_LUMINANCE )
		{
			if( 8 == ddpf.dwRGBBitCount )
			{
				if( ISBITMASK(0x0000000f,0x00000000,0x00000000,0x000000f0) )
					return D3DFMT_A4L4;
				if( ISBITMASK(0x000000ff,0x00000000,0x00000000,0x00000000) )
					return D3DFMT_L8;
			}

			if( 16 == ddpf.dwRGBBitCount )
			{
				if( ISBITMASK(0x0000ffff,0x00000000,0x00000000,0x00000000) )
					return D3DFMT_L16;
				if( ISBITMASK(0x000000ff,0x00000000,0x00000000,0x0000ff00) )
					return D3DFMT_A8L8;
			}
		}
		else if( ddpf.dwFlags & DDS_ALPHA )
		{
			if( 8 == ddpf.dwRGBBitCount )
			{
				return D3DFMT_A8;
			}
		}
		else if( ddpf.dwFlags & DDS_FOURCC )
		{
			if( MAKEFOURCC( 'D', 'X', 'T', '1' ) == ddpf.dwFourCC )
				return D3DFMT_DXT1;
			if( MAKEFOURCC( 'D', 'X', 'T', '2' ) == ddpf.dwFourCC )
				return D3DFMT_DXT2;
			if( MAKEFOURCC( 'D', 'X', 'T', '3' ) == ddpf.dwFourCC )
				return D3DFMT_DXT3;
			if( MAKEFOURCC( 'D', 'X', 'T', '4' ) == ddpf.dwFourCC )
				return D3DFMT_DXT4;
			if( MAKEFOURCC( 'D', 'X', 'T', '5' ) == ddpf.dwFourCC )
				return D3DFMT_DXT5;

			if( MAKEFOURCC( 'R', 'G', 'B', 'G' ) == ddpf.dwFourCC )
				return D3DFMT_R8G8_B8G8;
			if( MAKEFOURCC( 'G', 'R', 'G', 'B' ) == ddpf.dwFourCC )
				return D3DFMT_G8R8_G8B8;

			if( MAKEFOURCC( 'U', 'Y', 'V', 'Y' ) == ddpf.dwFourCC )
				return D3DFMT_UYVY;
			if( MAKEFOURCC( 'Y', 'U', 'Y', '2' ) == ddpf.dwFourCC )
				return D3DFMT_YUY2;

			// Check for D3DFORMAT enums being set here
			switch( ddpf.dwFourCC )
			{
			case D3DFMT_A16B16G16R16:
			case D3DFMT_Q16W16V16U16:
			case D3DFMT_R16F:
			case D3DFMT_G16R16F:
			case D3DFMT_A16B16G16R16F:
			case D3DFMT_R32F:
			case D3DFMT_G32R32F:
			case D3DFMT_A32B32G32R32F:
			case D3DFMT_CxV8U8:
				return (D3DFORMAT)ddpf.dwFourCC;
			}
		}

		return D3DFMT_UNKNOWN;
	}




	//static HRESULT CreateTextureFromDDS( LPDIRECT3DDEVICE9 pDev, DDS_HEADER* pHeader, __inout_bcount(BitSize) BYTE* pBitData, UINT BitSize,
	//	__out LPDIRECT3DTEXTURE9* ppTex )
	//{
	//	HRESULT hr = S_OK;
	//	D3DLOCKED_RECT LockedRect;

	//	UINT iWidth = pHeader->dwWidth;
	//	UINT iHeight = pHeader->dwHeight;
	//	UINT iMipCount = pHeader->dwMipMapCount;
	//	if( 0 == iMipCount )
	//		iMipCount = 1;

	//	if (pHeader->dwCubemapFlags != 0
	//		|| (pHeader->dwHeaderFlags & DDS_HEADER_FLAGS_VOLUME) )
	//	{
	//		// For now only support 2D textures, not cubemaps or volumes
	//		return E_FAIL;
	//	}

	//	D3DFORMAT fmt = GetD3D9Format( pHeader->ddspf );

	//	// Create the texture
	//	LPDIRECT3DTEXTURE9 pTexture;
	//	LPDIRECT3DTEXTURE9 pStagingTexture;
	//	hr = pDev->CreateTexture( iWidth,
	//		iHeight,
	//		iMipCount,
	//		0, // usage
	//		fmt,
	//		D3DPOOL_DEFAULT,
	//		&pTexture,
	//		NULL );
	//	if( FAILED( hr ) )
	//		return hr;

	//	hr = pDev->CreateTexture( iWidth,
	//		iHeight,
	//		iMipCount,
	//		0, // usage
	//		fmt,
	//		D3DPOOL_SYSTEMMEM,
	//		&pStagingTexture,
	//		NULL );
	//	if( FAILED( hr ) )
	//	{
	//		SafeRelease( pTexture );
	//		return hr;
	//	}

	//	// Lock, fill, unlock
	//	UINT RowBytes, NumRows;
	//	BYTE* pSrcBits = pBitData;

	//	for( UINT i = 0; i < iMipCount; i++ )
	//	{
	//		GetSurfaceInfo( iWidth, iHeight, fmt, NULL, &RowBytes, &NumRows );

	//			if( SUCCEEDED( pStagingTexture->LockRect( i, &LockedRect, NULL, 0 ) ) )
	//		{
	//			BYTE* pDestBits = ( BYTE* )LockedRect.pBits;

	//			// Copy stride line by line
	//			for( UINT h = 0; h < NumRows; h++ )
	//			{
	//				CopyMemory( pDestBits, pSrcBits, RowBytes );
	//				pDestBits += LockedRect.Pitch;
	//				pSrcBits += RowBytes;
	//			}

	//			pStagingTexture->UnlockRect( i );
	//		}

	//		iWidth = iWidth >> 1;
	//		iHeight = iHeight >> 1;
	//		if( iWidth == 0 )
	//			iWidth = 1;
	//		if( iHeight == 0 )
	//			iHeight = 1;
	//	}

	//	hr = pDev->UpdateTexture( pStagingTexture, pTexture );
	//	SafeRelease( pStagingTexture );
	//	if( FAILED( hr ) )
	//		return hr;

	//	// Set the result
	//	*ppTex = pTexture;
	//	return hr;
	//}


	/*HRESULT CreateDDSTextureFromFile( LPDIRECT3DDEVICE9 pDev, WCHAR* szFileName, LPDIRECT3DTEXTURE9* ppTex )
	{
		BYTE* pHeapData = NULL;
		DDS_HEADER* pHeader= NULL;
		BYTE* pBitData = NULL;
		UINT BitSize = 0;

		HRESULT hr = LoadTextureDataFromFile( szFileName, &pHeapData, &pHeader, &pBitData, &BitSize );
		if(FAILED(hr))
		{
			SafeDeleteArray( pHeapData );
			return hr;
		}

		hr = CreateTextureFromDDS( pDev, pHeader, pBitData, BitSize, ppTex );
		SafeDeleteArray( pHeapData );
		return hr;
	}*/
}