namespace Editor
{
	partial class AssertViewer
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.treeContent = new System.Windows.Forms.TreeView();
			this.spliter_main = new System.Windows.Forms.TableLayoutPanel();
			this.spliter_searchbar = new System.Windows.Forms.TableLayoutPanel();
			this.btnSearch = new System.Windows.Forms.Button();
			this.txtSearchText = new System.Windows.Forms.TextBox();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.listContent = new System.Windows.Forms.ListView();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.textResourceLocation = new System.Windows.Forms.TextBox();
			this.imageList1 = new System.Windows.Forms.ImageList(this.components);
			this.toolTipLocation = new System.Windows.Forms.ToolTip(this.components);
			this.spliter_main.SuspendLayout();
			this.spliter_searchbar.SuspendLayout();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.SuspendLayout();
			// 
			// treeContent
			// 
			this.treeContent.Dock = System.Windows.Forms.DockStyle.Fill;
			this.treeContent.FullRowSelect = true;
			this.treeContent.HideSelection = false;
			this.treeContent.HotTracking = true;
			this.treeContent.Location = new System.Drawing.Point(0, 0);
			this.treeContent.Name = "treeContent";
			this.treeContent.Size = new System.Drawing.Size(233, 106);
			this.treeContent.TabIndex = 4;
			this.treeContent.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeContent_AfterSelect);
			// 
			// spliter_main
			// 
			this.spliter_main.ColumnCount = 1;
			this.spliter_main.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.spliter_main.Controls.Add(this.spliter_searchbar, 0, 2);
			this.spliter_main.Controls.Add(this.splitContainer1, 0, 1);
			this.spliter_main.Controls.Add(this.textResourceLocation, 0, 0);
			this.spliter_main.Dock = System.Windows.Forms.DockStyle.Fill;
			this.spliter_main.Location = new System.Drawing.Point(0, 0);
			this.spliter_main.Name = "spliter_main";
			this.spliter_main.RowCount = 3;
			this.spliter_main.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 29F));
			this.spliter_main.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.spliter_main.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
			this.spliter_main.Size = new System.Drawing.Size(239, 421);
			this.spliter_main.TabIndex = 5;
			// 
			// spliter_searchbar
			// 
			this.spliter_searchbar.ColumnCount = 2;
			this.spliter_searchbar.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.spliter_searchbar.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
			this.spliter_searchbar.Controls.Add(this.btnSearch, 1, 0);
			this.spliter_searchbar.Controls.Add(this.txtSearchText, 0, 0);
			this.spliter_searchbar.Dock = System.Windows.Forms.DockStyle.Fill;
			this.spliter_searchbar.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.spliter_searchbar.Location = new System.Drawing.Point(3, 394);
			this.spliter_searchbar.Name = "spliter_searchbar";
			this.spliter_searchbar.RowCount = 1;
			this.spliter_searchbar.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.spliter_searchbar.Size = new System.Drawing.Size(233, 24);
			this.spliter_searchbar.TabIndex = 6;
			// 
			// btnSearch
			// 
			this.btnSearch.Dock = System.Windows.Forms.DockStyle.Fill;
			this.btnSearch.Location = new System.Drawing.Point(176, 3);
			this.btnSearch.Name = "btnSearch";
			this.btnSearch.Size = new System.Drawing.Size(54, 18);
			this.btnSearch.TabIndex = 0;
			this.btnSearch.Text = "Search";
			this.btnSearch.UseVisualStyleBackColor = true;
			// 
			// txtSearchText
			// 
			this.txtSearchText.Dock = System.Windows.Forms.DockStyle.Fill;
			this.txtSearchText.Location = new System.Drawing.Point(3, 3);
			this.txtSearchText.Name = "txtSearchText";
			this.txtSearchText.Size = new System.Drawing.Size(167, 21);
			this.txtSearchText.TabIndex = 1;
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.Location = new System.Drawing.Point(3, 32);
			this.splitContainer1.Name = "splitContainer1";
			this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.treeContent);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.listContent);
			this.splitContainer1.Size = new System.Drawing.Size(233, 356);
			this.splitContainer1.SplitterDistance = 106;
			this.splitContainer1.TabIndex = 5;
			// 
			// listContent
			// 
			this.listContent.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
			this.listContent.Dock = System.Windows.Forms.DockStyle.Fill;
			this.listContent.FullRowSelect = true;
			this.listContent.GridLines = true;
			this.listContent.HideSelection = false;
			this.listContent.Location = new System.Drawing.Point(0, 0);
			this.listContent.MultiSelect = false;
			this.listContent.Name = "listContent";
			this.listContent.Size = new System.Drawing.Size(233, 246);
			this.listContent.TabIndex = 6;
			this.listContent.UseCompatibleStateImageBehavior = false;
			this.listContent.View = System.Windows.Forms.View.Details;
			this.listContent.SelectedIndexChanged += new System.EventHandler(this.listContent_SelectedIndexChanged);
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Name";
			this.columnHeader1.Width = 230;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Data";
			this.columnHeader2.Width = 10;
			// 
			// textResourceLocation
			// 
			this.textResourceLocation.Dock = System.Windows.Forms.DockStyle.Fill;
			this.textResourceLocation.Location = new System.Drawing.Point(3, 3);
			this.textResourceLocation.Name = "textResourceLocation";
			this.textResourceLocation.ReadOnly = true;
			this.textResourceLocation.Size = new System.Drawing.Size(233, 21);
			this.textResourceLocation.TabIndex = 7;
			this.textResourceLocation.MouseLeave += new System.EventHandler(this.textResourceLocation_MouseLeave);
			this.textResourceLocation.MouseEnter += new System.EventHandler(this.textResourceLocation_MouseEnter);
			// 
			// imageList1
			// 
			this.imageList1.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
			this.imageList1.ImageSize = new System.Drawing.Size(16, 16);
			this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
			// 
			// AssertViewer
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(239, 421);
			this.Controls.Add(this.spliter_main);
			this.HideOnClose = true;
			this.Name = "AssertViewer";
			this.TabText = "AssertViewer";
			this.Text = "AssertViewer";
			this.Load += new System.EventHandler(this.AssertViewer_Load);
			this.spliter_main.ResumeLayout(false);
			this.spliter_main.PerformLayout();
			this.spliter_searchbar.ResumeLayout(false);
			this.spliter_searchbar.PerformLayout();
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			this.splitContainer1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.TreeView treeContent;
		private System.Windows.Forms.TableLayoutPanel spliter_main;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.ListView listContent;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ImageList imageList1;
		private System.Windows.Forms.TableLayoutPanel spliter_searchbar;
		private System.Windows.Forms.Button btnSearch;
		private System.Windows.Forms.TextBox txtSearchText;
		private System.Windows.Forms.TextBox textResourceLocation;
		private System.Windows.Forms.ToolTip toolTipLocation;

	}
}