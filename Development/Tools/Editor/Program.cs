using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.IO;

namespace Editor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

			
            //Directory.SetCurrentDirectory( @"..\..\..\..\..\bin");
            //String Dir = Directory.GetCurrentDirectory();

			bool ImmdiateMode = true;

			if ( ImmdiateMode )
			{
				using (MainFrame frame = new MainFrame())
				{
					frame.Show();

					while (frame.Created)
					{
						frame.Application_Idle(null, null);
						Application.DoEvents();
					}
				}
			}
			else
			{
				MainFrame frame = new MainFrame();
				Application.Idle += new EventHandler(frame.Application_Idle);
				Application.Run(frame);
			}
			



        }
    }
}
