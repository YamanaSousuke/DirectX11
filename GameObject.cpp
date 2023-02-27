#include "GameObject.h"

using namespace DirectX;

// コンストラクタ
GameObject::GameObject(const Model& model)
{
	this->model = model;
}

// モデルの設定
void GameObject::SetModel(const Model& model)
{
	this->model = model;
}

// モデルの取得
Model& GameObject::GetModel()
{
	return model;
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
// void GameObject::SetMaterial(const Material& material)
// {
// 	this->material = material;
// }

// 描画
void GameObject::Draw(ID3D11DeviceContext* immediateContext, Effect& effect)
{
	auto& meshList = model.GetMeshData();
	for (auto& mesh : meshList) {
		// 頂点バッファーとインデックスバッファーの設定
		ID3D11Buffer* vertexBuffers[1] = { mesh.vertexBuffer.Get() };
		UINT strides[1] = { sizeof(VertexPositionNormalTextureColor) };
		UINT offsets[1] = { 0 };
		immediateContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, strides, offsets);
		immediateContext->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// 定数バッファーの設定
		effect.SetWorldMatrix(transform.GetWorldMatrix());
		effect.SetMaterial(mesh.material);
		effect.Apply(immediateContext);

		// テクスチャの設定
		ID3D11ShaderResourceView* textureViews[1] = { mesh.texture.Get() };
		immediateContext->PSSetShaderResources(0, 1, textureViews);
		// 描画
		immediateContext->DrawIndexed(static_cast<UINT>(mesh.indices.size()), 0, 0);
	}
}

// リソースの解放
void GameObject::Release()
{
	delete this;
}
