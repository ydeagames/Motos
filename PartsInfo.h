#pragma once

//======================================================
// File Name : PartsInfo.h
// Summary : アイテム情報クラス
// Date : 2019/11/18
// Author : Takafumi Ban
//======================================================
#pragma once
#include "GameObject.h"
#include "Moji.h"
#include "Blink.h"
#include <vector>
class Game;
class PartsInfo : public GameObject
{
public:
	// パーツの種類
	enum PARTS_KIND
	{
		POWER_PARTS,
		JUMP_PARTS,
		PARTS_KIND_NUM,
	};
	// 各パーツの最大使用可能数
	static const int PARTS_MAX_USE[PARTS_KIND_NUM];
public:
	// コンストラクタ
	PartsInfo();
	// 初期化関数
	void Initialize(ID3D11ShaderResourceView* texture, int x, int y, PARTS_KIND kind);
	// 更新関数
	virtual void Update(float elapsedTime);
	// 描画
	virtual void Render();
	// パーツ取得数をリセットする関数
	void Reset();
	// パーツ取得数を取得する関数
	void GetParts();
	// 使用するパーツの数を設定する関数
	bool SetUseCnt(int cnt);
	// 使用したパーツ数を取得する関数
	int GetUseCnt();
	// 使用するパーツを設定する時の点滅設定
	void SelectPartsMode(bool flag);
	// 取得数を設定する関数
	void SetGetCnt(int cnt);
	// 点滅のリセット関数
	void ResetBlink();
private:
	// テクスチャハンドル
	ID3D11ShaderResourceView* m_texture;
	// 表示位置
	int m_x, m_y;
	// パーツの種類
	PARTS_KIND m_kind;
	// 同時に使用できるパーツ数
	int m_max;
	// 保持パーツ数
	int m_get;
	// 現在使用中のパーツ数（0 以下の場合は表示なし）
	int m_use;
	// 取得数を文字列で表示するためのオブジェクト
	Moji m_cntStr;
	// 点滅管理オブジェクト
	std::unique_ptr<Blink> m_pBlink;
	// 点滅タイマー
	std::vector<int> m_blinkTimer;
	// 使用するパーツの数を設定中の点滅フラグ
	bool m_blinkFlag;
};