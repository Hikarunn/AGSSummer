using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace UiData
{

	internal class ImageArray :
		IExport
	{
		private List<Data> datas_ = new List<Data>();
		public ImageArray(string filePath)
		{
			//private List<Data> datas = new List<Data>();
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
				var tmpImgName = param.Element("imageName");
				if (tmpImgName == null)
                {
					return;
                }
				var imageName = tmpImgName.Value;

				var tmpDivX = param.Element("divX");
				if (tmpDivX == null)
                {
					return;
                }
				var divX = Int32.Parse(tmpDivX.Value);

				var tmpDivY = param.Element("divY");
				if (tmpDivY == null)
                {
					return;
                }
				var divY = Int32.Parse(tmpDivY.Value);

				var tmpSizeX = param.Element("sizeX");
				if (tmpSizeX == null)
                {
					return ;
                }
				var sizeX = Int32.Parse(tmpSizeX.Value);

				var tmpSizeY = param.Element("sizeY");
				if (tmpSizeY == null)
                {
					return;
                }
				var sizeY = Int32.Parse(tmpSizeY.Value);

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

				var tmpInterval = param.Element("interval");
				if (tmpInterval == null)
                {
					return;
                }
				var interval = float.Parse(tmpInterval.Value);

				Console.WriteLine(
					"imageName=" + imageName +
					"divX=" + divX + "divY=" + divY +
					"sizeX" + sizeX + "sizeY=" + sizeY +
					"posX=" + x + "posY=" + y +
					"interval=" + interval
					);

				datas_.Add(new Data(imageName, divX, divY, sizeX, sizeY, x, y, interval));
			}
		}
		public void Export(BinaryWriter bw)
		{
			bw.Write((int)IExport.UiType.ImageArray);
			bw.Write(datas_.Count);

			foreach(var data in datas_)
            {
				bw.Write(data.imageName.Length);
				bw.Write(data.imageName.ToArray());
				bw.Write(data.divX);
				bw.Write(data.divY);
				bw.Write(data.sizeX);
				bw.Write(data.sizeY);
				bw.Write(data.x);
				bw.Write(data.y);
				bw.Write(data.interval);
            }
		}

		private struct Data
        {
			public string imageName;
			public int divX;
			public int divY;
			public int sizeX;
			public int sizeY;
			public float x;
			public float y;
			public float interval;
			public Data(string imageName,int divX, int divY, int sizeX, int sizeY, float x, float y,float interval)
            {
				this.imageName = imageName;
				this.divX = divX;
				this.divY = divY;
				this.sizeX = sizeX;
				this.sizeY = sizeY;
				this.x = x;
				this.y = y;
				this.interval = interval;
            }
        }
	}
}
