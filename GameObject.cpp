#include "GameObject.h"

using namespace DirectX;

// モデルの設定
void GameObject::SetModel(const FbxMeshFile* model)
{
	this->model = model;
}

// トランスフォームの取得
Transform& GameObject::GetTransform()
{
	return transform;
}

// テクスチャの設定
void GameObject::SetTexture(ID3D11ShaderResourceView* texture)
{
	this->texture = texture;
}

// マテリアルの設定
void GameObject::SetMaterial(const Material& material)
{
	this->material = material;
}

// 描画
void GameObject::Draw(ID3D11DeviceContext* immediateContext, Effect& effect)
{
	static float time = 0.0f;
	time += 0.01666f;

	auto meshList = model->GetMeshData();
	for (auto& mesh : meshList) {
		// 頂点バッファーとインデックスバッファーの設定
		ID3D11Buffer* vertexBuffers[1] = { mesh.vertexBuffer.Get() };
		UINT strides[1] = { sizeof(VertexPositionNormalTextureColor) };
		UINT offsets[1] = { 0 };
		immediateContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, strides, offsets);
		immediateContext->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		transform.SetRotation(0.0f, XM_PIDIV2, 0.0f);
		transform.SetScale(0.05f, 0.05f, 0.05f);
		transform.SetPosition(0.0f, 0.0f ,0.0f);
		effect.SetWorldMatrix(transform.GetWorldMatrix());
		effect.SetMaterial(mesh.material);
		effect.Apply(immediateContext);

		// テクスチャの設定
		ID3D11ShaderResourceView* textureViews[1] = { mesh.texture.Get() };
		immediateContext->PSSetShaderResources(0, 1, textureViews);
		immediateContext->DrawIndexed(static_cast<UINT>(mesh.indices.size()), 0, 0);
	}
}

// リソースの解放
void GameObject::Release()
{
	delete this;
}
