using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UiData
{
	/// <summary>
	/// 画像のクラス
	/// </summary>
	internal class Image : IExport
	{
		private enum ViewType
		{
			Non,
            blink,
        }

		/// <summary>
		/// データ
		/// </summary>
		private List<Data> dataList_ = new List<Data>();

		/// <summary>
		/// コンストラクタ(xmlファイルを指定)
		/// </summary>
		/// <param name="filePath"> xmlファイルのパス </param>
		/// <exception cref="Exception"></exception>
		public Image(string filePath)
		{
			// xmlファイルをロード
			var xml = XElement.Load(filePath);
			var parameters = xml.Element("Datas");
			if (parameters == null)
			{
				throw new Exception();
			}

			var tmp = parameters.Elements("Data");
			foreach (var param in tmp)
			{
				// posXのデータを読み取る
				var tmpX = param.Element("posX");
				if (tmpX == null)
				{
					return;
				}
				var x = float.Parse(tmpX.Value);

				// posYのデータを読み取る
				var tmpY = param.Element("posY");
				if (tmpY == null)
				{
					return;
				}
				var y = float.Parse(tmpY.Value);

				// imageNameのデータを読み取る
				var tmpImgName = param.Element("imageName");
				if (tmpImgName == null)
				{
					return;
				}
				var imageName = tmpImgName.Value;

				var tmpIViewType = param.Element("viewType");
				if (tmpIViewType == null)
				{
					return;
				}
				var viewType = (ViewType)Int32.Parse(tmpIViewType.Value);

				// 読み込んだデータをコンソールに表示
				Console.WriteLine("x=" + x + "y=" + y + "ImageName=" + imageName + "ViewType" + viewType);

				// リストに追加
				dataList_.Add(new Data(x,y,imageName, viewType));
			}
		}

		/// <summary>
		/// 出力する
		/// </summary>
		/// <param name="bw"> バイナリライター </param>
		public void Export(BinaryWriter bw)
		{
			// uiのタイプを出力する
			bw.Write((int)IExport.UiType.Image);

			// リストの数を出力する
			bw.Write(dataList_.Count);

			// 中身全部出力する
			foreach (var data in dataList_)
			{
				bw.Write(data.x);
				bw.Write(data.y);
				bw.Write(data.imageFile.Length);
				bw.Write(data.imageFile.ToArray());
				bw.Write((int)data.type);
			}
		}


		/// <summary>
		/// データ
		/// </summary>
		private struct Data
		{
			public float x;
			public float y;
			public string imageFile;
			public ViewType type;
            public Data(float x, float y, string imageFile, ViewType type)
			{
				this.x = x;
				this.y = y;
				this.imageFile = imageFile;
                this.type = type;

            }
			
		}
	}
}
