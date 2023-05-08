using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ObjectDataPacker
{
	/// <summary>
	/// 各データの出力をさせるためのインターフェース
	/// </summary>
	internal interface IExport
	{
		enum Type
		{
			Transform,			// 座標回転拡縮データ
			Collider,			// 当たり判定データ
			Model,				// モデルデータのパス
			Spawn,				// スポーンさせる座標とデータパス
			Spawner,			// スポナーの位置
			Other				// そのほかのパラメータ用
			
		}

		/// <summary>
		/// 出力用抽象関数
		/// </summary>
		/// <param name="bw"></param>
		public void Export(BinaryWriter bw);

		/// <summary>
		/// リストに表示する情報を取得する抽象関数
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		public string GetInfo();

		public void Edit(ObjectDataPacker objectDataPacker)
        {

        }
	}
}
