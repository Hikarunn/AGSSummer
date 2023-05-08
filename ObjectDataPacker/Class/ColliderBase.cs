using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ObjectDataPacker
{
	internal abstract class ColliderBase : IExport
	{
		public enum ColType
		{
			Box,		// ボックス
			Capsule,	// カプセル
			Sphere,		// 球体
			Mesh,        // メッシュ
			Terrain,	// 地形や建物
			Charactor,	// キャラクター
		}

		protected ColType colType_;

		public ColliderBase(ColType type, string path)
		{
			this.colType_ = type;
			Load(path);
		}

		public ColliderBase(ColType type, BinaryReader br)
        {
			this.colType_ = type;
			Load(br);
        }

		/// <summary>
		/// ファイルからロードする
		/// </summary>
		/// <param name="path"></param>
		abstract protected void Load(string path);

		/// <summary>
		/// バイナリリーダーから読み込む
		/// </summary>
		/// <param name="br"></param>
		abstract protected void Load(BinaryReader br);

		/// <summary>
		/// バイナリライターから出力する
		/// </summary>
		/// <param name="bw"></param>
		public abstract void Export(BinaryWriter bw);

		/// <summary>
		/// タイプをセットする
		/// </summary>
		/// <param name="type"></param>
		public void SetType(ColType type)
		{
			colType_ = type;
		}

		/// <summary>
		/// 情報取得用
		/// </summary>
		/// <returns></returns>
		public abstract string GetInfo();
	}
}
