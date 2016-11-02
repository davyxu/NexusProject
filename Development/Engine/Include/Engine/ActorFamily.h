#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup System
	*  @{
	*/


	class CameraController;

	/** 
		摄像机控制器组件
	*/
	class CameraControllerComponent : public GeometryComponent
	{
		DECLARE_RTTI_CLASS( CameraControllerComponent, GeometryComponent, ClassFlag_Intrinsic )
	public:
		CameraControllerComponent( );

		virtual ~CameraControllerComponent( );

		virtual void Draw( SceneView* RV );

		virtual void Tick( float Elapse );

		/** 
			设置控制器类型
		*/
		void SetControllerType( CameraController* Controller, IInputDriver* InputDrv  );

	private:
		CameraController* mController;
	};


	/** @} */
	/** @} */
}
