using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ObjectDataPacker
{
	internal class SpawnObj : IExport
	{
		public enum SpType
		{
			Stage,
			Sky,		// スカイドームの生成
			Player,		// プレイヤーの生成
			Enemy		// 敵の生成

		}

		/// <summary>
		/// スポーンするタイプ
		/// </summary>
		SpType type_;

		/// <summary>
		/// 生成用の座標
		/// </summary>
		private Vector3 pos_;

		/// <summary>
		/// 生成の回転
		/// </summary>
		private Vector3 rot_;

		/// <summary>
		/// 生成の拡縮
		/// </summary>
		private Vector3 scale_;

		/// <summary>
		/// パス
		/// </summary>
		private string path_;

		public SpawnObj(string transform, string path, SpType type)
		{
			path_ = path;
			type_ = type;
			var open = File.OpenRead(transform);
			if (open == null)
			{
				return;
			}

			var br = new BinaryReader(open);
			var sig = new char[4];

			for (int i = 0; i < sig.Length; i++)
			{
				sig[i] = br.ReadChar();
				if (Transform.sig_[i] != sig[i])
				{
					return;
				}
			}

			// スポーンさせるトランスフォームを読み込む
			pos_ = new Vector3(br);
			rot_ = new Vector3(br);
			scale_ = new Vector3(br);

			open.Close();
		}

		public SpawnObj(BinaryReader br)
		{
			// タイプを読み込む
			type_ =  (SpType)br.ReadInt32();

			// トランスフォームを読み込む
			pos_ = new Vector3(br);
			rot_ = new Vector3(br);
			scale_ = new Vector3(br);

			// 文字数を読み込む
			int num = br.ReadInt32();

			// 文字数分読み込み文字列型にする
			var str  = new String(br.ReadChars(num));
			if (str != null)
			{
				path_ = str;
			}
			else
			{
				path_ = "";
			}
		}

		public void Export(BinaryWriter bw)
		{
			// タイプを書き込む
			bw.Write((int)IExport.Type.Spawn);

			// サイズを書き込む
			uint size = (uint)(12 * 3 + 4 + path_.Length);
			bw.Write(size);
			bw.Write((int)type_);

			// スポーンさせるtransformを書き込む
			pos_.Write(bw);
			rot_.Write(bw);
			scale_.Write(bw);

			// 文字数と文字列を書き込む
			bw.Write(path_.Length);
			bw.Write(path_.ToArray());
		}

		public  string GetInfo()
		{
			return path_ + "pos[" + pos_ + "]";
		}

		public void Edit(ObjectDataPacker objectDataPacker)
		{
			var path = objectDataPacker.GetTextFromForm("データのファイルパスを入力してください",path_);
			if (path == null)
			{
				return;
			}

			var transform = objectDataPacker.GetTransformFromForm(ref pos_, ref rot_, ref scale_);
			if (transform == null)
			{
				return;
			}

			path_ = path;
			pos_.x = transform.Item1.x;
			pos_.y = transform.Item1.y;
			pos_.z = transform.Item1.z;
			rot_.x = transform.Item2.x;
			rot_.y = transform.Item2.y;
			rot_.x = transform.Item2.z;
			scale_.x = transform.Item3.x;
			scale_.y = transform.Item3.y;
			scale_.z = transform.Item3.z;

			objectDataPacker.ChangeListBoxString(GetInfo());
		}
	}
}
