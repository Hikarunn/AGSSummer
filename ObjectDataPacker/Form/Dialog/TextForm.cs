using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ObjectDataPacker
{
	public partial class TextForm : Form
	{
		private string text;

		public string Text_
        {
			set { text = value; textBox1.Text = value; }
			get { return text; }
        }

		public TextForm()
		{
			text = "Text";
			InitializeComponent();
		}

		private void 入力_Click(object sender, EventArgs e)
		{
			text = textBox1.Text;
			this.DialogResult = DialogResult.OK;
			textBox1.Text = String.Empty;
			this.Close();
		}

		public void ClearText()
		{
			text = String.Empty;
			textBox1.Text = String.Empty;
		}

		public void SetLabel(string str)
        {
			TextLabel1.Text = str;
        }

		private void キャンセル_Click(object sender, EventArgs e)
		{
			this.DialogResult = DialogResult.Cancel;
			this.Close();
		}
	}
}
