using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UiData
{
	internal interface IExport
	{
		/// <summary>
		/// Uiの種類
		/// </summary>
		public enum UiType
		{
			Image,		// 画像
			ImageArray,	// 画像配列版
			Button,		// ボタン
			Slider,		// スライダー
			InputSet,	// 入力設定
			Gage,		// ゲージ
			Combo,		// コンボ
			LoadIndicaator,		// ロード用
			SwitchButton,		// オンオフがあるボタン
		}

		/// <summary>
		/// ファイルに出力する処理
		/// </summary>
		/// <param name="bw"> バイナリライター </param>
		abstract public void Export(BinaryWriter bw);
	}
}
