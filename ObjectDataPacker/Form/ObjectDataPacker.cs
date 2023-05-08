namespace ObjectDataPacker
{
	public partial class ObjectDataPacker : Form
	{
		/// <summary>
		/// �o�͂��邽�߂̃C���^�[�t�F�[�X�̃��X�g
		/// </summary>
		private List<IExport> exports_ = new List<IExport>();

		/// <summary>
		/// dialog
		/// </summary>
		private Dialog dialog_ = new Dialog();

		/// <summary>
		/// �e�L�X�g���͗p�t�H�[��
		/// </summary>
		private TextForm textForm = new TextForm();

		private TransformForm transformForm_ = new TransformForm();

		/// <summary>
		/// �o�[�W����
		/// </summary>
		static readonly private uint version_ = 0;

		/// <summary>
		// �o�͗p�V�O�l�`��
		/// </summary>
		static readonly private char[] fileSig = { 'd', 'a', 't', 'a' };

		/// <summary>
		/// �t�@�C���ǂݍ��ݗp�f���Q�[�g
		/// </summary>
		/// <param name="br"></param>
		private delegate void LoadFunc(BinaryReader br);

		/// <summary>
		/// �^�C�v�ƃf���Q�[�g�̃f�B�N�V���i���[
		/// </summary>
		private Dictionary<IExport.Type, LoadFunc> loadFuncs = new Dictionary<IExport.Type, LoadFunc>();


		public ObjectDataPacker()
		{
			loadFuncs.Add(IExport.Type.Collider, LoadCollider);
			loadFuncs.Add(IExport.Type.Model, LoadMoodel);
			loadFuncs.Add(IExport.Type.Transform, LoadTransform);
			loadFuncs.Add(IExport.Type.Spawn, LoadSpawnObject);
			loadFuncs.Add(IExport.Type.Other,LoadXmlParameter);

			InitializeComponent();
		}

		/// <summary>
		/// �_�C�A���O����J���t�@�C���̃p�X���擾����
		/// </summary>
		/// <param name="mess"> �^�C�g�����b�Z�[�W </param>
		/// <returns> �������ǂ����H(bool)�ƃt�@�C���p�X�̕����� </returns>
		public string? GetOpenFilePathFromDialog(string mess)
		{
			openFileDialog1.Title = mess;
			if (openFileDialog1.ShowDialog() == DialogResult.OK)
			{
				return openFileDialog1.FileName;
			}
			return null;
		}

		public string? GetSaveFilePathFromDialog(string mess)
		{
			return GetTextFromForm(mess, "");
		}

		public string?GetTextFromForm(string mess, string start)
		{
			textForm.SetLabel(mess);
			textForm.Text_= start;
			if (textForm.ShowDialog() == DialogResult.OK)
			{
				return textForm.Text_;
			}
			return null;
		}

		public string? GetTextFromForm(string mess)
		{
			textForm.SetLabel(mess);
			if (textForm.ShowDialog() == DialogResult.OK)
			{
				return textForm.Text_;
			}
			return null;
		}

		public Tuple<Vector3,Vector3,Vector3>? GetTransformFromForm(ref Vector3 pos, ref Vector3 rot, ref Vector3 scale)
        {
			transformForm_.SetValue(ref pos, ref rot, ref scale);
			if (transformForm_.ShowDialog() == DialogResult.OK)
            {
				return transformForm_.GetValue();
            }
			return null;
        }

		/// <summary>
		/// a�̓��e�̃��X�g�{�b�N�X�̕������b�ɂ���
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		public void ChangeListBoxString(string a, string b)
		{
			ListBox.Items[ListBox.Items.IndexOf(a)] = b;
		}

		/// <summary>
		/// �I�𒆂̃��X�g�{�b�N�X�̕������a�ɒu��������
		/// </summary>
		/// <param name="a"></param>
		public void ChangeListBoxString(string a)
		{
			var select = ListBox.SelectedItem;
			ListBox.Items[ListBox.Items.IndexOf(select)] = a;
		}

		private void Position_Click(object sender, EventArgs e)
		{
			var result = GetOpenFilePathFromDialog("�t�@�C����I�����Ă�������");
			if (result != null)
			{
				var trans = new Transform(result);
				exports_.Add(trans);
				ListBox.Items.Add(trans.GetInfo());
			}
		}

		private void Collider_Click(object sender, EventArgs e)
		{
			if (openFileDialog1.ShowDialog() == DialogResult.OK)
			{
				var col = new Transform(openFileDialog1.FileName);
				exports_.Add(col);
				ListBox.Items.Add(col.GetInfo());
			}
		}

		private void Model_Click(object sender, EventArgs e)
		{
			var result = GetTextFromForm("���f���̃t�@�C���p�X����͂��Ă�������");
			if (result != null)
			{
				var model = new Model(result);
				exports_.Add(model);
				ListBox.Items.Add(model.GetInfo());
				textForm.ClearText();
			}
		}

		private void skyDome_Click(object sender, EventArgs e)
		{
			var result = GetOpenFilePathFromDialog("�g�����X�t�H�[���f�[�^��I�����Ă�������");
			if (result != null)
			{
				var  texResult = GetTextFromForm("�X�J�C�h�[���̃��f���̃p�X����͂��Ă�������");
				if (texResult != null)
				{
					var sky = new SpawnObj(result, result, SpawnObj.SpType.Sky);
					exports_.Add(sky);
					ListBox.Items.Add(sky.GetInfo());
					textForm.ClearText();
				}
			}
		}

		private void Player_Click(object sender, EventArgs e)
		{
			var result = GetOpenFilePathFromDialog("�g�����X�t�H�[���f�[�^��I�����Ă�������");
			if (result != null)
			{
				var textResult = GetTextFromForm("�v���C���[�̃f�[�^�t�@�C���̃p�X����͂��Ă�������");
				if (textResult != null)
				{
					var pl = new SpawnObj(result, textResult, SpawnObj.SpType.Player);
					exports_.Add(pl);
					ListBox.Items.Add(pl.GetInfo());
					textForm.ClearText();
				}
			}
		}

		private void primitiveToolStripMenuItem_Click(object sender, EventArgs e)
		{
			var result = GetOpenFilePathFromDialog("�R���C�_�[�f�[�^��I�����Ă�������");
			if (result != null)
			{
				var prim = new PrimitiveCollider(result);
				exports_.Add(prim);
				ListBox.Items.Add(prim.GetInfo());
			}
		}

		private void �o��ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (saveFileDialog1.ShowDialog() == DialogResult.OK)
			{
				var bw = new BinaryWriter(saveFileDialog1.OpenFile());
				var sig = new char[4];
				sig[0] = 'd';
				sig[1] = 'a';
				sig[2] = 't';
				sig[3] = 'a';
				bw.Write(sig);
				bw.Write(version_);
				bw.Write(0);
				foreach (var export in exports_)
				{
					export.Export(bw);
				}

				bw.Close();
				exports_.Clear();
				ListBox.Items.Clear();
			}
		}

		private void �ǂݍ���_Click(object sender, EventArgs e)
		{
			var result = GetOpenFilePathFromDialog("�ǂݍ��ރf�[�^��I�����Ă�������");
			if (result != null)
			{
				var file = File.OpenRead(result);
				var br = new BinaryReader(file);
				var sig = new char[4];

				for (int i = 0; i < 4; i++)
				{
					sig[i] = br.ReadChar();
					if (sig[i] != fileSig[i])
					{
						// �V�O�l�`������v���ĂȂ��Ƃ�
						return;
					}
				}

				var version = br.ReadUInt32();

				if (version != version_)
				{
					// �o�[�W�����s��v��
					return;
				}

				ListBox.Items.Clear();
				exports_.Clear();

				var checkValue = br.ReadInt32();
				IExport.Type type = (IExport.Type)br.ReadInt32();
				while (true)
				{
					br.ReadUInt32();
					if (loadFuncs.ContainsKey(type))
					{
						loadFuncs[type](br);
					}

					try
					{
						type = (IExport.Type)br.ReadInt32();
					}
					catch (Exception)
					{
						break;
					}
				}

				br.Close();
			}
		}

		private void LoadTransform(BinaryReader br)
		{
			var trans = new Transform(br);
			exports_.Add(trans);
			ListBox.Items.Add(trans.GetInfo());
		}

		private void LoadCollider(BinaryReader br)
		{
			var type = (ColliderBase.ColType)br.ReadInt32();
			IExport export;
			if (type != ColliderBase.ColType.Mesh)
			{
				export = new PrimitiveCollider(type, br);
			}
			else
			{
				export = new MeshCollider(type, br);
			}
			exports_.Add(export);
			ListBox.Items.Add(export.GetInfo());


		}

		private void LoadMoodel(BinaryReader br)
		{
			var model = new Model(br);
			exports_.Add(model);
			ListBox.Items.Add(model.GetInfo());

		}

		private void LoadSpawnObject(BinaryReader br)
		{
			var spObj = new SpawnObj(br);
			exports_.Add(spObj);
			ListBox.Items.Add(spObj.GetInfo());
		}

		private void LoadXmlParameter(BinaryReader br)
		{
			var xml = new XmlParameter(br);
			exports_.Add(xml);
			ListBox.Items.Add(xml.GetInfo());
		}

		private void �폜_Click(object sender, EventArgs e)
		{
			var select = ListBox.SelectedItem;

			exports_.RemoveAll(ex => ex.GetInfo() == (string)select);
			ListBox.Items.Remove(select);
		}

		private void EnemySpawn_Click(object sender, EventArgs e)
		{
			var result = GetOpenFilePathFromDialog("�g�����X�t�H�[���f�[�^��I�����Ă�������");
			if (result != null)
			{
				var textResult = GetTextFromForm("Enemy�̃f�[�^�t�@�C���̃p�X����͂��Ă�������");
				if (textResult != null)
				{
					var enemy = new SpawnObj(result, textResult, SpawnObj.SpType.Enemy);
					exports_.Add(enemy);
					ListBox.Items.Add(enemy.GetInfo());
					textForm.ClearText();
				}
			}
		}


		private void xmlParameterToolStripMenuItem_Click(object sender, EventArgs e)
		{
			var result = GetOpenFilePathFromDialog("�R���C�_�[�f�[�^��I�����Ă�������");
			if (result != null)
			{
				var xml = new XmlParameter(result);
				exports_.Add(xml);
				ListBox.Items.Add(xml.GetInfo());
			}
		}

		private void meshToolStripMenuItem_Click(object sender, EventArgs e)
		{
			var textResult = GetTextFromForm("���b�V���̃t�@�C���̃p�X����͂��Ă�������");
			if (textResult != null)
			{
				var meshCol = new MeshCollider(textResult);
				exports_.Add(meshCol);
				ListBox.Items.Add(meshCol.GetInfo());
				textForm.ClearText();
			}
		}

		private void �ҏWToolStripMenuItem_Click(object sender, EventArgs e)
		{
			var select = ListBox.SelectedItem;

			var export = exports_.Find(ex => ex.GetInfo() == (string)select);

			if (export != null)
			{
				export.Edit(this);
			}
		}
	}
}