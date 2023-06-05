using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
namespace UiData
{
	internal class LoadIndicator :
		IExport
	{
		private List<Data> datas_ = new List<Data>();
		public LoadIndicator(string filePath)
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
				var tmpImageName1 = param.Element("imageName1");
				if (tmpImageName1 == null)
				{
					return;
				}
				var imageName1 = tmpImageName1.Value;

				var tmpImageName2 = param.Element("imageName2");
				if (tmpImageName2 == null)
				{
					return;
				}
				var imageName2 = tmpImageName2.Value;

				var tmpX = param.Element("posX");
				if (tmpX == null)
				{
					return;
				}
				var x = float.Parse(tmpX.Value);

				var tmpY = param.Element("posY");
				if (tmpY == null)
				{
					return;
				}
				var y = float.Parse(tmpY.Value);

				Console.WriteLine(
					"imageName1=" + imageName1 +
					"imageName2=" + imageName2 +
					"x=" + x +
					"y=" + y
					);
				datas_.Add(new Data(imageName1, imageName2, x, y));
			}
		}

		public void Export(BinaryWriter bw)
		{
			bw.Write((int)IExport.UiType.LoadIndicaator);
			bw.Write(datas_.Count);
			
			foreach(var data in datas_)
            {
				bw.Write(data.imageName1.Length);
				bw.Write(data.imageName1.ToArray());
				bw.Write(data.imageName2.Length);
				bw.Write(data.imageName2.ToArray());
				bw.Write(data.x);
				bw.Write(data.y);
            }
		}

		private struct Data
        {
			public string imageName1;
			public string imageName2;
			public float x;
			public float y;
			public Data(string imageName1, string imageName2, float x, float y)
            {
				this.imageName1 = imageName1;
				this.imageName2 = imageName2;
				this.x = x;
				this.y = y;
            }
        }
	}
}
