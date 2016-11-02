namespace Editor
{
    partial class ViewWindow
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
            this.SuspendLayout();
            // 
            // ViewWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(545, 464);
            this.HideOnClose = true;
            this.Name = "ViewWindow";
            this.TabText = "ViewWindow";
            this.Text = "ViewWindow";
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ViewWindow_MouseUp);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.ViewWindow_KeyUp);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.ViewWindow_MouseMove);
            this.Resize += new System.EventHandler(this.ViewWindow_Resize);
            this.ResumeLayout(false);

        }

        #endregion
    }
}