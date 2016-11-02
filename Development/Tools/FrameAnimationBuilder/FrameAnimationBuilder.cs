using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.Design;
using System.IO;
using System.Diagnostics;
using System.Collections;

namespace FrameAnimationBuilder
{
	public partial class FrameAnimationBuilder : Form
	{
		public FrameAnimationBuilder()
		{
			InitializeComponent();
		}


		public class MyFolderDialog : FolderNameEditor
		{
			FolderNameEditor.FolderBrowser fDialog = new System.Windows.Forms.Design.FolderNameEditor.FolderBrowser();
			public MyFolderDialog()
			{
				fDialog.Style = FolderBrowserStyles.BrowseForEverything | FolderBrowserStyles.ShowTextBox;
				fDialog.StartLocation = FolderBrowserFolder.MyComputer;
			}

			public DialogResult DisplayDialog(string description)
			{
				fDialog.Description = description;
				return fDialog.ShowDialog();
			}
			public string Path
			{
				get
				{
					return fDialog.DirectoryPath;
				}
			}
			~MyFolderDialog()
			{
				fDialog.Dispose();
			}
		} 


		private void mMerge_Click(object sender, EventArgs e)
		{

			ArrayList array = new ArrayList( );


			Size singesize = new Size( 0, 0 );
			


			foreach (string filename in Directory.GetFiles(mPath.Text))
			{
				if (Path.GetExtension(filename).ToLower() != ".png")
					continue;

				Image image = Image.FromFile( filename );

				if (singesize.Height != 0 && singesize.Height != image.Height)
				{
					MessageBox.Show("Not all image height are equal!");
					return;
				}

				singesize.Height = image.Height;
				singesize.Width = image.Width;  

				array.Add( image );
			}

			if ( array.Count == 0 )
			{
				MessageBox.Show("No png file found");
				return;
			}

			if ( mFirstAsFinalFrame.Checked )
			{
				array.Add(array[0]);
			}

			Size mergedsize = new Size(0, 0);


			int spliteframes = int.Parse(mSpliteFrames.Text);

			if ( spliteframes != 0 )
			{
                // 类似于 1.0001 也约等于 2的算法 ( 1小时过1秒 约等于 2小时算法一样 )
                int cols = (int)( (array.Count + (float)(spliteframes - 1 ) ) / (float)(spliteframes) );
				int row = Math.Min( spliteframes, array.Count );
				mergedsize.Width = row * singesize.Width;
				mergedsize.Height = cols * singesize.Height;
			}
			else
			{
				mergedsize.Width = array.Count * singesize.Width;
				mergedsize.Height = singesize.Height;
			}

			Bitmap bitmap = new Bitmap( mergedsize.Width, mergedsize.Height );
			Graphics mergedgraph = Graphics.FromImage(bitmap);


			

			int xoffset = 0;
			int yoffset = 0;
			int widthframe = 0;
			foreach( Image image in array )
			{
                mergedgraph.DrawImage(image, new Rectangle(xoffset, yoffset, image.Width, image.Height));
				xoffset+= image.Width;
				widthframe++;

				// 拆成竖行
				if ( spliteframes != 0 && widthframe >= spliteframes )
				{
					xoffset = 0;
					yoffset += image.Height;
					widthframe = 0;
				}
			}

	


			string tfilename = Path.GetFileName(mPath.Text);
			string foldername = Path.GetDirectoryName(mPath.Text);
			
			string finalname = Path.Combine(foldername, tfilename)+ ".png";
			bitmap.Save(finalname , System.Drawing.Imaging.ImageFormat.Png);

			MessageBox.Show("Output file: " + finalname + "\r\nTotal " + array.Count.ToString() + " frames\r\nFrame size:" + ( new Size(mergedsize.Width/array.Count, mergedsize.Height)).ToString() + "\r\nFinal image size: " + mergedsize.ToString());


            foreach (Image image in array)
            {
                image.Dispose();
            }

            bitmap.Dispose();

		}

		

		private void mSplite_Click(object sender, EventArgs e)
		{

		}

		private void mBrowse_Click(object sender, EventArgs e)
		{
			MyFolderDialog dialog = new MyFolderDialog();


			if (dialog.DisplayDialog("Please select a folder contain PNG file") != DialogResult.OK)
				return;

			mPath.Text = dialog.Path;
		}

	}
}