#include "pch.h"
#include "File.h"
#include <limits>

using namespace std;

//--------------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------------
File::File(const wchar_t *fname)
: length(0)
, data(nullptr)
{
	ifstream in(fname, ios::in | ios::binary);

	// �I�[�v�������H
	_ASSERT(in);

	// �t�@�C���T�C�Y���擾
	ios::pos_type tmp = in.tellg();
	ios::off_type size_ = in.seekg(0, ios::end).tellg() - in.seekg(0, ios::beg).tellg();
	in.seekg(tmp);

	// �t�@�C���T�C�Y��int�ŕ\����ő�l�𒴂��Ă��Ȃ��H
	_ASSERT(!(std::numeric_limits<int>::max() < size_));
	length = static_cast<int>(size_);

	//���������m�ۂ��ēǂݍ���
	data = new char[length];
	in.read(data, length);

	// �N���[�Y
	in.close();
}

//--------------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------------
File::~File()
{
	if (data)
	{
		delete[] data;
	}
}

