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
	public partial class TransformForm : Form
	{
		private Vector3 pos_;
		private Vector3 rot_;
		private Vector3 scale_;

		public TransformForm()
		{
			pos_ = new Vector3();
			rot_ = new Vector3();
			scale_ = new Vector3();
			InitializeComponent();
		}

		public void SetValue(ref Vector3 pos, ref Vector3 rot,  ref Vector3 scale)
		{
			PosX.Value = (decimal)pos.x;
			PosY.Value = (decimal)pos.y;
			PosZ.Value = (decimal)pos.z;
			RotX.Value = (decimal)rot.x;
			RotY.Value = (decimal)rot.y;
			RotZ.Value = (decimal)rot.x;
			ScaleX.Value = (decimal)scale.x;
			ScaleY.Value = (decimal)scale.y;
			ScaleZ.Value = (decimal)scale.x;
		}

		public void Clear()
        {
			pos_.x = 0.0f;
			pos_.y = 0.0f;
			pos_.z = 0.0f;
			rot_.x = 0.0f;
			rot_.y = 0.0f;
			rot_.z = 0.0f;
			scale_.x = 0.0f;
			scale_.y = 0.0f;
			scale_.z = 0.0f;
		}

		public Tuple< Vector3,Vector3,Vector3> GetValue()
        {
			return new Tuple<Vector3, Vector3, Vector3>(pos_,rot_,scale_);
        }

		private void button1_Click(object sender, EventArgs e)
		{
			// okの時
			pos_.x = (float)PosX.Value;
			pos_.y = (float)PosY.Value;
			pos_.z = (float)PosZ.Value;
			rot_.x = (float)RotX.Value;
			rot_.y = (float)RotY.Value;
			rot_.z = (float)RotZ.Value;
			scale_.x = (float)ScaleX.Value;
			scale_.y = (float)ScaleY.Value;
			scale_.z = (float)ScaleZ.Value;
			DialogResult = DialogResult.OK;
			Close();
		}

		private void button2_Click(object sender, EventArgs e)
		{
			DialogResult = DialogResult.Cancel;
			Clear();
			Close();
		}



	}
}
