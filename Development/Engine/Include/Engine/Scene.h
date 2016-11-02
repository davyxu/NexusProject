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
		场景侦听器
	*/
	class SceneListener : public RTTIObject
	{
	public:
		DECLARE_RTTI_CLASS( SceneListener, RTTIObject, ClassFlag_Intrinsic )

		virtual ~SceneListener( ){ }

		virtual void OnCreate( ){}

		virtual void OnDestroy( ){}

		virtual void OnEnter( ){}

		virtual void OnLeave( ){}

		virtual void OnUpdate( float elapse ){}
	};

	
	/** 
		创建管理
	*/
	class SceneMaster
	{
	public:
		SceneMaster( );

		virtual ~SceneMaster( );

		void Create(  );

		void Destroy( );

		void Update( float elapse );

		void Goto( const NameRef& SceneClassName );

		SceneListener* Get( const NameRef& SceneClassName );

		void Release( );

		void GotoNext( );

		void GotoPrev( );

		const wchar* GetCurrentSceneName( );

	private:
		typedef std::map<NameRef,SceneListener*> SceneMap;
		SceneMap			mSceneMap;
		SceneMap::iterator	mIterator;
		SceneListener*		mCurrent;
		SceneListener*		mNextGoto;
		SimpleLog			mLogger;
	};

	extern SceneMaster*		GSceneMaster;

	/** @} */
	/** @} */
};