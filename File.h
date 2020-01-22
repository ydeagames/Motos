#pragma once

#include <fstream>

// ファイル管理クラス
class File
{
public:
	// サイズ
	int length;

	// 読み込み先のバッファ
	char *data;

public:
	File(const wchar_t *fname);
	~File();
};
