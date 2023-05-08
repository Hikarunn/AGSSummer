namespace ObjectDataPacker
{
    partial class TextForm
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
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.入力 = new System.Windows.Forms.Button();
            this.キャンセル = new System.Windows.Forms.Button();
            this.TextLabel1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(159, 53);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(265, 23);
            this.textBox1.TabIndex = 0;
            // 
            // 入力
            // 
            this.入力.Location = new System.Drawing.Point(391, 120);
            this.入力.Name = "入力";
            this.入力.Size = new System.Drawing.Size(75, 23);
            this.入力.TabIndex = 1;
            this.入力.Text = "入力";
            this.入力.UseVisualStyleBackColor = true;
            this.入力.Click += new System.EventHandler(this.入力_Click);
            // 
            // キャンセル
            // 
            this.キャンセル.Location = new System.Drawing.Point(500, 120);
            this.キャンセル.Name = "キャンセル";
            this.キャンセル.Size = new System.Drawing.Size(75, 23);
            this.キャンセル.TabIndex = 2;
            this.キャンセル.Text = "キャンセル";
            this.キャンセル.UseVisualStyleBackColor = true;
            this.キャンセル.Click += new System.EventHandler(this.キャンセル_Click);
            // 
            // TextLabel1
            // 
            this.TextLabel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.TextLabel1.Location = new System.Drawing.Point(0, 0);
            this.TextLabel1.Name = "TextLabel1";
            this.TextLabel1.Size = new System.Drawing.Size(587, 33);
            this.TextLabel1.TabIndex = 3;
            this.TextLabel1.Text = "label1";
            this.TextLabel1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // TextForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(587, 155);
            this.Controls.Add(this.TextLabel1);
            this.Controls.Add(this.キャンセル);
            this.Controls.Add(this.入力);
            this.Controls.Add(this.textBox1);
            this.Name = "TextForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private TextBox textBox1;
        private Button 入力;
        private Button キャンセル;
        private Label TextLabel1;
    }
}