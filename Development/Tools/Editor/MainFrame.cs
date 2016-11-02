using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using System.IO;
using System.Reflection;
using System.Diagnostics;
using Wrapper;


namespace Editor
{
    //class RTTIInstance<TRTTIInfo>
    //{

    //}

    public partial class MainFrame : Form
    {
		private static MainFrame mInstance;

        private Wrapper.GameEngine mEngine;

		const String LAYOUT_FILE = @"..\Config\DefaultLayout.xml";

		ViewWindow mViewwindow;
        PropertyWindow mPropertyWindow;

        public Wrapper.GameEngine EngineAPI
        {
            get{ return mEngine; }
        }

		public static MainFrame Instance( )
		{
			return mInstance;
		}

        public MainFrame()
        {
			mInstance = this;

            InitializeComponent();
        }

		private IDockContent LayoutLoadCallback( string PersistString )
		{
			Assembly assembly = Assembly.GetExecutingAssembly();
			IDockContent obj = (IDockContent)assembly.CreateInstance(PersistString);
			if (obj != null)
			{
				if (obj.GetType() == typeof(ViewWindow))
				{
					mViewwindow = (ViewWindow)obj;
				}
                else if ( obj.GetType() == typeof(PropertyWindow))
                {
                    mPropertyWindow = (PropertyWindow)obj;
                    
                }

                 

				return obj;
			}


			return null;
		}

        private void RestoreDefaultLayout( )
        {
            AssertViewer Window1 = new AssertViewer();
            Window1.Show( MainDockPanel );

            Hierarchy Window2 = new Hierarchy();
            Window2.Show(MainDockPanel);

            PreviewWindow Window3 = new PreviewWindow();
            Window3.Show(MainDockPanel);

            PropertyWindow Window4 = new PropertyWindow();
            Window4.Show(MainDockPanel);

            mViewwindow = new ViewWindow();
            mViewwindow.Show(MainDockPanel);
        }

        private void MainFrame_Load(object sender, EventArgs e)
        {


			mEngine = new Wrapper.GameEngine();

            MainDockPanel.DocumentStyle = DocumentStyle.DockingWindow;

            if (!File.Exists(LAYOUT_FILE))
            {
                RestoreDefaultLayout();
            }
            else
            {
                MainDockPanel.LoadFromXml(LAYOUT_FILE, new DeserializeDockContent(LayoutLoadCallback));
            }

	
			mEngine.InitializeEngine((int)mViewwindow.Handle);


            mPropertyWindow.Initialize();
        }

        public void Application_Idle( object sender, EventArgs e )
        {
			if (mEngine != null )
				mEngine.Update();
        }

        private void MainFrame_FormClosed(object sender, FormClosedEventArgs e)
        {
			if (mEngine != null)
			{
				mEngine.FinalizeEngine();
				mEngine.Dispose();
			}
        }

		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Close();
		}

		private void saveLayoutToolStripMenuItem_Click(object sender, EventArgs e)
		{
			MainDockPanel.SaveAsXml(LAYOUT_FILE);
		}

		private void MainDockPanel_ActiveContentChanged(object sender, EventArgs e)
		{

		}
    }
}
