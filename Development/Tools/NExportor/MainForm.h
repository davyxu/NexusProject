#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Managed {

	/// <summary>
	/// Summary for MainForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  mBtnExport;
	private: System::Windows::Forms::Button^  mBtnExit;



	private: System::Windows::Forms::TextBox^  mTxtModelName;
















	private: System::Windows::Forms::GroupBox^  groupBox5;











	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::ListBox^  listBox2;










	private: System::Windows::Forms::GroupBox^  groupBox4;
	private: System::Windows::Forms::TextBox^  mTxtExportDesc;


	private: System::Windows::Forms::CheckBox^  mChkExportAnimation;
	private: System::Windows::Forms::CheckBox^  mChkExportSkin;




	private: System::Windows::Forms::GroupBox^  mAnimationProcessGroup;

	private: System::Windows::Forms::TextBox^  mTxtSampingRate;

	private: System::Windows::Forms::Label^  label2;

	private: System::Windows::Forms::ComboBox^  mCboAniMethod;
	private: System::Windows::Forms::CheckBox^  mChkExportTangent;

	private: System::Windows::Forms::CheckBox^  mChkExportMaterial;



















	protected: 

	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->mBtnExport = (gcnew System::Windows::Forms::Button());
			this->mBtnExit = (gcnew System::Windows::Forms::Button());
			this->mTxtModelName = (gcnew System::Windows::Forms::TextBox());
			this->mChkExportTangent = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->mChkExportMaterial = (gcnew System::Windows::Forms::CheckBox());
			this->mChkExportAnimation = (gcnew System::Windows::Forms::CheckBox());
			this->mChkExportSkin = (gcnew System::Windows::Forms::CheckBox());
			this->mTxtExportDesc = (gcnew System::Windows::Forms::TextBox());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->listBox2 = (gcnew System::Windows::Forms::ListBox());
			this->mAnimationProcessGroup = (gcnew System::Windows::Forms::GroupBox());
			this->mTxtSampingRate = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->mCboAniMethod = (gcnew System::Windows::Forms::ComboBox());
			this->groupBox5->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->mAnimationProcessGroup->SuspendLayout();
			this->SuspendLayout();
			// 
			// mBtnExport
			// 
			this->mBtnExport->Location = System::Drawing::Point(200, 217);
			this->mBtnExport->Name = L"mBtnExport";
			this->mBtnExport->Size = System::Drawing::Size(76, 23);
			this->mBtnExport->TabIndex = 0;
			this->mBtnExport->Text = L"Export";
			this->mBtnExport->UseVisualStyleBackColor = true;
			this->mBtnExport->Click += gcnew System::EventHandler(this, &MainForm::mBtnExport_Click);
			// 
			// mBtnExit
			// 
			this->mBtnExit->Location = System::Drawing::Point(283, 217);
			this->mBtnExit->Name = L"mBtnExit";
			this->mBtnExit->Size = System::Drawing::Size(76, 23);
			this->mBtnExit->TabIndex = 1;
			this->mBtnExit->Text = L"Cancel";
			this->mBtnExit->UseVisualStyleBackColor = true;
			this->mBtnExit->Click += gcnew System::EventHandler(this, &MainForm::mBtnCancel_Click);
			// 
			// mTxtModelName
			// 
			this->mTxtModelName->Location = System::Drawing::Point(6, 20);
			this->mTxtModelName->Name = L"mTxtModelName";
			this->mTxtModelName->Size = System::Drawing::Size(341, 21);
			this->mTxtModelName->TabIndex = 0;
			// 
			// mChkExportTangent
			// 
			this->mChkExportTangent->AutoSize = true;
			this->mChkExportTangent->Location = System::Drawing::Point(6, 88);
			this->mChkExportTangent->Name = L"mChkExportTangent";
			this->mChkExportTangent->Size = System::Drawing::Size(120, 16);
			this->mChkExportTangent->TabIndex = 0;
			this->mChkExportTangent->Text = L"Tangent,Binormal";
			this->mChkExportTangent->UseVisualStyleBackColor = true;
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->mChkExportTangent);
			this->groupBox5->Controls->Add(this->mChkExportMaterial);
			this->groupBox5->Controls->Add(this->mChkExportAnimation);
			this->groupBox5->Controls->Add(this->mChkExportSkin);
			this->groupBox5->Location = System::Drawing::Point(12, 83);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(182, 112);
			this->groupBox5->TabIndex = 4;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Export Options";
			// 
			// mChkExportMaterial
			// 
			this->mChkExportMaterial->AutoSize = true;
			this->mChkExportMaterial->Location = System::Drawing::Point(6, 66);
			this->mChkExportMaterial->Name = L"mChkExportMaterial";
			this->mChkExportMaterial->Size = System::Drawing::Size(72, 16);
			this->mChkExportMaterial->TabIndex = 0;
			this->mChkExportMaterial->Text = L"Material";
			this->mChkExportMaterial->UseVisualStyleBackColor = true;
			this->mChkExportMaterial->CheckedChanged += gcnew System::EventHandler(this, &MainForm::mChkExportAnimation_CheckedChanged);
			// 
			// mChkExportAnimation
			// 
			this->mChkExportAnimation->AutoSize = true;
			this->mChkExportAnimation->Location = System::Drawing::Point(6, 42);
			this->mChkExportAnimation->Name = L"mChkExportAnimation";
			this->mChkExportAnimation->Size = System::Drawing::Size(78, 16);
			this->mChkExportAnimation->TabIndex = 0;
			this->mChkExportAnimation->Text = L"Animation";
			this->mChkExportAnimation->UseVisualStyleBackColor = true;
			this->mChkExportAnimation->CheckedChanged += gcnew System::EventHandler(this, &MainForm::mChkExportAnimation_CheckedChanged);
			// 
			// mChkExportSkin
			// 
			this->mChkExportSkin->AutoSize = true;
			this->mChkExportSkin->Location = System::Drawing::Point(6, 20);
			this->mChkExportSkin->Name = L"mChkExportSkin";
			this->mChkExportSkin->Size = System::Drawing::Size(96, 16);
			this->mChkExportSkin->TabIndex = 0;
			this->mChkExportSkin->Text = L"SkeletonMesh";
			this->mChkExportSkin->UseVisualStyleBackColor = true;
			// 
			// mTxtExportDesc
			// 
			this->mTxtExportDesc->Location = System::Drawing::Point(12, 219);
			this->mTxtExportDesc->Multiline = true;
			this->mTxtExportDesc->Name = L"mTxtExportDesc";
			this->mTxtExportDesc->ReadOnly = true;
			this->mTxtExportDesc->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->mTxtExportDesc->Size = System::Drawing::Size(182, 68);
			this->mTxtExportDesc->TabIndex = 3;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->mTxtModelName);
			this->groupBox4->Location = System::Drawing::Point(12, 12);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(355, 52);
			this->groupBox4->TabIndex = 2;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Name";
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->BackColor = System::Drawing::SystemColors::Window;
			this->checkBox2->Location = System::Drawing::Point(10, 0);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(120, 16);
			this->checkBox2->TabIndex = 9;
			this->checkBox2->Text = L"Export Mesh/Skin";
			this->checkBox2->UseVisualStyleBackColor = false;
			// 
			// listBox2
			// 
			this->listBox2->FormattingEnabled = true;
			this->listBox2->ItemHeight = 12;
			this->listBox2->Location = System::Drawing::Point(10, 20);
			this->listBox2->Name = L"listBox2";
			this->listBox2->Size = System::Drawing::Size(176, 196);
			this->listBox2->TabIndex = 0;
			// 
			// mAnimationProcessGroup
			// 
			this->mAnimationProcessGroup->Controls->Add(this->mTxtSampingRate);
			this->mAnimationProcessGroup->Controls->Add(this->label2);
			this->mAnimationProcessGroup->Controls->Add(this->mCboAniMethod);
			this->mAnimationProcessGroup->Location = System::Drawing::Point(200, 83);
			this->mAnimationProcessGroup->Name = L"mAnimationProcessGroup";
			this->mAnimationProcessGroup->Size = System::Drawing::Size(167, 112);
			this->mAnimationProcessGroup->TabIndex = 4;
			this->mAnimationProcessGroup->TabStop = false;
			this->mAnimationProcessGroup->Text = L"Animation Process";
			// 
			// mTxtSampingRate
			// 
			this->mTxtSampingRate->Location = System::Drawing::Point(100, 46);
			this->mTxtSampingRate->Name = L"mTxtSampingRate";
			this->mTxtSampingRate->Size = System::Drawing::Size(59, 21);
			this->mTxtSampingRate->TabIndex = 2;
			this->mTxtSampingRate->Text = L"0.1";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 49);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(89, 12);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Sampling Rate:";
			// 
			// mCboAniMethod
			// 
			this->mCboAniMethod->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->mCboAniMethod->FormattingEnabled = true;
			this->mCboAniMethod->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Sampling (Great Size)", L"Key Frame (Slow)"});
			this->mCboAniMethod->Location = System::Drawing::Point(6, 20);
			this->mCboAniMethod->Name = L"mCboAniMethod";
			this->mCboAniMethod->Size = System::Drawing::Size(153, 20);
			this->mCboAniMethod->TabIndex = 0;
			this->mCboAniMethod->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::mCboAniMethod_SelectedIndexChanged);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(384, 299);
			this->ControlBox = false;
			this->Controls->Add(this->mTxtExportDesc);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->mAnimationProcessGroup);
			this->Controls->Add(this->groupBox5);
			this->Controls->Add(this->mBtnExit);
			this->Controls->Add(this->mBtnExport);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"MainForm";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"N Export";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->mAnimationProcessGroup->ResumeLayout(false);
			this->mAnimationProcessGroup->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void mBtnExport_Click(System::Object^  sender, System::EventArgs^  e);

	private: System::Void mBtnCancel_Click(System::Object^  sender, System::EventArgs^  e);

	private: static MainForm^ sSelf = nullptr;

	public: static MainForm^ Instance( );

	private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e);
	
	private: System::Void Export( );
private: System::Void mCboAniMethod_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void mChkExportAnimation_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
};

public ref class WindowWrapper : System::Windows::Forms::IWin32Window
	{
		public: WindowWrapper(IntPtr handle)
		{
			_hwnd = handle;
		}

	public:
		property IntPtr Handle
		{
			virtual IntPtr get(void){return _hwnd;};
		};

		private: IntPtr _hwnd;
	};

}
