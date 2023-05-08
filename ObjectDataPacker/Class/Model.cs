using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ObjectDataPacker
{
	/// <summary>
	/// モデル
	/// </summary>
	internal class Model : IExport
	{
		// モデルのファイルパス
		private string name_;
		public Model(string name)
		{
			this.name_ = name;
		}

		public Model(BinaryReader br)
		{
			// 文字列数を取得する
			var num = br.ReadInt32();

			// 文字数分読み込み文字列型に
			var str = new String (br.ReadChars(num));
			if (str != null)
			{
				this.name_ = str;
			}
            else
            {
				this.name_ = "";
            }
		}

		public void Export(BinaryWriter bw)
		{
			// データの種類を書き込む
			bw.Write((int)IExport.Type.Model);

			// データの総サイズを書き込む
			uint size = (uint)name_.Length + 4;
			bw.Write(size);

			// 文字数を書き込む
			bw.Write(name_.Length);

			// 文字列を書き込む
			bw.Write(name_.ToArray());
		}

		public string GetInfo()
		{
			return name_;
		}

		public void Edit(ObjectDataPacker objectDataPacker)
		{
			var text = objectDataPacker.GetTextFromForm("モデルのファイルパスを入力してください",name_);
			if (text != null)
            {
				name_ = text;
            }

			objectDataPacker.ChangeListBoxString(name_);
		}
	}

}
