using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using Wrapper;

namespace Editor
{
    public partial class ViewWindow : DockContent
    {
		static ViewWindow mInstance;

		public static ViewWindow Instance()
		{
			return mInstance;
		}


        public ViewWindow( )
        {
			mInstance = this;

            InitializeComponent();
        }

        public Int32 GetRenderHandle()
        {
            return (int)this.Handle;
        }

		private void ViewWindow_Resize(object sender, EventArgs e)
		{
			Size thisSize = Size;

			if (thisSize.Width == 0 || thisSize.Width == 0)
				return;

			if (GameEngine.Instance() != null)
				GameEngine.Instance().ResizeWindow(thisSize.Width, thisSize.Height, GetRenderHandle());
		}

		protected override void OnMouseWheel(System.Windows.Forms.MouseEventArgs e)
		{
			if (GameEngine.Instance() != null)
				GameEngine.Instance().NotifyMouseScroll(e.Delta);
		} 

		private void ViewWindow_MouseMove(object sender, MouseEventArgs e)
		{
			if (GameEngine.Instance() != null)
				GameEngine.Instance().NotifyMouseMove(e.X, e.Y);
		}

        private void ViewWindow_KeyUp(object sender, KeyEventArgs e)
        {
            if (GameEngine.Instance() != null)
                GameEngine.Instance().NotifyKeyUp( (int)e.KeyCode );
        }

        private void ViewWindow_MouseUp(object sender, MouseEventArgs e)
        {
            if (GameEngine.Instance() != null)
            {
				int KeyCode = 0;
				switch (e.Button )
				{
				case MouseButtons.Left:
						KeyCode = 0x1;
						break;
				case MouseButtons.Right:
						KeyCode = 0x2;
						break;
				case MouseButtons.Middle:
						KeyCode = 0x10;
						break;
				}

				if ( KeyCode != 0 )
					GameEngine.Instance().NotifyMouseUp(KeyCode);
                
            }
        }
    }
}
