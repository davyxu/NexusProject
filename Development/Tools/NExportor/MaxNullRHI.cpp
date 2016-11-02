#include "ExportPCH.h"
#include "PluginInclude.h"

namespace NekoEngine
{
	IRenderHardware* RHI = null;
	RenderStatus GRenderStatus;

	void RHIInitialize( )
	{

	}

	void RHIFinalize( )
	{

	}

	IInputDriver* CreateDInput( )
	{
		return null;
	}

	bool DDSCodec_Load( const wchar* FileName, NekoEngine::TextureMipMap& MipData, DDSTextureParameter& Parameter )
	{
		return false;
	}

	bool DDSCodec_Load( DataStream* Stream, NekoEngine::TextureMipMap& MipData, DDSTextureParameter& Parameter )
	{
		return false;
	}
}
