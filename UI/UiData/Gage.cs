using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace UiData
{
	internal class Gage :
		IExport
	{
		/// <summary>
		/// 処理の種類
		/// </summary>
		private enum FuncID
		{
			Hp,			// hpゲージ
			Skill,		// スキル
			Stamina		// スタミナ
		}

		/// <summary>
		/// データ
		/// </summary>
		private List<Data> datas_ = new List<Data>();

		public Gage(string filePath)
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
				// 処理の種類を読み込む
				var tmpFuncID = param.Element("funcID");
				if (tmpFuncID == null)
				{
					return;
				}
				var funcID = (FuncID)Int32.Parse(tmpFuncID.Value);

				// 画像名を読み込む
				var tmpImageNeme = param.Element("imageName");
				if (tmpImageNeme == null)
				{
					return;
				}
				var imageName = tmpImageNeme.Value;

				// フレーム用画像名を読み込む
				var tmpFrameImageName = param.Element("frameImageName");
				if (tmpFrameImageName == null)
				{
					return;
				}
				var frameImageName = tmpFrameImageName.Value;

				// 開始時の値を読み込む
				var tmpStartValue = param.Element("startValue");
				if (tmpStartValue == null)
				{
					return ;
				}
				var startValue = float.Parse(tmpStartValue.Value);

				// 座標を取得x
				var tmpX = param.Element("posX");
				if (tmpX == null)
				{
					return;
				}
				var x = float.Parse(tmpX.Value);

				// 座標を取得y
				var tmpY = param.Element("posY");
				if (tmpY == null)
				{
					return;
				}
				var y = float.Parse(tmpY.Value);

				// コンソールに出力
				Console.WriteLine(
					"funcID" + funcID + 
					"imageName=" + imageName + 
					"frameImage=" + frameImageName + 
					"startValue=" + startValue + 
					"x="+ x + "y=" + y
					);
				datas_.Add(new Data(funcID,imageName,frameImageName,startValue,x,y));
			}
		}
		public void Export(BinaryWriter bw)
		{
			// UIのタイプを出力
			bw.Write((int)IExport.UiType.Gage);

			// 総数を出力
			bw.Write(datas_.Count);

			// すべてを出力
			foreach (var data in datas_)
			{
				bw.Write((int)data.funcID);
				bw.Write(data.imageName.Length);
				bw.Write(data.imageName.ToArray());
				bw.Write(data.frameImageName.Length);
				bw.Write(data.frameImageName.ToArray());
				bw.Write(data.startValue);
				bw.Write(data.x);
				bw.Write(data.y);
			}
		}

		private struct Data
		{
			public FuncID funcID;
			public string imageName;
			public string frameImageName;
			public float startValue;
			public float x;
			public float y;

			public Data(FuncID funcID, string imageName, string frameImageName, float startValue, float x, float y)
			{
				this.funcID = funcID;
				this.imageName = imageName;
				this.frameImageName = frameImageName;
				this.startValue = startValue;
				this.x = x;
				this.y = y;
			}
		}
	}
}
