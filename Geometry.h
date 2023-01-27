#pragma once

#include <vector>
#include <DirectXMath.h>

// 球やキューブなどの形状データ
namespace Geometry {

	template<class VertexType>
	struct Meshdata {
		std::vector<VertexType> vertices;
		std::vector<uint16_t> indices;

        std::vector<DirectX::XMFLOAT3> position;
        std::vector<DirectX::XMFLOAT3> normal;
        std::vector<DirectX::XMFLOAT2> texCoord;
	};

	// キューブの作成
	template<class VertexType>
	Meshdata<VertexType> CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f)
	{
		using namespace DirectX;

		Meshdata<VertexType> meshData = {};
		meshData.vertices.resize(24);
        meshData.position.resize(24);
        meshData.normal.resize(24);
        meshData.texCoord.resize(24);
        float halfWidth = width / 2.0f, halfHeight = height / 2.0f, halfDepth = depth / 2.0f;
        
        // 右面
        meshData.position[0]  = XMFLOAT3( halfWidth, -halfHeight, -halfDepth);
        meshData.position[1]  = XMFLOAT3( halfWidth,  halfHeight, -halfDepth);
        meshData.position[2]  = XMFLOAT3( halfWidth,  halfHeight,  halfDepth);
        meshData.position[3]  = XMFLOAT3( halfWidth, -halfHeight,  halfDepth);
        // 左面
        meshData.position[4]  = XMFLOAT3(-halfWidth, -halfHeight,  halfDepth);
        meshData.position[5]  = XMFLOAT3(-halfWidth,  halfHeight,  halfDepth);
        meshData.position[6]  = XMFLOAT3(-halfWidth,  halfHeight, -halfDepth);
        meshData.position[7]  = XMFLOAT3(-halfWidth, -halfHeight, -halfDepth);
        // 上面
        meshData.position[8]  = XMFLOAT3(-halfWidth,  halfHeight, -halfDepth);
        meshData.position[9]  = XMFLOAT3(-halfWidth,  halfHeight,  halfDepth);
        meshData.position[10] = XMFLOAT3( halfWidth,  halfHeight,  halfDepth);
        meshData.position[11] = XMFLOAT3( halfWidth,  halfHeight, -halfDepth);
        // 底面
        meshData.position[12] = XMFLOAT3( halfWidth, -halfHeight, -halfDepth);
        meshData.position[13] = XMFLOAT3( halfWidth, -halfHeight,  halfDepth);
        meshData.position[14] = XMFLOAT3(-halfWidth, -halfHeight,  halfDepth);
        meshData.position[15] = XMFLOAT3(-halfWidth, -halfHeight, -halfDepth);
        // 奥面
        meshData.position[16] = XMFLOAT3( halfWidth, -halfHeight,  halfDepth);
        meshData.position[17] = XMFLOAT3( halfWidth,  halfHeight,  halfDepth);
        meshData.position[18] = XMFLOAT3(-halfWidth,  halfHeight,  halfDepth);
        meshData.position[19] = XMFLOAT3(-halfWidth, -halfHeight,  halfDepth);
        // 正面
        meshData.position[20] = XMFLOAT3(-halfWidth, -halfHeight, -halfDepth);
        meshData.position[21] = XMFLOAT3(-halfWidth,  halfHeight, -halfDepth);
        meshData.position[22] = XMFLOAT3( halfWidth,  halfHeight, -halfDepth);
        meshData.position[23] = XMFLOAT3( halfWidth, -halfHeight, -halfDepth);

        for (UINT i = 0; i < 4; i++) {
            // 右面
            meshData.normal[i] = XMFLOAT3(1.0f, 0.0f, 0.0f);
            // 左面
            meshData.normal[i + 4] = XMFLOAT3(-1.0f, 0.0f, 0.0f);
            // 上面
            meshData.normal[i + 8] = XMFLOAT3(0.0f, 1.0f, 0.0f);
            // 底面
            meshData.normal[i + 12] = XMFLOAT3(0.0f, -1.0f, 0.0f);
            // 奥面
            meshData.normal[i + 16] = XMFLOAT3(0.0f, 0.0f, 1.0f);
            // 正面
            meshData.normal[i + 20] = XMFLOAT3(0.0f, 0.0f, -1.0f);
        }

        for (int i = 0; i < 6; i++) {
            meshData.texCoord[i * 4 + 0] = XMFLOAT2(0.0f, 1.0f);
            meshData.texCoord[i * 4 + 1] = XMFLOAT2(0.0f, 0.0f);
            meshData.texCoord[i * 4 + 2] = XMFLOAT2(1.0f, 0.0f);
            meshData.texCoord[i * 4 + 3] = XMFLOAT2(1.0f, 1.0f);
        }

        meshData.indices = {
            0, 1, 2, 2, 3, 0,		// 右面
            4, 5, 6, 6, 7, 4,		// 左面
            8, 9, 10, 10, 11, 8,	// 上面
            12, 13, 14, 14, 15, 12,	// 底面
            16, 17, 18, 18, 19, 16, // 奥面
            20, 21, 22, 22, 23, 20	// 正面
        };
        return meshData;
	}
}
