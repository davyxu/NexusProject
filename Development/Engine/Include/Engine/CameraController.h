#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Render
	*  @{
	*/

	class IInputDriver;

	/** 
		摄像机控制器抽象类
	*/
	class CameraController
	{
	public:
		CameraController( );

		/** 
			挂接一个输入设备
		*/
		void AttachInputDevice( IInputDriver* driver );

		/** 
			挂接一个摄像机
		*/
		void AttachCamera( Camera* camera );

		/** 
			更新逻辑
		*/
		virtual void Tick( float elapse ) = 0;

	protected:
		Camera* mCamera;
		IInputDriver* mInputDev;
	};

	/** 
		摄像机球形控制器
	*/
	class ArcBallCameraController : public CameraController
	{
	public:
		virtual void Tick( float elapse  );
	};


	/** 
		第一人称控制器
	*/
	class FPSCameraController : public CameraController
	{
	public:
		virtual void Tick( float elapse );
	};


	/** 
		焦点摄像机控制器
	*/
	class FocusCameraController : public CameraController
	{
	public:
		FocusCameraController( );

		virtual void Tick( float elapse );

		bool mZooming;
		float mSavedMouseY;
		Vector3 mSavedPos;
	};



	/** @} */
	/** @} */
}