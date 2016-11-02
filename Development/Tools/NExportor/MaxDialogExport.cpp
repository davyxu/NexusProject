#include "ExportPCH.h"
#include "PluginInclude.h"
#include "resource.h"

MaxDialogExport* DialogExport = null;

INT_PTR MaxDialogExport::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg) 
	{
	case WM_INITDIALOG:
		DialogExport->mWindow = hWnd;
		DialogExport->LoadConfig();
		break;

	case WM_DESTROY:
		DialogExport->SaveConfig();
		break;

	case WM_COMMAND:

		switch( LOWORD(wParam))
		{
		case IDC_SETRESOURCENAME:
			{
				DialogExport->QueryResourceName();
			}
			break;
		case IDC_BROWSER_FOLDER:
			{
				DialogExport->BrowserOutputFolder();
			}
			break;
		case IDC_EXPORT:
			{
				DialogExport->Export();
			}
			break;
		case IDC_CHK_MESH:
			{
				OnMeshChecked();


			}
			break;
		case IDC_CHK_SKIN:
			{
				OnSkinChecked();

			}
			break;
		}
		break;


	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		GMaxInterface->RollupMouseMessage(hWnd,msg,wParam,lParam); 
		break;

	default:
		return 0;
	}

	return 1;
}


MaxDialogExport::MaxDialogExport()
{
	DialogExport = this;

	mWindow = GMaxInterface->AddRollupPage(
		GInstance,
		MAKEINTRESOURCE(IDD_EXPORTOPTION),
		(DLGPROC)MaxDialogExport::MsgProc,
		"Neko Exporter",
		0);

	SetFocus( GetDlgItem( mWindow, IDC_RESOURCENAME) );

	GMaxCollector = new MaxCollector;
}

MaxDialogExport::~MaxDialogExport()
{
	SafeDelete( GMaxCollector );

	GMaxInterface->DeleteRollupPage(mWindow);
	DialogExport = null;
}

int CALLBACK BrowserCallbackProc(HWND hWnd, UINT uMsg,LPARAM lParam, LPARAM lpData) 
{
	switch(uMsg)   
	{  

	case   BFFM_INITIALIZED:   
		::SendMessage   (hWnd, BFFM_SETSELECTION, 1, lpData);
		//参数lpData：Application-defined value that was specified in the lParam member of the 
		//BROWSEINFO structure.   
		break; 

	default:   
		break;   
	}   
	return   0;   
}

void MaxDialogExport::BrowserOutputFolder( )
{
	char prevDir[MAX_PATH];
	GetDlgItemText( mWindow, IDC_OUTPUTFOLDER, prevDir, sizeof prevDir );

	BROWSEINFOA param;
	param.hwndOwner = (HWND)GMaxInterface->GetMAXHWnd();
	param.iImage = null;
	param.lParam = (LPARAM)prevDir;
	param.lpfn = BrowserCallbackProc;
	param.lpszTitle = "Select folder";
	param.pidlRoot = null;
	param.pszDisplayName = null;
	param.ulFlags = BIF_RETURNONLYFSDIRS|BIF_RETURNFSANCESTORS;
	LPITEMIDLIST pidl = SHBrowseForFolder( &param );

	if ( pidl == null )
		return;

	char buffer[MAX_PATH];
	if (!SHGetPathFromIDList( pidl, buffer) )
		return;


	SetDlgItemText( mWindow, IDC_OUTPUTFOLDER, buffer );

	if ( IsDlgButtonChecked( mWindow, IDC_NAMEREPLACE ) == BST_CHECKED )
	{	
		NekoEngine::StringConverter conv;
		SetDlgItemTextW( mWindow, IDC_SETRESOURCENAME, NekoEngine::Path::GetFileNameWithoutExtension( conv.AnsiToUnicode(buffer) ).c_str() );
	}
}




void MaxDialogExport::SaveConfig( )
{
	const char* ConfigPath = GMaxInterface->GetDir( APP_PLUGCFG_DIR );

	NekoEngine::StringConverter conv;
	NekoEngine::WString ConfigFile = NekoEngine::StringHelper::FormatString( L"%s\\NExport.ini", conv.AnsiToUnicode(ConfigPath) );
	
	NekoEngine::PropertySerializer Ser( ConfigFile.c_str(), false );

	SerializeConfig( Ser );
}

void MaxDialogExport::SerializeConfig( NekoEngine::PropertySerializer& Ser )
{
	NekoEngine::WString OutputPath;
	NekoEngine::WString ResourceName;

	bool ExportMesh = true;
	bool ExportSkin = false;
	bool ExportAnimation = false;

	bool AutoNameReplace = false;
	bool ExportSelected = false;
	

	if ( Ser.IsSaving() )
	{
		NekoEngine::StringConverter conv;

		char Path[256];
		::GetDlgItemText( mWindow, IDC_OUTPUTFOLDER, Path, sizeof Path);
		OutputPath = conv.AnsiToUnicode( Path );

		char Name[256];
		::GetDlgItemText( mWindow, IDC_SETRESOURCENAME , Name, sizeof Name);
		ResourceName = conv.AnsiToUnicode( Name );

		ExportMesh = (::IsDlgButtonChecked( mWindow, IDC_CHK_MESH ) == BST_CHECKED);
		ExportSkin = (::IsDlgButtonChecked( mWindow, IDC_CHK_SKIN ) == BST_CHECKED);
		ExportAnimation = (::IsDlgButtonChecked( mWindow, IDC_CHK_ANIM ) == BST_CHECKED);

		AutoNameReplace = (::IsDlgButtonChecked( mWindow, IDC_NAMEREPLACE ) == BST_CHECKED);
		ExportSelected = (::IsDlgButtonChecked( mWindow, IDC_EXPORTSELECTED ) == BST_CHECKED);
	}

	Ser.SerializeSection(L"Path" );
	Ser << L"OutputPath" << OutputPath
		<< L"ResourceName" << ResourceName;

	Ser.SerializeSection(L"ExportOption");
	Ser << L"ExportMesh" << ExportMesh
		<< L"ExportSkin" << ExportSkin
		<< L"ExportAnimation" << ExportAnimation;

	Ser.SerializeSection(L"Config" );
	Ser << L"AutoNameReplace" << AutoNameReplace
		<< L"ExportSelected" << ExportSelected;

	if ( Ser.IsLoading() )
	{
		NekoEngine::StringConverter conv;
		::SetDlgItemText( mWindow, IDC_OUTPUTFOLDER, conv.UnicodeToAnsi( OutputPath.c_str() ) );
		::SetDlgItemText( mWindow, IDC_SETRESOURCENAME, conv.UnicodeToAnsi( ResourceName.c_str() ) );

		::CheckDlgButton( mWindow,  IDC_CHK_MESH, ExportMesh ? BST_CHECKED:BST_UNCHECKED );
		if ( ExportMesh )
			OnMeshChecked();

		::CheckDlgButton( mWindow,  IDC_CHK_SKIN, ExportSkin ? BST_CHECKED:BST_UNCHECKED );
		if ( ExportSkin )
			OnSkinChecked();

		::CheckDlgButton( mWindow,  IDC_CHK_ANIM, ExportAnimation ? BST_CHECKED:BST_UNCHECKED );

		::CheckDlgButton( mWindow,  IDC_NAMEREPLACE, AutoNameReplace ? BST_CHECKED:BST_UNCHECKED );

		::CheckDlgButton( mWindow,  IDC_EXPORTSELECTED, ExportSelected ? BST_CHECKED:BST_UNCHECKED );
	}
}

void MaxDialogExport::LoadConfig()
{
	const char* ConfigPath = GMaxInterface->GetDir( APP_PLUGCFG_DIR );

	NekoEngine::StringConverter conv;
	NekoEngine::WString ConfigFile = NekoEngine::StringHelper::FormatString( L"%s\\NExport.ini", conv.AnsiToUnicode(ConfigPath) );

	NekoEngine::PropertySerializer Ser( ConfigFile.c_str(), true );

	SerializeConfig( Ser );
}


bool MaxDialogExport::CanSaveFile( NekoEngine::eResourceType Type, NekoEngine::WString& FinalFileName )
{
	char Path[256];
	::GetDlgItemText( mWindow, IDC_OUTPUTFOLDER, Path, sizeof Path);

	// 未设定输出路径
	if ( strcmp( Path, "" ) == 0 )
	{
		::MessageBoxA( GMaxInterface->GetMAXHWnd(), "Please select the output path", "Require output path!", MB_OK|MB_ICONINFORMATION );
		return false;
	}


	char Name[256];
	::GetDlgItemText( mWindow, IDC_SETRESOURCENAME , Name, sizeof Name);

	// 未设定资源名
	if ( strcmp( Name, "" ) == 0 )
	{
		MessageBoxA( GMaxInterface->GetMAXHWnd(), "Please enter the resource name", "Require resource name!", MB_OK|MB_ICONINFORMATION );
		return false;
	}

	NekoEngine::StringConverter conv;

	// 合成资源名
	FinalFileName = NekoEngine::StringHelper::FormatString( L"%s\\%s%s", 
		conv.AnsiToUnicode( Path), 
		conv.AnsiToUnicode( Name ),
		NekoEngine::ResourceLocation::GetExtension( Type ).c_str());


	// 文件已经存在
	if ( NekoEngine::File::Exists( FinalFileName.c_str() ) )
	{
		wchar_t Buffer[256];
		NekoEngine::StringHelper::FormatBuffer( Buffer, sizeof Buffer, L"File %s already exist, overwrite ?", FinalFileName.c_str() );
		INT ret = MessageBoxW( GMaxInterface->GetMAXHWnd(), Buffer , L"File Exist", MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1 );
		if ( ret == IDYES )
		{
			return true;
		}
		
		return false;		
	}
	
	return true;
}

void MaxDialogExport::Export( )
{
	bool ExportSelected = ::IsDlgButtonChecked( mWindow, IDC_EXPORTSELECTED ) == BST_CHECKED ;
	GMaxCollector->CollectData( ExportSelected );

	NekoEngine::dword TotalExportedMesh = 0;
	NekoEngine::dword TotalExportedBone = 0;
	float TotalExportedAnimDuration = 0;
	NekoEngine::dword TotalExportedMaterial = 0;

	// 导出模型方式
	if ( ::IsDlgButtonChecked( mWindow, IDC_CHK_MESH ) == BST_CHECKED )
	{
		TotalExportedMesh = GMaxCollector->mMaxModel->GetMeshCount();
		NekoEngine::WString FinalFileName;
		if ( CanSaveFile(NekoEngine::RFT_StaticMesh, FinalFileName) )
		{
			VertexExportOption Veo;
			GMaxCollector->mMaxModel->Save( FinalFileName.c_str(), Veo );
		}
		else
		{
			TotalExportedMesh = 0;
		}
	}

	// 导出蒙皮方式
	if ( IsDlgButtonChecked( mWindow, IDC_CHK_SKIN ) == BST_CHECKED )
	{
		TotalExportedMesh = GMaxCollector->mMaxModel->GetMeshCount();

		if ( GMaxCollector->mMaxSkeleton->GetBoneNumber() == 0 )
		{
			::MessageBoxA( GMaxInterface->GetMAXHWnd(), "Can not found any bone in skin !", "Export Skin Failed", MB_OK|MB_ICONINFORMATION );
			return;
		}

		TotalExportedBone = GMaxCollector->mMaxSkeleton->GetBoneNumber();

		VertexExportOption Veo;
		Veo.mSkin = true;

		NekoEngine::WString FinalFileName;
		if ( CanSaveFile(NekoEngine::RFT_SkeletonMesh, FinalFileName) )
		{
			GMaxCollector->mMaxModel->Save( FinalFileName.c_str(), Veo );
		}
		else
		{
			TotalExportedMesh = 0;
			TotalExportedBone = 0;
		}
	}

	// 导出动画
	if ( IsDlgButtonChecked( mWindow, IDC_CHK_ANIM ) == BST_CHECKED )
	{
		SkeletonAnimationExportOption Aeo;

		if ( Aeo.mUseSampling )
		{
			GMaxCollector->mMaxSkeletonAnimation->BuildTrackBySampling( Aeo.mSamplingRate );
		}
		else
		{
			GMaxCollector->mMaxSkeletonAnimation->BuildTrackByKeyFrame();
		}

		if ( GMaxCollector->mMaxSkeletonAnimation->mSkeletonAnimationResource.GetTrackCount() == 0 )
		{
			::MessageBoxA( GMaxInterface->GetMAXHWnd(), "Can not found any animation track!", "Export Animation Failed", MB_OK|MB_ICONINFORMATION );
			return;
		}

		TotalExportedAnimDuration = GMaxCollector->mMaxSkeletonAnimation->mSkeletonAnimationResource.GetDuration();

		NekoEngine::WString FinalFileName;
		if ( CanSaveFile(NekoEngine::RFT_SkeletonAnimation, FinalFileName) )
		{
			GMaxCollector->mMaxSkeletonAnimation->Save( FinalFileName.c_str() );
		}
		else
		{
			TotalExportedAnimDuration = 0.0f;
		}
	}


	NekoEngine::AString Msg = NekoEngine::StringHelperA::FormatString( "Total export Mesh/Skin %d\r\nBone: %d\r\nAnimation duration: %fs"
																		, TotalExportedMesh
																		, TotalExportedBone
																		, TotalExportedAnimDuration );

	::MessageBoxA( GMaxInterface->GetMAXHWnd(), Msg.c_str(), "Export Result", MB_OK|MB_ICONINFORMATION );

}



INT_PTR MaxDialogExport::QueryDialogMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg) 
	{
	case WM_INITDIALOG:
		char buffer[256];
		GetDlgItemText( DialogExport->mWindow, IDC_SETRESOURCENAME, buffer, sizeof buffer );
		SetDlgItemText( hWnd, IDC_TEXT, buffer );
		
		::SendMessage(GetDlgItem( hWnd, IDC_TEXT) ,EM_SETSEL, 0 ,-1 );
		break;

	case WM_DESTROY:
		break;

	case WM_COMMAND:

		switch( LOWORD(wParam))
		{
		case IDC_QUERYNAME_OK:
			char buffer[256];
			GetDlgItemText( hWnd, IDC_TEXT, buffer, sizeof buffer );
			SetDlgItemText( DialogExport->mWindow, IDC_SETRESOURCENAME, buffer );
			::EndDialog( hWnd, 1 );
			break;
		case IDC_QUERYNAME_CANCEL:
			::EndDialog( hWnd, 0 );
			break;
		}
	default:
		return 0;
	}

	return 1;
}

void MaxDialogExport::QueryResourceName( )
{
	::DialogBoxParamA(	GInstance,
						MAKEINTRESOURCE(IDD_QUERYNAME),
						GMaxInterface->GetMAXHWnd(),
						(DLGPROC)QueryDialogMessage,(LPARAM)this);
}

void MaxDialogExport::OnMeshChecked()
{
	int state = IsDlgButtonChecked( DialogExport->mWindow, IDC_CHK_MESH );
	CheckDlgButton( DialogExport->mWindow, IDC_CHK_SKIN, !state );
	CheckDlgButton( DialogExport->mWindow, IDC_CHK_ANIM, BST_UNCHECKED );
}

void MaxDialogExport::OnSkinChecked()
{
	int state = IsDlgButtonChecked( DialogExport->mWindow, IDC_CHK_SKIN );
	CheckDlgButton( DialogExport->mWindow, IDC_CHK_MESH, !state );
	CheckDlgButton( DialogExport->mWindow, IDC_CHK_ANIM, BST_CHECKED );
}
