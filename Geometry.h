#pragma once

#include <vector>
#include <map>
#include <string>

#include <functional>

#include <DirectXMath.h>
#include "Vertex.h"

namespace Geometry {
    template<class VertexType = VertexPositionNormalTexture>
    struct Meshdata {
        std::vector<VertexType> vertices;
        std::vector<uint16_t> indices;
    };

    // キューブの作成
    template<class VertexType = VertexPositionNormalTexture>
    Meshdata<VertexType> CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f);
}


// 球やキューブなどの形状データ
namespace Geometry {
    namespace Internal {
        // 頂点データ
        struct VertexData {
            DirectX::XMFLOAT3 position;
            DirectX::XMFLOAT3 normal;
            DirectX::XMFLOAT2 texCoord;
        };

        // 頂点配列に要素を設定する
        template<class VertexType>
        void InsertVertexElement(VertexType& vertexDst, const VertexData& vertexSrc)
        {
            static std::string semanticName;
            static const std::map<std::string, std::pair<size_t, size_t>> semanticSizeMap = {
                {"POSITION", std::pair<size_t, size_t>(0, 12)},
                {"NORMAL", std::pair<size_t, size_t>(12, 24)},
                {"TEXCOORD", std::pair<size_t, size_t>(24, 32)}
            };

            // 対応するセマンティクス名から要素をセットする
            for (size_t i = 0; i < ARRAYSIZE(VertexType::inputLayout); i++)
            {
                semanticName = VertexType::inputLayout[i].SemanticName;
                const auto& range = semanticSizeMap.at(semanticName);
                memcpy_s(reinterpret_cast<char*>(&vertexDst) + VertexType::inputLayout[i].AlignedByteOffset, range.second - range.first,
                    reinterpret_cast<const char*>(&vertexSrc) + range.first, range.second - range.first);
            }
        }
    }


	// キューブの作成
    template<class VertexType>
    inline Meshdata<VertexType> CreateBox(float width, float height, float depth)
	{
		using namespace DirectX;

		Meshdata<VertexType> meshData = {};
		meshData.vertices.resize(24);
        
        Internal::VertexData vertexData[24] = {};
        float halfWidth = width / 2.0f, halfHeight = height / 2.0f, halfDepth = depth / 2.0f;
        
        // 右面
        vertexData[0].position = XMFLOAT3(halfWidth, -halfHeight, -halfDepth);
        vertexData[1].position = XMFLOAT3(halfWidth, halfHeight, -halfDepth);
        vertexData[2].position = XMFLOAT3(halfWidth, halfHeight, halfDepth);
        vertexData[3].position  = XMFLOAT3( halfWidth, -halfHeight,  halfDepth);
        // 左面
        vertexData[4].position = XMFLOAT3(-halfWidth, -halfHeight, halfDepth);
        vertexData[5].position = XMFLOAT3(-halfWidth, halfHeight, halfDepth);
        vertexData[6].position = XMFLOAT3(-halfWidth, halfHeight, -halfDepth);
        vertexData[7].position  = XMFLOAT3(-halfWidth, -halfHeight, -halfDepth);
        // 上面
        vertexData[8].position = XMFLOAT3(-halfWidth, halfHeight, -halfDepth);
        vertexData[9].position = XMFLOAT3(-halfWidth, halfHeight, halfDepth);
        vertexData[10].position = XMFLOAT3(halfWidth, halfHeight, halfDepth);
        vertexData[11].position = XMFLOAT3( halfWidth,  halfHeight, -halfDepth);
        // 底面
        vertexData[12].position = XMFLOAT3(halfWidth, -halfHeight, -halfDepth);
        vertexData[13].position = XMFLOAT3(halfWidth, -halfHeight, halfDepth);
        vertexData[14].position = XMFLOAT3(-halfWidth, -halfHeight, halfDepth);
        vertexData[15].position = XMFLOAT3(-halfWidth, -halfHeight, -halfDepth);
        // 奥面
        vertexData[16].position = XMFLOAT3(halfWidth, -halfHeight, halfDepth);
        vertexData[17].position = XMFLOAT3(halfWidth, halfHeight, halfDepth);
        vertexData[18].position = XMFLOAT3(-halfWidth, halfHeight, halfDepth);
        vertexData[19].position = XMFLOAT3(-halfWidth, -halfHeight,  halfDepth);
        // 正面
        vertexData[20].position = XMFLOAT3(-halfWidth, -halfHeight, -halfDepth);
        vertexData[21].position = XMFLOAT3(-halfWidth, halfHeight, -halfDepth);
        vertexData[22].position = XMFLOAT3(halfWidth, halfHeight, -halfDepth);
        vertexData[23].position = XMFLOAT3( halfWidth, -halfHeight, -halfDepth);

         for (UINT i = 0; i < 4; i++) {
             // 右面
             vertexData[i + 0].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
             // 左面
             vertexData[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
             // 上面
             vertexData[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
             // 底面
             vertexData[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
             // 奥面
             vertexData[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
             // 正面
             vertexData[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
         }

        for (int i = 0; i < 6; i++) {
            vertexData[i * 4 + 0].texCoord = XMFLOAT2(0.0f, 1.0f);
            vertexData[i * 4 + 1].texCoord = XMFLOAT2(0.0f, 0.0f);
            vertexData[i * 4 + 2].texCoord = XMFLOAT2(1.0f, 0.0f);
            vertexData[i * 4 + 3].texCoord = XMFLOAT2(1.0f, 1.0f);
        }

        for (int i = 0; i < 24; i++)
        {
            Internal::InsertVertexElement(meshData.vertices[i], vertexData[i]);
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
