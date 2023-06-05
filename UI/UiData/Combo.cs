using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace UiData
{
	internal class Combo :
		IExport
	{
		private List<Data> datas_ = new List<Data>();

		public Combo(string fileName)
		{
			// xmlファイルを読み込む
			var xml = XElement.Load(fileName);
			var parameters = xml.Element("Datas");
			if (parameters == null)
			{
				throw new Exception();
			}

			var tmp = parameters.Elements("Data");
			foreach (var param in tmp)
			{
				var tmpNumImageName = param.Element("numImageName");
				if (tmpNumImageName == null)
				{
					return;
				}
				var numImageName = tmpNumImageName.Value;

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
					return;
				}
				var sizeX = Int32.Parse(tmpSizeX.Value);

				var tmpSizeY = param.Element("sizeY");
				if (tmpSizeY == null)
				{
					return;
				}
				var sizeY = Int32.Parse(tmpSizeY.Value);

				var tmpImageName = param.Element("imageName");
				if (tmpImageName == null)
				{
					return;
				}
				var imageName = tmpImageName.Value;

				var tmpFrameImageName = param.Element("frameImageName");
				if (tmpFrameImageName == null)
				{
					return;
				}
				var frameImageName = tmpFrameImageName.Value;

				var tmpStartValue = param.Element("startValue");
				if (tmpStartValue == null)
				{
					return ;
				}
				var startValue = float.Parse(tmpStartValue.Value);

				var tmpPosX = param.Element("posX");
				if (tmpPosX == null)
				{
					return;
				}
				var x = float.Parse(tmpPosX.Value);

				var tmpPosY = param.Element("posY");
				if (tmpPosY == null)
				{
					return;
				}
				var y = float.Parse(tmpPosY.Value);

				var tmpIntervalX = param.Element("intervalX");
				if (tmpIntervalX == null)
				{
					return;
				}
				var intervalX = float.Parse(tmpIntervalX.Value);

				var tmpIntervalY = param.Element("intervalY");
				if (tmpIntervalY == null)
				{
					return;
				}
				var intervalY = float.Parse(tmpIntervalY.Value);


				Console.WriteLine(
					"numImageName=" + numImageName +
					"divX=" + divX + "divY=" + divY +
					"sizeX=" + sizeX + "sizeY=" + sizeY +
					"imageName=" + imageName + "frameImageName=" + frameImageName +
					"x=" + x + "y=" + y +
					"intervalX=" + intervalX + "intervalY=" + intervalY
					);

				datas_.Add(
					new Data(
						numImageName,
						divX, divY,
						sizeX, sizeY,
						imageName, frameImageName,
						startValue,
						x, y,
						intervalX, intervalY
						));

			}
		}

		public void Export(BinaryWriter bw)
		{
			bw.Write((int)IExport.UiType.Combo);
			bw.Write(datas_.Count);
			foreach(var data in datas_)
            {
				bw.Write(data.numImageName.Length);
				bw.Write(data.numImageName.ToArray());
				bw.Write(data.divX);
				bw.Write(data.divY);
				bw.Write(data.sizeX);
				bw.Write(data.sizeY);
				bw.Write(data.imageName.Length);
				bw.Write(data.imageName.ToArray());
				bw.Write(data.frameImageName.Length);
				bw.Write(data.frameImageName.ToArray());
				bw.Write(data.startValue);
				bw.Write(data.x);
				bw.Write(data.y);
				bw.Write(data.intervalX);
				bw.Write(data.intervalY);
            }
		}

		private struct Data
		{
			public string numImageName;
			public int divX;
			public int divY;
			public int sizeX;
			public int sizeY;
			public string imageName;
			public string frameImageName;
			public float startValue;
			public float x;
			public float y;
			public float intervalX;
			public float intervalY;

			public Data(
				string numImageName,
				int divX, int divY, 
				int sizeX, int sizeY, 
				string imageName, string frameImageName,
				float startValue, 
				float x, float y, 
				float intervalX, float intervalY)
			{
				this.numImageName = numImageName;
				this.divX = divX;
				this.divY = divY;
				this.sizeX = sizeX;
				this.sizeY = sizeY;
				this.imageName = imageName;
				this.frameImageName = frameImageName;
				this.startValue = startValue;
				this.x = x;
				this.y = y;
				this.intervalX = intervalX;
				this.intervalY = intervalY;
			}
		}
	}
}
