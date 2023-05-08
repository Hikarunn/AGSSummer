namespace ObjectDataPacker
{
    partial class ObjectDataPacker
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.追加ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Transform = new System.Windows.Forms.ToolStripMenuItem();
            this.Collider = new System.Windows.Forms.ToolStripMenuItem();
            this.primitiveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.meshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Spawn = new System.Windows.Forms.ToolStripMenuItem();
            this.skyDomeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Player = new System.Windows.Forms.ToolStripMenuItem();
            this.enemyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Model = new System.Windows.Forms.ToolStripMenuItem();
            this.otherToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.xmlParameterToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.読み込み = new System.Windows.Forms.ToolStripMenuItem();
            this.出力ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.削除ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.ListBox = new System.Windows.Forms.ListBox();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.編集ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.追加ToolStripMenuItem,
            this.読み込み,
            this.出力ToolStripMenuItem,
            this.削除ToolStripMenuItem,
            this.toolStripMenuItem1,
            this.toolStripMenuItem2,
            this.編集ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(800, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // 追加ToolStripMenuItem
            // 
            this.追加ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Transform,
            this.Collider,
            this.Spawn,
            this.Model,
            this.otherToolStripMenuItem});
            this.追加ToolStripMenuItem.Name = "追加ToolStripMenuItem";
            this.追加ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.追加ToolStripMenuItem.Text = "追加";
            // 
            // Transform
            // 
            this.Transform.Name = "Transform";
            this.Transform.Size = new System.Drawing.Size(180, 22);
            this.Transform.Text = "Transform";
            this.Transform.Click += new System.EventHandler(this.Position_Click);
            // 
            // Collider
            // 
            this.Collider.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.primitiveToolStripMenuItem,
            this.meshToolStripMenuItem});
            this.Collider.Name = "Collider";
            this.Collider.Size = new System.Drawing.Size(180, 22);
            this.Collider.Text = "Collider";
            this.Collider.Click += new System.EventHandler(this.Collider_Click);
            // 
            // primitiveToolStripMenuItem
            // 
            this.primitiveToolStripMenuItem.Name = "primitiveToolStripMenuItem";
            this.primitiveToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.primitiveToolStripMenuItem.Text = "Primitive";
            this.primitiveToolStripMenuItem.Click += new System.EventHandler(this.primitiveToolStripMenuItem_Click);
            // 
            // meshToolStripMenuItem
            // 
            this.meshToolStripMenuItem.Name = "meshToolStripMenuItem";
            this.meshToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.meshToolStripMenuItem.Text = "Mesh";
            this.meshToolStripMenuItem.Click += new System.EventHandler(this.meshToolStripMenuItem_Click);
            // 
            // Spawn
            // 
            this.Spawn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.skyDomeToolStripMenuItem,
            this.Player,
            this.enemyToolStripMenuItem});
            this.Spawn.Name = "Spawn";
            this.Spawn.Size = new System.Drawing.Size(180, 22);
            this.Spawn.Text = "Spawn";
            // 
            // skyDomeToolStripMenuItem
            // 
            this.skyDomeToolStripMenuItem.Name = "skyDomeToolStripMenuItem";
            this.skyDomeToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.skyDomeToolStripMenuItem.Text = "SkyDome";
            this.skyDomeToolStripMenuItem.Click += new System.EventHandler(this.skyDome_Click);
            // 
            // Player
            // 
            this.Player.Name = "Player";
            this.Player.Size = new System.Drawing.Size(180, 22);
            this.Player.Text = "Player";
            this.Player.Click += new System.EventHandler(this.Player_Click);
            // 
            // enemyToolStripMenuItem
            // 
            this.enemyToolStripMenuItem.Name = "enemyToolStripMenuItem";
            this.enemyToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.enemyToolStripMenuItem.Text = "Enemy";
            this.enemyToolStripMenuItem.Click += new System.EventHandler(this.EnemySpawn_Click);
            // 
            // Model
            // 
            this.Model.Name = "Model";
            this.Model.Size = new System.Drawing.Size(180, 22);
            this.Model.Text = "Model";
            this.Model.Click += new System.EventHandler(this.Model_Click);
            // 
            // otherToolStripMenuItem
            // 
            this.otherToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.xmlParameterToolStripMenuItem});
            this.otherToolStripMenuItem.Name = "otherToolStripMenuItem";
            this.otherToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.otherToolStripMenuItem.Text = "Other";
            // 
            // xmlParameterToolStripMenuItem
            // 
            this.xmlParameterToolStripMenuItem.Name = "xmlParameterToolStripMenuItem";
            this.xmlParameterToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.xmlParameterToolStripMenuItem.Text = "XmlParameter";
            this.xmlParameterToolStripMenuItem.Click += new System.EventHandler(this.xmlParameterToolStripMenuItem_Click);
            // 
            // 読み込み
            // 
            this.読み込み.Name = "読み込み";
            this.読み込み.Size = new System.Drawing.Size(65, 20);
            this.読み込み.Text = "読み込み";
            this.読み込み.Click += new System.EventHandler(this.読み込み_Click);
            // 
            // 出力ToolStripMenuItem
            // 
            this.出力ToolStripMenuItem.Name = "出力ToolStripMenuItem";
            this.出力ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.出力ToolStripMenuItem.Text = "出力";
            this.出力ToolStripMenuItem.Click += new System.EventHandler(this.出力ToolStripMenuItem_Click);
            // 
            // 削除ToolStripMenuItem
            // 
            this.削除ToolStripMenuItem.Name = "削除ToolStripMenuItem";
            this.削除ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.削除ToolStripMenuItem.Text = "削除";
            this.削除ToolStripMenuItem.Click += new System.EventHandler(this.削除_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(12, 20);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(12, 20);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // ListBox
            // 
            this.ListBox.FormattingEnabled = true;
            this.ListBox.ItemHeight = 15;
            this.ListBox.Location = new System.Drawing.Point(142, 103);
            this.ListBox.Name = "ListBox";
            this.ListBox.Size = new System.Drawing.Size(443, 199);
            this.ListBox.TabIndex = 1;
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.DefaultExt = "data";
            this.saveFileDialog1.FileName = "object.data";
            this.saveFileDialog1.Filter = "*.data|";
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(61, 4);
            // 
            // 編集ToolStripMenuItem
            // 
            this.編集ToolStripMenuItem.Name = "編集ToolStripMenuItem";
            this.編集ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.編集ToolStripMenuItem.Text = "編集";
            this.編集ToolStripMenuItem.Click += new System.EventHandler(this.編集ToolStripMenuItem_Click);
            // 
            // ObjectDataPacker
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.ListBox);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "ObjectDataPacker";
            this.Text = "ObjectDataPacker";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private MenuStrip menuStrip1;
        private ToolStripMenuItem 追加ToolStripMenuItem;
        private OpenFileDialog openFileDialog1;
        private ToolStripMenuItem Transform;
        private ToolStripMenuItem Collider;
        private ListBox ListBox;
        private ToolStripMenuItem Spawn;
        private SaveFileDialog saveFileDialog1;
        private ToolStripMenuItem Model;
        private ToolStripMenuItem skyDomeToolStripMenuItem;
        private ToolStripMenuItem Player;
        private ContextMenuStrip contextMenuStrip1;
        private ToolStripMenuItem primitiveToolStripMenuItem;
        private ToolStripMenuItem meshToolStripMenuItem;
        private ToolStripMenuItem 読み込み;
        private ToolStripMenuItem toolStripMenuItem1;
        private ToolStripMenuItem toolStripMenuItem2;
        private ToolStripMenuItem 出力ToolStripMenuItem;
        private ToolStripMenuItem 削除ToolStripMenuItem;
        private ToolStripMenuItem enemyToolStripMenuItem;
        private ToolStripMenuItem otherToolStripMenuItem;
        private ToolStripMenuItem xmlParameterToolStripMenuItem;
        private ToolStripMenuItem 編集ToolStripMenuItem;
    }
}