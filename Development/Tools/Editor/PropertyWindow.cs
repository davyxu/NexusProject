using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using System.Diagnostics;
using Wrapper;

namespace Editor
{
    public partial class PropertyWindow : DockContent
    {
        Actor mA;
        public PropertyWindow( )
        {
            InitializeComponent();
        }

        private void Property_Load(object sender, EventArgs e)
        {
            
        }

        public void Initialize( )
        {
            mA = GameEngine.Instance().CreateActor();
            propertyGrid1.SelectedObject = ReflectorManager.Instance().GetPropertyObject( mA );
        }

        //private void PropertyWindow_Resize(object sender, EventArgs e)
        //{

        //}

    }
}
