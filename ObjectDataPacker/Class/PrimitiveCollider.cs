using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ObjectDataPacker
{
	/// <summary>
	/// プリミティブな形状の当たり判定
	/// </summary>
	internal class PrimitiveCollider : ColliderBase
	{
		/// <summary>
		/// タイプごとに読み込みオブジェクトを生成するためのデリゲート宣言
		/// </summary>
		/// <param name="reader"></param>
		delegate void LoadTypeFunc(BinaryReader reader);

		/// <summary>
		/// Typeと上記のデリゲートで構成されるディクショナリー
		/// </summary>
		Dictionary<ColType, LoadTypeFunc> load_ = new Dictionary<ColType, LoadTypeFunc>();

		/// <summary>
		/// 形状
		/// </summary>
		private Primitive? primitive_;

		static private readonly char[] colSig = { 'c', 'd', 'a', 't' };

		public PrimitiveCollider(string path) :
			 base(ColliderBase.ColType.Box, path)
		{
		}

		public PrimitiveCollider(ColType type, BinaryReader br) :
			base(type,br)
		{

		}

		/// <summary>
		/// 出力する
		/// </summary>
		/// <param name="bw"></param>
		public override void Export(BinaryWriter bw)
		{
			// タイプを出力
			bw.Write((int)IExport.Type.Collider);
			if (primitive_ == null)
			{
				return;
			}
			
			// 当たり判定の情報を出力
			primitive_.Export(bw);
		}

		/// <summary>
		/// 当たり判定ファイルから読み込む
		/// </summary>
		/// <param name="path"></param>
		protected override void Load(string path)
		{
			// 以下ディクショナリーに関数を追加
			load_.Add(ColType.Box, BoxLoad);
			load_.Add(ColType.Sphere, SphereLoad);
			load_.Add(ColType.Capsule, CapsuleLoad);

			// ファイルを開ける
			var file = File.OpenRead(path);
			var br = new BinaryReader(file);

			// シグネチャチェック
			var sig = new char[4];
			sig[0] = 'c';
			sig[1] = 'd';
			sig[2] = 'a';
			sig[3] = 't';
			for (int i = 0; i < 4; i++)
			{
				sig[i] = br.ReadChar();
				if (sig[i] != colSig[i])
				{
					return;
				}
			}

			// タイプをセット
			base.SetType((ColliderBase.ColType)br.ReadInt32());
			load_[colType_](br);

			br.Close();

		}

		/// <summary>
		/// boxの生成
		/// </summary>
		/// <param name="reader"></param>
		void BoxLoad(BinaryReader reader)
		{
			primitive_ = new Box(reader);
		}

		/// <summary>
		/// 球体の生成
		/// </summary>
		/// <param name="reader"></param>
		void SphereLoad(BinaryReader reader)
		{
			primitive_ = new Sphere(reader);
		}

		/// <summary>
		/// カプセルの生成
		/// </summary>
		/// <param name="reader"></param>
		void CapsuleLoad(BinaryReader reader)
		{
			primitive_ = new Capsule(reader);
		}

		public override string GetInfo()
		{
			if (primitive_ == null)
			{
				return "";
			}
			return primitive_.GetType().ToString();

		}

		protected override void Load(BinaryReader br)
		{
			// 以下ディクショナリーに関数を追加
			load_.Add(ColType.Box, BoxLoad);
			load_.Add(ColType.Sphere, SphereLoad);
			load_.Add(ColType.Capsule, CapsuleLoad);
			load_[colType_](br);
		}

		/// <summary>
		/// 大元になるクラス
		/// </summary>
		abstract class Primitive
		{
			public Vector3 pos_;
			public Primitive(BinaryReader br)
			{
				pos_ = new Vector3(br);
			}

			public abstract void Export(BinaryWriter bw);
		}

		/// <summary>
		/// Box
		/// </summary>
		class Box : Primitive
		{
			// ボックスサイズ
			public Vector3 size_;

			public Box(BinaryReader br) : base(br)
			{
				size_ = new Vector3(br);
			}

			public override void Export(BinaryWriter bw)
			{
				// サイズを書き込む
				bw.Write(28);

				// タイプを書き込む
				bw.Write((int)ColType.Box);

				// 座標とサイズを書き込む
				pos_.Write(bw);
				size_.Write(bw);
			}
		}

		/// <summary>
		/// カプセル
		/// </summary>
		class Capsule : Primitive
		{
			// カプセルの高さ
			private float height_;

			// カプセルの半径
			private float radius_;

			public Capsule(BinaryReader br) : base(br)
			{
				height_ = br.ReadSingle();
				radius_ = br.ReadSingle();
			}

			public override void Export(BinaryWriter bw)
			{
				// サイズを書き込む
				bw.Write(24);

				// タイプを書き込む
				bw.Write((int)ColType.Capsule);

				// 座標と半径と高さを書き込む
				pos_.Write(bw);
				bw.Write(height_);
				bw.Write(radius_);
			}
		}

		/// <summary>
		/// 球体
		/// </summary>
		class Sphere : Primitive
		{
			// 半径
			float radius_;
			public Sphere(BinaryReader br) : base(br)
			{
				radius_ = br.ReadSingle();
			}

			public override void Export(BinaryWriter bw)
			{
				// サイズを書き込む
				bw.Write(20);

				// タイプを書き込む
				bw.Write((int)ColType.Sphere);

				// 座標と半径を書き込む
				pos_.Write(bw);
				bw.Write(radius_);
			}
		}

	}
}
