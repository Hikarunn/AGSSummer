// See https://aka.ms/new-console-template for more information
using System.Linq;
using UiData;
class App
{
	/// <summary>
	/// xmlファイルを指定してIExportを作成するデリゲート
	/// </summary>
	/// <param name="filePath"> xmlのファイルのパス </param>
	/// <returns> IExport </returns>
	delegate IExport LoadFunc(string filePath);

	/// <summary>
	/// 文字列をキーにLoadFuncを値として持つディクショナリー
	/// </summary>
	static Dictionary<string, LoadFunc> loadFuncs = new Dictionary<string, LoadFunc>();

	static public void Main(string[] args)
	{
		if (args.Length <= 1)
		{
			// ファイルが指定してないとき
			Console.WriteLine("ファイルが有りません");
			return;
		}

		// 追加
		loadFuncs.Add("image", CreateImageExport);
		loadFuncs.Add("imageArray", CreateImageArrayExport);
		loadFuncs.Add("button", CreateButtonExport);
		loadFuncs.Add("slider", CreateSliderExport);
		loadFuncs.Add("inputSet", CreateInputSetExport);
		loadFuncs.Add("load", CreateLoadExport);
		loadFuncs.Add("combo", CreateComboExport);
		loadFuncs.Add("gage", CreateGageExport);
	//	loadFuncs.Add("switch", CreateSwitchButtonExport);

		// リストを作成
		List<IExport> exports = new List<IExport>();

		// xmlファイル分処理する
		for (int i = 1; i < args.Length; i++)
		{
			foreach(var func in loadFuncs)
			{
				if (args[i].EndsWith(func.Key))
				{
					// 一致する関数があった時処理する
					exports.Add(func.Value(args[i]));
					break;
				}
			}
		}

		// ファイルを作成
		var file = File.Create(args[0]);

		// バイナリライターを作成
		var bw = new BinaryWriter(file);

		// シグネチャを出力
		var sig = "uida";

		foreach (var c in sig)
		{
			bw.Write(c);
		}

		// バージョン情報を出力
		var version = 0.0f;
		bw.Write(version);

		// exportsの総数をチェック用の値にしとく
		int ckData = exports.Count;
		bw.Write(ckData);

		// すべてのexportを出力する
		foreach (var export in exports)
		{
			export.Export(bw);
		}

		// ファイルを閉じる
		bw.Close();
		file.Close();
	}

	static IExport CreateImageExport(string filePath)
	{
		return new Image(filePath);
	}

	static IExport CreateButtonExport(string filaPath)
	{
		return new Button(filaPath);
	}

	static IExport CreateSliderExport(string filePath)
	{
		return new Slider(filePath);
	}

	static IExport CreateInputSetExport(string fielPath)
	{
		return new InputSet(fielPath);
	}

	static IExport CreateLoadExport(string filePath)
	{
		return new LoadIndicator(filePath);
	}

	static IExport CreateComboExport(string filePath)
	{
		return new Combo(filePath);
	}

	static IExport CreateGageExport(string filePath)
	{
		return new Gage(filePath);
	}

	static IExport CreateImageArrayExport(string filePath)
    {
		return new ImageArray(filePath);
    }


	//static IExport CreateSwitchButtonExport(string filePath) 
	//{
	//	return new SwitchButton(filePath);
	//}
}
