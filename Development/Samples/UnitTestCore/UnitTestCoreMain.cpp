#include "UnitTestCorePCH.h"



extern void PropertyTest( );

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	NEKO_CORE_INIT

	Application::SetStartupPathByExecutable();

	PropertyTest( );
 
	return TRUE;
}