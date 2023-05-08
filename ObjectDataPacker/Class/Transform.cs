using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ObjectDataPacker
{
	/// <summary>
	/// 座標、回転、拡縮のクラス
	/// </summary>
	internal class Transform : IExport
	{
		/// <summary>
		/// 座標
		/// </summary>
		private Vector3 pos_;

		/// <summary>
		/// 回転
		/// </summary>
		private Vector3 rotation_;

		/// <summary>
		/// 拡縮
		/// </summary>
		private Vector3 scale_;

		/// <summary>
		/// transform用データのシグネチャ
		/// </summary>
		public static readonly char[] sig_ = { 't', 'd', 'a', 't' };



		public Transform(string file)
		{
			var open = File.OpenRead(file);
			if (open == null)
			{
				return;
			}

			var br = new BinaryReader(open);
			var sig = new char[4];

			for (int i = 0; i < sig.Length; i++)
			{
				sig[i] = br.ReadChar();
			}



			pos_ = new Vector3(br);
			rotation_ = new Vector3(br);
			scale_ = new Vector3(br);

			open.Close();

		}

		public Transform(BinaryReader br)
		{
			pos_ = new Vector3(br);
			rotation_ = new Vector3(br);
			scale_ = new Vector3(br);
		}


		public void Export(BinaryWriter bw)
		{
			bw.Write((int)IExport.Type.Transform);
			uint size = 36;
			bw.Write(size);
			pos_.Write(bw);
			rotation_.Write(bw);
			scale_.Write(bw);
		}

		public string GetInfo()
		{
			return "Transform" + "pos[" + pos_.ToString() + "]rotation[" + rotation_.ToString() + "]scale[" + scale_.ToString() + "]";
		}

		public void Edit(ObjectDataPacker objectDataPacker)
		{
			var result = objectDataPacker.GetTransformFromForm(ref pos_,ref rotation_,ref scale_);
			if (result != null)
			{
				pos_.x = result.Item1.x;
				pos_.y = result.Item1.y;
				pos_.z = result.Item1.z;
				rotation_.x = result.Item2.x;
				rotation_.y = result.Item2.y;
				rotation_.x = result.Item2.z;
				scale_.x = result.Item3.x;
				scale_.y = result.Item3.y;
				scale_.z = result.Item3.z;

				objectDataPacker.ChangeListBoxString(GetInfo());
			}
		}
	}
}
