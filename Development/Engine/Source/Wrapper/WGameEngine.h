#pragma once

using namespace System;

namespace Wrapper
{
	//public delegate void ResourceIteratorDelegate( String^ Path );

	public ref class GameEngine : public ManualSingleton<GameEngine^>
	{
		//IMPLEMENT_SINGLETON( GameEngine )
	public:
		GameEngine( );

		~GameEngine( );


		

		void InitializeEngine( int Handle );

		void FinalizeEngine();

		void Update( );

		void ResizeWindow( int X, int Y, int NewHandle  );

		void NotifyMouseMove( int x ,int y );

		void NotifyMouseScroll( int z );

		void NotifyKeyUp( int KeyCode );

		void NotifyMouseUp( int KeyCode );

		Actor^ CreateActor( );
		
		property String^ GameEngine::ContentPath
		{
			String^ get( )
			{
				return gcnew String( NekoEngine::GResourceMaster->GetContentPath( ) );
			}

			Void set( String^ value )
			{
				NekoEngine::GResourceMaster->SetContentPath( Utility::ToCharPtr( value ) );
			}
		}

	private:
		//static GameEngine^ mInstance;

		

	internal:
		NekoEngine::GameEngine* mEngine;
		NekoEngine::RenderWindow* mWindow;
	};
}