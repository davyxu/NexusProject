// NexusExporter.cpp : Defines the entry point for the DLL application.
//

#include "ExportPCH.h"
#include "MaxExporter.h"
#include "PluginInclude.h"

#include "resource.h"

//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")


Interface* GMaxInterface = null;

#define MaxUtility_CLASS_ID	Class_ID(0x4e0ec54d, 0x7af2a31d)
HINSTANCE GInstance;

class MaxUtility : public UtilityObj 
{
public:
	void BeginEditParams(Interface *ip,IUtil *iu);

	void EndEditParams(Interface *ip,IUtil *iu);

	virtual void DeleteThis(){ }

	MaxUtility();
	~MaxUtility();		

	MaxDialogExport* mExporterDialog;
};

static MaxUtility theMaxUtility;


class MaxUtilityClassDesc : public ClassDesc2 {
public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return &theMaxUtility; }
	const TCHAR *	ClassName() { return "Neko Engine Max Exporter"; }
	SClass_ID		SuperClassID() { return UTILITY_CLASS_ID; }
	Class_ID		ClassID() { return MaxUtility_CLASS_ID; }
	const TCHAR* 	Category() { return "Game Exporter"; }

	const TCHAR*	InternalName() { return _T("MaxUtility"); }
	HINSTANCE		HInstance() { return GInstance; }
};

static MaxUtilityClassDesc MaxUtilityDesc;
ClassDesc2* GetMaxUtilityDesc() { return &MaxUtilityDesc; }






//--- MaxUtility -------------------------------------------------------
MaxUtility::MaxUtility()
: mExporterDialog( null )
{

}

MaxUtility::~MaxUtility()
{

}



void MaxUtility::BeginEditParams(Interface *ip,IUtil *iu) 
{
	GMaxInterface = ip;

	mExporterDialog = new MaxDialogExport;
}

void MaxUtility::EndEditParams(Interface *ip,IUtil *iu) 
{
	SafeDelete( mExporterDialog );
}


int controlsInit = FALSE;


BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
{
	GInstance = hinstDLL;				// Hang on to this DLL's instance handle.

	if (!controlsInit) {
		controlsInit = TRUE;
		InitCustomControls(GInstance);	// Initialize MAX's custom controls
		InitCommonControls();			// Initialize Win95 controls

		MaxCollector::StaticInit();
	}

	if ( fdwReason == DLL_PROCESS_DETACH && controlsInit )
	{
		MaxCollector::StaticExit();
	}

	return (TRUE);
}

extern "C"
{

// This function returns a string that describes the DLL and where the user
// could purchase the DLL if they don't have it.
__declspec( dllexport ) const TCHAR* LibDescription()
{
	return "N Export Desc";
}

// This function returns the number of plug-in classes this DLL
//TODO: Must change this number when adding a new class
__declspec( dllexport ) int LibNumberClasses()
{
	return 1;
}

// This function returns the number of plug-in classes this DLL
__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
	switch(i) {
case 0: return GetMaxUtilityDesc();
default: return 0;
	}
}

// This function returns a pre-defined constant indicating the version of 
// the system under which it was compiled.  It is used to allow the system
// to catch obsolete DLLs.
__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

}