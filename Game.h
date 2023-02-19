#pragma once

#include "D3DApp.h"
#include "Camera.h"
#include "GameObject.h"

// アプリケーション全体を表す
class Game : public D3DApp
{
public:
	// コンストラクタ
	Game(LPCWSTR title, UINT width, UINT height);
	// 初期化
	bool Init();
	// シーンの更新
	void UpdateScene() override;
	// シーンの描画
	void DrawScene() override;
private:
	// カメラ
	Camera camera = {};
	// エフェクト
	Effect effect = {};
	// FBXファイルの読み込み
	FbxMeshFile fbxMeshfile = {};
	//	オブジェクト
	GameObject house;

	// フォグについての設定
	bool fogEnable = false;
	DirectX::XMFLOAT3 fogColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	float fogStart = 0.0f;
	float fogEnd = 40.0f;

	// タイマー
	float time = 0.0f;
};