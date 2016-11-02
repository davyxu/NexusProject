#include "ExportPCH.h"
#include "MainForm.h"
#include <vcclr.h>
#include "PluginInclude.h"

const wchar_t* ToUnmanagedUnicode( System::String^ str )
{
	pin_ptr<const WCHAR> nativeString1  = PtrToStringChars( str );
	return (const wchar_t*)nativeString1;
}



System::Void Managed::MainForm::MainForm_Load( System::Object^ sender, System::EventArgs^ e )
{	
	mAnimationProcessGroup->Enabled = mChkExportAnimation->Checked;
	mChkExportMaterial->Checked = true;
	mCboAniMethod->SelectedIndex = 0;

	MaxInterface& Exp = MaxInterface::Instance();

	mChkExportSkin->Checked = ( Exp.mMaxSkeleton->GetBoneNumber() > 0 );
	mChkExportAnimation->Checked = mChkExportSkin->Checked;

	mTxtModelName->Text = System::String( Exp.mModelName.c_str() ).ToString();

	MaxModel* maxmodel = MaxInterface::Instance().mMaxModel;

	mTxtExportDesc->Text = System::String::Format( "Mesh: {0}\r\n"
							"Bone: {1}\r\n"							
							, System::Int32( Exp.mMaxModel->GetMeshCount() )
							, System::Int32( Exp.mMaxSkeleton->GetBoneNumber() )
						  );
}

System::Void Managed::MainForm::Export()
{
	MaxInterface::Instance().mUseAnimationSampling = (mCboAniMethod->SelectedIndex == 0);
	MaxInterface::Instance().mAnimationSamplingRate = float::Parse( mTxtSampingRate->Text );


	MaxInterface& maxinterface = MaxInterface::Instance();

	const wchar_t* modelname = MaxInterface::Instance().mModelName.c_str();

	VertexExportOption veo;
	veo.mSkin = mChkExportSkin->Checked;
	veo.mTangent = mChkExportTangent->Checked;

	maxinterface.mMaxModel->Save( modelname, veo );


	if ( mChkExportAnimation->Checked )
	{
		maxinterface.mMaxAnimation->Save( modelname );
	}

	if ( mChkExportMaterial->Checked )
	{
		maxinterface.mMaxMaterial->Save( modelname );
	}

	Windows::Forms::MessageBox::Show( "Model export finished!","Model Export", MessageBoxButtons::OK, MessageBoxIcon::Information );
}

System::Void Managed::MainForm::mBtnExport_Click( System::Object^ sender, System::EventArgs^ e )
{
	Export();
}

System::Void Managed::MainForm::mBtnCancel_Click( System::Object^ sender, System::EventArgs^ e )
{
	this->Close( );
}

Managed::MainForm^ Managed::MainForm::Instance()
{
	if ( sSelf == nullptr )
		sSelf = gcnew MainForm( );

	return sSelf;
}

System::Void Managed::MainForm::mCboAniMethod_SelectedIndexChanged( System::Object^ sender, System::EventArgs^ e )
{
	mTxtSampingRate->Enabled = ( mCboAniMethod->SelectedIndex == 0 );
}

System::Void Managed::MainForm::mChkExportAnimation_CheckedChanged( System::Object^ sender, System::EventArgs^ e )
{
	mAnimationProcessGroup->Enabled = mChkExportAnimation->Checked;
}