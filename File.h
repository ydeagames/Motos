#pragma once

#include <fstream>

// �t�@�C���Ǘ��N���X
class File
{
public:
	// �T�C�Y
	int length;

	// �ǂݍ��ݐ�̃o�b�t�@
	char *data;

public:
	File(const wchar_t *fname);
	~File();
};
