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
using System.IO;
using System.Diagnostics;

namespace Editor
{
	public partial class AssertViewer : DockContent
	{
		private String mSelectedResourceLocation;

		static AssertViewer mInstance;
	
		public static AssertViewer Instance( )
		{
			return mInstance;
		}

		public AssertViewer()
		{
			mInstance = this;
			InitializeComponent();
		}

		public String SelectedResourceLocation
		{
			get { return mSelectedResourceLocation; }
		}

		void RefreshFolder( )
		{
			String CurrentLevelPath = GameEngine.Instance().ContentPath;

			AddNodes(CurrentLevelPath, treeContent.Nodes);
		}

		void AddNodes(string RootPath, TreeNodeCollection Root)
		{
			String[] AllDirecory = Directory.GetDirectories(RootPath);

			foreach (String dir in AllDirecory)
			{
				string NodeName = Path.GetFileName(dir);

				// 带有注释的文件夹不放进来
				if (NodeName.Length > 0 && NodeName.Substring(0, 1) == "#")
					continue;

				TreeNode NewNode = new TreeNode(NodeName);
				NewNode.Tag = dir;

				Root.Add(NewNode);

				AddNodes(dir, NewNode.Nodes);
			}
		}

		void RefreshFiles( )
		{
			if ( treeContent.SelectedNode == null )
				return;

			String path = (String)treeContent.SelectedNode.Tag;
			if (path == null)
				return;

			String[] filelist = Directory.GetFiles(path);

			listContent.Items.Clear();

			foreach( String filename in filelist )
			{
				ListViewItem Item = new ListViewItem();
				Item.Text = Path.GetFileNameWithoutExtension( filename );
				listContent.Items.Add(Item);
			}

		}

		private void AssertViewer_Load(object sender, EventArgs e)
		{
			toolTipLocation.AutoPopDelay = 10000;
			toolTipLocation.InitialDelay = 100;
			toolTipLocation.ReshowDelay = 500;

			RefreshFolder();
		}


		private void treeContent_AfterSelect(object sender, TreeViewEventArgs e)
		{
			RefreshFiles();
			UpdateResourceLocation();

			
		}

		void SetLocationText( String text )
		{
			mSelectedResourceLocation = text;
			textResourceLocation.Text = text;
		}

		void UpdateResourceLocation( )
		{
			if (listContent.SelectedItems.Count == 0)
			{
				SetLocationText(null);
				return;
			}

			Debug.Assert(treeContent.SelectedNode != null);

			TreeNode CurrentNode = treeContent.SelectedNode;
			String ParentLayer = CurrentNode.Text;

			while (CurrentNode.Parent != null)
			{
				CurrentNode = CurrentNode.Parent;

				ParentLayer = CurrentNode.Text + "." + ParentLayer;
			}

			ListViewItem Item = listContent.SelectedItems[0];

			SetLocationText( ParentLayer + "." + Item.Text );
		}

		

		private void listContent_SelectedIndexChanged(object sender, EventArgs e)
		{
			UpdateResourceLocation();
		}

		private void textResourceLocation_MouseEnter(object sender, EventArgs e)
		{
			toolTipLocation.Show(textResourceLocation.Text, textResourceLocation);
		}

		private void textResourceLocation_MouseLeave(object sender, EventArgs e)
		{
			toolTipLocation.Hide(textResourceLocation);
		}
	}
}
