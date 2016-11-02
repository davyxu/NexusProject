#pragma once

namespace NekoEngine
{
	inline dword FLOAT_TO_DWORD(float v)
	{
		union
		{
			float f;
			dword u;
		} fnu;
		fnu.f = v;
		return fnu.u;
	}

	inline float DWORD_TO_FLOAT(dword v)
	{
		union
		{
			dword u;
			float f;
		} unf;
		unf.u = v;
		return unf.f;
	}

	extern Logger* D3D9Logger;

	extern IDirect3DDevice9* GDirect3DDevice;

	extern bool GD3DDeviceLost;

	extern bool GD3DIsFullscreenDevice;

	extern IDirect3DSurface9* GD3DBackBuffer;

	extern void UpdateDevice_RenderThread();

	extern IDirect3DSurface9* GRTSurfaceCopy;	
}

// ¼ì²éD3D·µ»ØÖµ
#define VALID_D3D_LOG( d3dcode ) \
{HRESULT hr = d3dcode; if ( FAILED(hr) )\
{\
	const wchar* file = __NFILE__;\
	const int line = __NLINE__;\
	D3D9Logger->Debug( L"D3D Return failed: 0x%x\r\n	%s(%d)", hr, file, line );\
}\
}


#define VALID_D3D_BREAK( d3dcode )\
{HRESULT hr = d3dcode; if ( FAILED(hr) )\
{\
	const wchar* file = __NFILE__;\
	const int line = __NLINE__;\
	D3D9Logger->Debug( L"D3D Return failed: 0x%x\r\n	%s(%d)", hr, file, line );\
	__asm int 3 \
}\
}



#define VD3DHR( d3dcode )	VALID_D3D_BREAK( d3dcode )