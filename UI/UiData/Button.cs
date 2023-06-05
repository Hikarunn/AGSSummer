using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace UiData
{
	internal class Button : IExport
	{
		/// <summary>
		/// ボタンの処理のID
		/// </summary>
		private enum FuncID
		{
			Tutorial,       // チュートリアル
			Play,           // ゲーム開始
			SelectOption,   // セレクトシーンからオプション
			SelectDialog,   // セレクトシーンからゲーム終了ダイアログ
			PauseBack,      // ポーズシーン戻る
			PauseOption,    // ポーズシーンからオプション
			PauseDialog,    // ポーズシーンから終了ダイアログ
			OptionBack,     // オプション戻る
			OptionOk,       // オプションOK
			DialogOk,       // ダイアログOK
			DialogCancel    // ダイアログキャンセル
		}

		/// <summary>
		/// データ
		/// </summary>
		private List<Data> data_ = new List<Data>();

		public Button(string fileName)
		{
			var xml = XElement.Load(fileName);
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

				// imageNameを読み込む
				var tmpImageName = param.Element("imageName");
				if (tmpImageName == null)
				{
					return;
				}
				var imageName = tmpImageName.Value;

				// frameImageNameを読み込む
				var tmpFrameImageName = param.Element("frameImageName");
				if (tmpFrameImageName == null)
				{
					return;
				}
				var frameImage = tmpFrameImageName.Value;

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

				// コンソールに出力する
				Console.WriteLine("funcID=" + funcID + "imageName=" + imageName + "frameImageName=" + frameImage + "x=" + x + "y=" + y);

				// 追加する
				data_.Add(new Data(funcID, imageName, frameImage, x, y));
			}
		}

		public void Export(BinaryWriter bw)
		{
			// Uiのタイプを出力する
			bw.Write((int)(IExport.UiType.Button));

			// データの数を出力する
			bw.Write(data_.Count);

			// データすべてを出力する
			foreach (var data in data_)
			{
				bw.Write((int)(data.funcID));
				bw.Write(data.imageName.Length);
				bw.Write(data.imageName.ToArray());
				bw.Write(data.frameImageName.Length);
				bw.Write(data.frameImageName.ToArray());
				bw.Write(data.x);
				bw.Write(data.y);
			}

		}

		private struct Data
		{
			public FuncID funcID;
			public string imageName;
			public string frameImageName;
			public float x;
			public float y;

			public Data(FuncID funcID, string imageName, string frameImageName, float x, float y)
			{
				this.funcID = funcID;
				this.imageName = imageName;
				this.frameImageName = frameImageName;
				this.x = x;
				this.y = y;
			}
		}
	}
}
