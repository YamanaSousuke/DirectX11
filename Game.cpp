#include "Game.h"

#include <iostream>

using namespace Microsoft::WRL;
using namespace DirectX;

// コンストラクター
Game::Game(LPCWSTR title, UINT width, UINT height) : D3DApp(title, width, height) { }

// 初期化
bool Game::Init()
{
	// デバイス、ウィンドウの初期化
	if (!D3DApp::Init()) {
		return false;
	}

	// FBXファイル読み込み機能の初期化
	fbxMeshfile.Init(device.Get());

	// サンプラー、ブレンド、ラスタライザステートの初期化
	if (!RenderState::InitAll(device.Get())) {
		return false;
	}

	// シェーダーと定数バッファーの初期化
	if (!effect.InitAll(device.Get())) {
		return false;
	}

	// ビュー行列の設定
	XMVECTOR eyePosition = XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
	XMVECTOR focusPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	camera.SetViewMatrix(eyePosition, focusPosition, upDirection);

	// プロジェクション行列の設定
	auto fovAngle = 60.0f;
	auto aspectRatio = AspectRatio();
	auto nearZ = 0.3f;
	auto farZ = 1000.0f;
	camera.SetProjectionMatrix(XMConvertToRadians(fovAngle), aspectRatio, nearZ, farZ);

	// FBXモデルの読み込み
	auto model = fbxMeshfile.Load("Models/House1/House1.fbx", deviceContext.Get());
	house.SetModel(model);
	house.GetTransform().SetScale(0.04f, 0.04f, 0.04f);
	house.GetTransform().SetPosition(0.5f, 0.0f, 0.0f);
	return true;
}

// シーンの更新
void Game::UpdateScene()
{
	time += 0.016f;
	// フォグについての説明
	if (ImGui::Begin("Fog")) {
		ImGui::Checkbox("Enable Fog", &fogEnable);
		ImGui::ColorEdit3("Color", &fogColor.x);
		ImGui::DragFloat("Fog Start", &fogStart, 0.05f, 0.0f, 0.0f, "%.3f");
		if (fogStart >= fogEnd) {
			fogEnd = fogStart;
		}
		ImGui::DragFloat("Fog End", &fogEnd, 0.05f, 0.0f, 0.0f, "%.3f");
	}
	ImGui::End();
	
	// モデルについての説明
	ImGui::Begin("Models");
	ImGui::Text("modelName : %s", house.GetModel().GetModelName());
	
	// トランスフォームの取得 
	auto position = house.GetTransform().GetPosition();
	auto rotation = house.GetTransform().GetRotationInDegree();
	auto scale = house.GetTransform().GetScale();
	
	// トランスフォームの設定
	if (ImGui::TreeNode("Transform")) {
		ImGui::Text("Position");
		ImGui::DragFloat3("##position", &position.x, 0.05f, 0.0f, 0.0f, "%.3f");
		ImGui::Text("Rotation");
		ImGui::Text("x : %.3f, y : %.3f, z : %.3f", rotation.x, rotation.y, rotation.z);
		ImGui::Text("Scale");
		ImGui::DragFloat3("##scale", &scale.x, 0.05f, 0.0f, 0.0f, "%.3f");
		ImGui::TreePop();
	}
	
	// トランスフォームの更新
	house.GetTransform().SetPosition(position);
	house.GetTransform().SetRotation(0.0f, time * 0.25f, 0.0f);
	house.GetTransform().SetScale(scale);
	
	auto& meshList = house.GetModel().GetMeshData();
	// マテリアルについての説明
	if (ImGui::TreeNode("Material")) {
		 
		for (auto& mesh : meshList) {
			ImGui::Text("material : %s", mesh.materialName.c_str());
		}
		ImGui::TreePop();
	}
	// テクスチャーについての説明
	if (ImGui::TreeNode("Texture")) {
		for (auto& mesh : meshList) {
			if (!mesh.textureName.empty()) {
				ImGui::Text("texture : %s", mesh.textureName.c_str());
			}
		}
		ImGui::TreePop();
	}
	
	// マテリアルのパラメーターの設定
	if (ImGui::TreeNode("Material Parameter")) {
		static float smooth = 0.0f;
		static float metallic = 0.0f;
		ImGui::Text("smooth");
		ImGui::SliderFloat("##smooth", &smooth, 0.0f, 1.0f, "%.2f");
		ImGui::Text("metallic");
		ImGui::SliderFloat("##metallic", &metallic, 0.0f, 1.0f, "%.2f");
		for (auto& mesh : meshList) {
			mesh.material.smooth = smooth;
			mesh.material.metallic = metallic;
		}
		ImGui::TreePop();
	}

	// 座標と拡大率のリセット
	if (ImGui::Button("Reset")) {
		house.GetTransform().SetPosition(0.0f, 0.0f, 0.0f);
		house.GetTransform().SetScale(0.04f, 0.04f, 0.04f);
	}

	ImGui::End();
	ImGui::Render();
	
	// ビュー行列、プロジェクション行列の設定
	effect.SetViewMatrix(camera.GetViewMatrix());
	effect.SetProjectionMatrix(camera.GetProjectionMatrix());
	
	// ライト
	DirectionalLight directionalLight[4] = {};
	directionalLight[0].diffuse = XMFLOAT4(1.0f, 0.84f, 0.0f, 1.0f);
	directionalLight[0].ambient = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	directionalLight[0].direction = XMFLOAT4(-1.0f, -1.0f, 2.0f, 0.0f);
	directionalLight[1].diffuse = XMFLOAT4(1.0f, 0.84f, 0.0f, 1.0f);
	directionalLight[1].ambient = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	directionalLight[1].direction = XMFLOAT4(-1.0f, -1.0f, 0.0f, 0.0f);
	directionalLight[2].diffuse = XMFLOAT4(1.0f, 0.84f, 0.0f, 1.0f);
	directionalLight[2].ambient = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	directionalLight[2].direction = XMFLOAT4(-1.0f, -1.0f, -2.0f, 0.0f);
	for (UINT i = 0; i < ARRAYSIZE(directionalLight); i++) {
		effect.SetDirectionalLight(i, directionalLight[i]);
	}

	// 視点
	effect.SetEyePosition(camera.GetTransform().GetPosition());
	
	// フォグについての設定
	effect.SetFogColor(fogColor);
	effect.SetFogState(fogEnable);
	effect.SetFogStart(fogStart);
	effect.SetFogRange(abs(fogEnd - fogStart));
}

// シーンの描画
void Game::DrawScene()
{
	// レンダーターゲットを設定
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
	// 画面のクリア
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), backgroundColor);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext->RSSetViewports(ARRAYSIZE(viewports), viewports);

	// 描画
	effect.SetTime(time);
	effect.RenderDefault(deviceContext.Get());
	effect.Apply(deviceContext.Get());
	house.Draw(deviceContext.Get(), effect);

	// 表示
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	swapchain->Present(1, 0);
}
