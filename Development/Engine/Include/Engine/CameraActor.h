#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup System
	*  @{
	*/


	
	/** 
		摄像机角色
	*/
	class CameraActor : public Actor
	{
		DECLARE_RTTI_CLASS( CameraActor, Actor, ClassFlag_Intrinsic| ClassFlag_Serializable )
	public:
		CameraActor( );

		Camera* GetCamera( ){ return &mCamera; }

		virtual void Serialize( BinarySerializer& Ser );

		virtual void OnObjectTransformed( dword TransformType );
		
		static const Vector3 cCameraFront;

	private:
		friend class CameraControllerComponent;
		Camera mCamera;

		bool mUpFixed;
	};

	typedef TRefPtr<CameraActor> CameraActorPtr;

	/** 
		Actor控制器基类
	*/
	class ActorControllerComponent : public GeometryComponent
	{
		DECLARE_RTTI_CLASS( ActorControllerComponent, GeometryComponent, ClassFlag_Intrinsic )
	public:		
		ActorControllerComponent( );

		virtual void Draw( SceneView* RV );

		void SetInputDriver( IInputDriver* InputDrv ){ mInputDevice = InputDrv; }

	protected:
		IInputDriver* mInputDevice;
	};


	/** 
		Actor的FPS控制器
		@remark
			通过控制物体的旋转及位置实现FPS方式移动
	*/
	class FPSActorControllerComponent : public ActorControllerComponent
	{
		DECLARE_RTTI_CLASS( FPSActorControllerComponent, ActorControllerComponent, ClassFlag_Intrinsic )
	public:		

		virtual void Tick( float Elapse );

		/// 水平旋转
		void Yaw( float XDelta );

		/// 给定一个对象坐标空间的矢量,根据对象本身的旋转进行移动
		void Move( const Vector3& Delta );

		/// 垂直俯仰
		void Pitch( float YDelta );

		/// 将一个矢量投射到对象空间
		Vector3 VectorMapIntoObjectSpace( const Vector3& V );
	};


	/** 
		观察预览方式的Actor控制器
	*/
	class FocusActorControllerComponent : public ActorControllerComponent
	{
		DECLARE_RTTI_CLASS( FocusActorControllerComponent, ActorControllerComponent, ClassFlag_Intrinsic )
	public:		
		FocusActorControllerComponent( );

		virtual void Tick( float Elapse );

		void SetFocusPoint( const Vector3& Value );

		void Phi( float Delta );

		void Theta( float Delta );

		void UpdateObjectTransform( );

		void OnAttach();
	private:
		Vector3 mFocusTarget;
		
		float mPhi;
		float mTheta;
		float mRadius;
	};


	/** @} */
	/** @} */
}