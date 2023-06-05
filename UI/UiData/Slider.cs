using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;


namespace UiData
{
	internal class Slider :
		IExport
	{
		/// <summary>
		/// スライダーの処理の種類
		/// </summary>
		enum FuncID
		{
			Se,			// 効果音調整
			Bgm,		// Bgm調整
			Sensitivity	// カメラ感度
		}

		private List<Data> datas_ = new List<Data>();

		public Slider(string filaPath)
		{
			// xmlファイルを読み込む
			var xml = XElement.Load(filaPath);
			var parameters = xml.Element("Datas");
			if (parameters == null)
			{
				throw new Exception();
			}

			var tmp = parameters.Elements("Data");
			foreach (var param in tmp)
			{
				// funcIDを読み込む
				var tmpFuncID = param.Element("funcID");
				if (tmpFuncID == null)
				{
					return;
				}
				var funcID = (FuncID)Int32.Parse(tmpFuncID.Value);

				// barImageNameを読み込む
				var tmpBarImageName = param.Element("barImageName");
				if (tmpBarImageName == null)
				{
					return;
				}
				var barImageName = tmpBarImageName.Value;

				// tabImageNameを読み込む
				var tmpTabImageName = param.Element("tabImageName");
				if (tmpTabImageName == null)
				{
					return;
				}
				var tabIamgeName = tmpTabImageName.Value;

				// startValueを読み込む
				var tmpStartValue = param.Element("startValue");
				if (tmpStartValue == null)
				{
					return;
				}
				var startValue = float.Parse(tmpStartValue.Value);

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
					return;
				}
				var y = float.Parse(tmpY.Value);

				// コンソールに出力
				Console.WriteLine("funcID=" + funcID + "barImageName=" + barImageName + "tabImageName=" + tabIamgeName + "startValue=" + startValue + "x=" + x + "y=" + y);

				// リストに追加
				datas_.Add(new Data(funcID, barImageName, tabIamgeName, startValue, x, y));
			}
		}

		public void Export(BinaryWriter bw)
		{
			// タイプを出力
			bw.Write((int)IExport.UiType.Slider);

			// データの数を出力
			bw.Write(datas_.Count);
			
			// すべて出力する
			foreach (var data in datas_)
			{
				bw.Write((int)data.funcID);
				bw.Write(data.barImageName.Length);
				bw.Write(data.barImageName.ToArray());
				bw.Write(data.tabIamgeName.Length);
				bw.Write(data.tabIamgeName.ToArray());
				bw.Write(data.startValue);
				bw.Write(data.x);
				bw.Write(data.y);
			}
		}

		private struct Data
		{
			public FuncID funcID;
			public string barImageName;
			public string tabIamgeName;
			public float startValue;
			public float x;
			public float y;
			public Data(FuncID funcID, string barImageName, string tabImageName, float startValue, float x, float y)
			{
				this.funcID = funcID;
				this.barImageName = barImageName;
				this.tabIamgeName = tabImageName;
				this.startValue = startValue;
				this.x = x;
				this.y = y;
			}
		}
	}
}
