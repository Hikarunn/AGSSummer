using System;
using System.Collections.Generic;
using System.Xml.Linq;
using System.Text;
using System.Threading.Tasks;


namespace ObjectDataPacker
{
	// xmlファイルのデータをパラメーターとしてバイナリ出力するクラス
	public class XmlParameter : IExport
	{
		// パラメータリスト
		private List<Tuple<int, float>> parameterList_  = new List<Tuple<int, float>>();

		// xmlファイルのパス
		private string filePath_;

		private uint dataNum_;

		public XmlParameter(string fileName)
		{
			filePath_ = fileName;
			XElement xml;
			try
			{
				xml = XElement.Load(filePath_);
			}
			catch (Exception )
			{
				throw new Exception("ファイルが開けませんでした");
			}

			// parametersのノードを取得
			var parameters = xml.Element("parameters");
			if (parameters == null)
			{
				return;
			}

			// parameterのノードすべてを見る
			foreach (var parameter in parameters.Elements("parameter"))
			{
				var id = parameter.Element("id");
				if (id == null)
				{
					continue;
				}

				var value = parameter.Element("value");
				if (value == null)
				{
					continue ;
				}

				parameterList_.Add(new Tuple<int, float>(Int32.Parse(id.Value),float.Parse(value.Value)));
			}

			dataNum_ = (uint)parameterList_.Count;
		}

		public XmlParameter(BinaryReader br)
		{
			filePath_ = "Other";
			dataNum_ = br.ReadUInt32();
			
			for (int i = 0; i < dataNum_; i++)
			{
				parameterList_.Add(new Tuple<int, float>(i, br.ReadSingle()));
			}
			
		}

		public void Export(BinaryWriter bw)
		{
			// データの種類を書き込む
			bw.Write((int)IExport.Type.Other);

			// データの総サイズを書き込む
			uint size = (uint)parameterList_.ToArray().Length * 4u;
			bw.Write(size + 4u);

			// データの数を書き込む
			bw.Write(dataNum_);

			foreach(var parameter in parameterList_)
			{
				bw.Write(parameter.Item2);
			}
		}

		public string GetInfo()
		{
			return "パラメータ数=" + dataNum_;
		}
	}
}
