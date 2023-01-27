#pragma once

#include <vector>
#include <DirectXMath.h>

// ����L���[�u�Ȃǂ̌`��f�[�^
namespace Geometry {

	template<class VertexType>
	struct Meshdata {
		std::vector<VertexType> vertices;
		std::vector<uint16_t> indices;
	};

	// ���_�f�[�^
	struct VertexData {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT4 color;
	};

	// �L���[�u�̍쐬
	template<class VertexType>
	Meshdata<VertexType> CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f)
	{
		using namespace DirectX;

		Meshdata<VertexType> meshData = {};
		meshData.vertices.resize(24);

		VertexData vertexData[24] = {};
        float halfWidth = width / 2.0f, halfHeight = height / 2.0f, halfDepth = depth / 2.0f;

        // �E��
        vertexData[0].position = XMFLOAT3(halfWidth, -halfHeight, -halfDepth);
        vertexData[1].position = XMFLOAT3(halfWidth, halfHeight, -halfDepth);
        vertexData[2].position = XMFLOAT3(halfWidth, halfHeight, halfDepth);
        vertexData[3].position = XMFLOAT3( halfWidth, -halfHeight,  halfDepth);
        // ����
        vertexData[4].position = XMFLOAT3(-halfWidth, -halfHeight, halfDepth);
        vertexData[5].position = XMFLOAT3(-halfWidth, halfHeight, halfDepth);
        vertexData[6].position = XMFLOAT3(-halfWidth, halfHeight, -halfDepth);
        vertexData[7].position = XMFLOAT3(-halfWidth, -halfHeight, -halfDepth);
        // ���
        vertexData[8].position = XMFLOAT3(-halfWidth, halfHeight, -halfDepth);
        vertexData[9].position = XMFLOAT3(-halfWidth, halfHeight, halfDepth);
        vertexData[10].position = XMFLOAT3(halfWidth, halfHeight, halfDepth);
        vertexData[11].position = XMFLOAT3( halfWidth,  halfHeight, -halfDepth);
        // ���
        vertexData[12].position = XMFLOAT3(halfWidth, -halfHeight, -halfDepth);
        vertexData[13].position = XMFLOAT3(halfWidth, -halfHeight, halfDepth);
        vertexData[14].position = XMFLOAT3(-halfWidth, -halfHeight, halfDepth);
        vertexData[15].position = XMFLOAT3(-halfWidth, -halfHeight, -halfDepth);
        // ����
        vertexData[16].position = XMFLOAT3(halfWidth, -halfHeight, halfDepth);
        vertexData[17].position = XMFLOAT3(halfWidth, halfHeight, halfDepth);
        vertexData[18].position = XMFLOAT3(-halfWidth, halfHeight, halfDepth);
        vertexData[19].position = XMFLOAT3(-halfWidth, -halfHeight,  halfDepth);
        // ����
        vertexData[20].position = XMFLOAT3(-halfWidth, -halfHeight, -halfDepth);
        vertexData[21].position = XMFLOAT3(-halfWidth, halfHeight, -halfDepth);
        vertexData[22].position = XMFLOAT3(halfWidth, halfHeight, -halfDepth);
        vertexData[23].position = XMFLOAT3( halfWidth, -halfHeight, -halfDepth);
	}
}
