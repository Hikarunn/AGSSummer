using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace UiData
{
	internal class InputSet : IExport
	{
		// セットするID
		private enum InputID
        {
			Jump,		// ジャンプ
			Attack,		// 攻撃
			Dash,		// ダッシュ
			Skill		// スキル
        }

		private List<Data> datas_ = new List<Data> ();
		public InputSet(string filePath)
		{
			// xmlファイルを読み込む
			var xml = XElement.Load(filePath);
			var parameters = xml.Element("Datas");
			if (parameters == null)
			{
				throw new Exception();
			}

			var tmp = parameters.Elements("Data");
			foreach (var param in tmp)
			{
				// inputIDを読み込む
				var tmpInputID = param.Element("inputID");
				if (tmpInputID == null)
                {
					return;
                }
				var inputID = (InputID)Int32.Parse(tmpInputID.Value);

				// posXを読み込む
				var tmpX = param.Element("posX");
				if (tmpX == null)
                {
					return;
                }
				var x = float.Parse(tmpX.Value);

				// posYを読み込む
				var tmpY = param.Element("posY");
				if (tmpY == null)
                {
					return ;
                }
				var y = float.Parse(tmpY.Value);

				// コンソールに出力
				Console.WriteLine("inputID=" + inputID + "x=" + x + "y=" + y);

				// 追加
				datas_.Add(new Data(inputID, x, y));
			}
		}

		public void Export(BinaryWriter bw)
		{
			// uiタイプを出力
			bw.Write((int)IExport.UiType.InputSet);

			// データの数を出力
			bw.Write(datas_.Count);

			// すべてのデータを出力
			foreach (var data in datas_)
            {
				bw.Write((int)data.inputID);
				bw.Write(data.x);
				bw.Write(data.y);
            }
		}

		private struct Data
        {
			public InputID inputID;
			public float x;
			public float y;
			public Data(InputID inputID,float x, float y)
            {
				this.inputID = inputID;
				this.x = x;
				this.y = y;
            }
        }
	}
}
