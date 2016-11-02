#pragma once

 
namespace NekoEngine
{
	class D3D9VideoDevice : public RHIVideoDevice
	{
	public:
		D3D9VideoDevice(void* InWindowHandle, dword Width, dword Height, bool bInIsFullscreen);

		virtual ~D3D9VideoDevice();

		virtual void Release( );

		virtual void Resize( dword Width, dword Height, bool bInIsFullscreen, void* NewWindowHandle );

		virtual void Present( );

		// Accessors.
		virtual handle GetWindowHandle() { return mWindowHandle; }

		virtual dword GetWidth( ){ return mWidth; }

		virtual dword GetHeight( ){ return mHeight; }

		virtual void UpdateDevice( );

		bool IsFullscreen() const { return mIsFullscreen; }

		

	private:
		void* mWindowHandle;
		dword mWidth,mHeight;
		bool mIsFullscreen;
	};

	typedef std::vector<D3D9VideoDevice*> D3DViewportArray;

	extern D3DViewportArray GD3DViewportArray;

	
}