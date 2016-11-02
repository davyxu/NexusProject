namespace FrameAnimationBuilder
{
	partial class FrameAnimationBuilder
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
			this.mMerge = new System.Windows.Forms.Button();
			this.mFirstAsFinalFrame = new System.Windows.Forms.CheckBox();
			this.mPath = new System.Windows.Forms.TextBox();
			this.mBrowse = new System.Windows.Forms.Button();
			this.mSpliteFrames = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// mMerge
			// 
			this.mMerge.Location = new System.Drawing.Point(242, 39);
			this.mMerge.Name = "mMerge";
			this.mMerge.Size = new System.Drawing.Size(109, 50);
			this.mMerge.TabIndex = 2;
			this.mMerge.Text = "Merge";
			this.mMerge.UseVisualStyleBackColor = true;
			this.mMerge.Click += new System.EventHandler(this.mMerge_Click);
			// 
			// mFirstAsFinalFrame
			// 
			this.mFirstAsFinalFrame.AutoSize = true;
			this.mFirstAsFinalFrame.Location = new System.Drawing.Point(12, 39);
			this.mFirstAsFinalFrame.Name = "mFirstAsFinalFrame";
			this.mFirstAsFinalFrame.Size = new System.Drawing.Size(205, 16);
			this.mFirstAsFinalFrame.TabIndex = 3;
			this.mFirstAsFinalFrame.Text = "Use first frame as final frame";
			this.mFirstAsFinalFrame.UseVisualStyleBackColor = true;
			// 
			// mPath
			// 
			this.mPath.Location = new System.Drawing.Point(12, 12);
			this.mPath.Name = "mPath";
			this.mPath.Size = new System.Drawing.Size(294, 21);
			this.mPath.TabIndex = 0;
			// 
			// mBrowse
			// 
			this.mBrowse.Location = new System.Drawing.Point(312, 10);
			this.mBrowse.Name = "mBrowse";
			this.mBrowse.Size = new System.Drawing.Size(39, 23);
			this.mBrowse.TabIndex = 1;
			this.mBrowse.Text = "...";
			this.mBrowse.UseVisualStyleBackColor = true;
			this.mBrowse.Click += new System.EventHandler(this.mBrowse_Click);
			// 
			// mSpliteFrames
			// 
			this.mSpliteFrames.Location = new System.Drawing.Point(165, 68);
			this.mSpliteFrames.Name = "mSpliteFrames";
			this.mSpliteFrames.Size = new System.Drawing.Size(51, 21);
			this.mSpliteFrames.TabIndex = 4;
			this.mSpliteFrames.Text = "0";
			this.mSpliteFrames.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(10, 64);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(149, 12);
			this.label1.TabIndex = 6;
			this.label1.Text = "Splite frames in width: ";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(10, 79);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(119, 12);
			this.label2.TabIndex = 6;
			this.label2.Text = "(0 means  no limit)";
			// 
			// FrameAnimationBuilder
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(363, 103);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.mSpliteFrames);
			this.Controls.Add(this.mBrowse);
			this.Controls.Add(this.mPath);
			this.Controls.Add(this.mFirstAsFinalFrame);
			this.Controls.Add(this.mMerge);
			this.Name = "FrameAnimationBuilder";
			this.Text = "FrameAnimationBuilder";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button mMerge;
		private System.Windows.Forms.CheckBox mFirstAsFinalFrame;
		private System.Windows.Forms.TextBox mPath;
		private System.Windows.Forms.Button mBrowse;
		private System.Windows.Forms.TextBox mSpliteFrames;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;

	}
}

