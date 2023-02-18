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
	// メッセージループの実行
	int Run();

private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// ウィンドウのタイトル
	LPCWSTR title = L"Title";
	// ウィンドウの幅
	int width = 640;
	// ウィンドウの高さ
	int height = 480;

	// カメラ
	Camera camera = {};
	// エフェクト
	Effect effect = {};
	// FBX
	FbxMeshFile fbxMeshfile = {};

	// フォグについての設定
	bool fogEnable = false;
	DirectX::XMFLOAT3 fogColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	float fogStart = 0.0f;
	float fogEnd = 40.0f;
	// GUIの初期化
	bool InitGUI();
};