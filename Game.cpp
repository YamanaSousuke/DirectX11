#include "Game.h"

#include <iostream>

using namespace Microsoft::WRL;
using namespace DirectX;

// �R���X�g���N�^�[
Game::Game(LPCWSTR title, UINT width, UINT height) : D3DApp(title, width, height) { }

// ������
bool Game::Init()
{
	// �f�o�C�X�A�E�B���h�E�̏�����
	if (!D3DApp::Init()) {
		return false;
	}

	// FBX�t�@�C���ǂݍ��݋@�\�̏�����
	fbxMeshfile.Init(device.Get());

	// �T���v���[�A�u�����h�A���X�^���C�U�X�e�[�g�̏�����
	if (!RenderState::InitAll(device.Get())) {
		return false;
	}

	// �V�F�[�_�[�ƒ萔�o�b�t�@�[�̏�����
	if (!effect.InitAll(device.Get())) {
		return false;
	}

	// �r���[�s��̐ݒ�
	XMVECTOR eyePosition = XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
	XMVECTOR focusPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	camera.SetViewMatrix(eyePosition, focusPosition, upDirection);

	// �v���W�F�N�V�����s��̐ݒ�
	auto fovAngle = 60.0f;
	auto aspectRatio = AspectRatio();
	auto nearZ = 0.3f;
	auto farZ = 1000.0f;
	camera.SetProjectionMatrix(XMConvertToRadians(fovAngle), aspectRatio, nearZ, farZ);

	// FBX���f���̓ǂݍ���
	auto model = fbxMeshfile.Load("Models/House1/House1.fbx", deviceContext.Get());
	house.SetModel(model);
	house.GetTransform().SetScale(0.04f, 0.04f, 0.04f);
	house.GetTransform().SetPosition(0.5f, 0.0f, 0.0f);
	return true;
}

// �V�[���̍X�V
void Game::UpdateScene()
{
	time += 0.016f;
	// �t�H�O�ɂ��Ă̐���
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
	
	// ���f���ɂ��Ă̐���
	ImGui::Begin("Models");
	ImGui::Text("modelName : %s", house.GetModel().GetModelName());
	
	// �g�����X�t�H�[���̎擾 
	auto position = house.GetTransform().GetPosition();
	auto rotation = house.GetTransform().GetRotationInDegree();
	auto scale = house.GetTransform().GetScale();
	
	// �g�����X�t�H�[���̐ݒ�
	if (ImGui::TreeNode("Transform")) {
		ImGui::Text("Position");
		ImGui::DragFloat3("##position", &position.x, 0.05f, 0.0f, 0.0f, "%.3f");
		ImGui::Text("Rotation");
		ImGui::Text("x : %.3f, y : %.3f, z : %.3f", rotation.x, rotation.y, rotation.z);
		ImGui::Text("Scale");
		ImGui::DragFloat3("##scale", &scale.x, 0.05f, 0.0f, 0.0f, "%.3f");
		ImGui::TreePop();
	}
	
	// �g�����X�t�H�[���̍X�V
	house.GetTransform().SetPosition(position);
	house.GetTransform().SetRotation(0.0f, time * 0.25f, 0.0f);
	house.GetTransform().SetScale(scale);
	
	auto& meshList = house.GetModel().GetMeshData();
	// �}�e���A���ɂ��Ă̐���
	if (ImGui::TreeNode("Material")) {
		 
		for (auto& mesh : meshList) {
			ImGui::Text("material : %s", mesh.materialName.c_str());
		}
		ImGui::TreePop();
	}
	// �e�N�X�`���[�ɂ��Ă̐���
	if (ImGui::TreeNode("Texture")) {
		for (auto& mesh : meshList) {
			if (!mesh.textureName.empty()) {
				ImGui::Text("texture : %s", mesh.textureName.c_str());
			}
		}
		ImGui::TreePop();
	}
	
	// �}�e���A���̃p�����[�^�[�̐ݒ�
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

	// ���W�Ɗg�嗦�̃��Z�b�g
	if (ImGui::Button("Reset")) {
		house.GetTransform().SetPosition(0.0f, 0.0f, 0.0f);
		house.GetTransform().SetScale(0.04f, 0.04f, 0.04f);
	}

	ImGui::End();
	ImGui::Render();
	
	// �r���[�s��A�v���W�F�N�V�����s��̐ݒ�
	effect.SetViewMatrix(camera.GetViewMatrix());
	effect.SetProjectionMatrix(camera.GetProjectionMatrix());
	
	// ���C�g
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

	// ���_
	effect.SetEyePosition(camera.GetTransform().GetPosition());
	
	// �t�H�O�ɂ��Ă̐ݒ�
	effect.SetFogColor(fogColor);
	effect.SetFogState(fogEnable);
	effect.SetFogStart(fogStart);
	effect.SetFogRange(abs(fogEnd - fogStart));
}

// �V�[���̕`��
void Game::DrawScene()
{
	// �����_�[�^�[�Q�b�g��ݒ�
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
	// ��ʂ̃N���A
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), backgroundColor);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext->RSSetViewports(ARRAYSIZE(viewports), viewports);

	// �`��
	effect.SetTime(time);
	effect.RenderDefault(deviceContext.Get());
	effect.Apply(deviceContext.Get());
	house.Draw(deviceContext.Get(), effect);

	// �\��
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	swapchain->Present(1, 0);
}
