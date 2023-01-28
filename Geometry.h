#pragma once

#include <vector>
#include <map>
#include <string>

#include <functional>

#include <DirectXMath.h>
#include "Vertex.h"

// ����L���[�u�Ȃǂ̌`��f�[�^
namespace Geometry {

    // ���_�f�[�^
    struct VertexData {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT2 texCoord;
    };
    
    // ���_�z��ɗv�f��ݒ肷��
    template<class VertexType>
    void InsertVertexElement(VertexType& vertexDst, const VertexData& vertexSrc)
    {
        static std::string semanticName;
        static const std::map<std::string, std::pair<size_t, size_t>> semanticSizeMap = {
            {"POSITION", std::pair<size_t, size_t>(0, 12)},
            {"NORMAL", std::pair<size_t, size_t>(12, 24)},
            {"TEXCOORD", std::pair<size_t, size_t>(24, 32)}
        };
    
        // �Ή�����Z�}���e�B�N�X������v�f���Z�b�g����
        for (size_t i = 0; i < ARRAYSIZE(VertexType::inputLayout); i++)
        {
            semanticName = VertexType::inputLayout[i].SemanticName;
            const auto& range = semanticSizeMap.at(semanticName);
            memcpy_s(reinterpret_cast<char*>(&vertexDst) + VertexType::inputLayout[i].AlignedByteOffset, range.second - range.first,
                reinterpret_cast<const char*>(&vertexSrc) + range.first, range.second - range.first);
        }
    }

    // ���_�z��
    template<class VertexType = VertexPositionNormalTexture>
    struct Meshdata {
        std::vector<VertexType> vertices;
        std::vector<uint16_t> indices;
    };
    
    // �L���[�u�̍쐬
    template<class VertexType = VertexPositionNormalTexture>
    Meshdata<VertexType> CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f);

    // ���̍쐬
    template<class VertexType = VertexPositionNormalTexture>
    Meshdata<VertexType> CreateSphere(float radius = 1.25f, UINT levels = 20, UINT slices = 20);

	// �L���[�u�̍쐬
    template<class VertexType>
    inline Meshdata<VertexType> CreateBox(float width, float height, float depth)
	{
		using namespace DirectX;

        // ������
		Meshdata<VertexType> meshData = {};
		meshData.vertices.resize(24);
        VertexData vertexData[24] = {};
        float halfWidth = width / 2.0f, halfHeight = height / 2.0f, halfDepth = depth / 2.0f;
        
        // ���W
        // �E��
        vertexData[0].position = XMFLOAT3(halfWidth, -halfHeight, -halfDepth);
        vertexData[1].position = XMFLOAT3(halfWidth, halfHeight, -halfDepth);
        vertexData[2].position = XMFLOAT3(halfWidth, halfHeight, halfDepth);
        vertexData[3].position  = XMFLOAT3( halfWidth, -halfHeight,  halfDepth);
        // ����
        vertexData[4].position = XMFLOAT3(-halfWidth, -halfHeight, halfDepth);
        vertexData[5].position = XMFLOAT3(-halfWidth, halfHeight, halfDepth);
        vertexData[6].position = XMFLOAT3(-halfWidth, halfHeight, -halfDepth);
        vertexData[7].position  = XMFLOAT3(-halfWidth, -halfHeight, -halfDepth);
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

        // �@��
        for (UINT i = 0; i < 4; i++) {
            vertexData[i + 0].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
            vertexData[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
            vertexData[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
            vertexData[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
            vertexData[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
            vertexData[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
        }

        // uv���W
        for (UINT i = 0; i < 6; i++) {
            vertexData[i * 4 + 0].texCoord = XMFLOAT2(0.0f, 1.0f);
            vertexData[i * 4 + 1].texCoord = XMFLOAT2(0.0f, 0.0f);
            vertexData[i * 4 + 2].texCoord = XMFLOAT2(1.0f, 0.0f);
            vertexData[i * 4 + 3].texCoord = XMFLOAT2(1.0f, 1.0f);
        }

        // ���_�z��ɗv�f��ݒ肷��
        for (int i = 0; i < 24; i++)
        {
            InsertVertexElement(meshData.vertices[i], vertexData[i]);
        }

        // �C���f�b�N�X
        meshData.indices = {
            0, 1, 2, 2, 3, 0,		// �E��
            4, 5, 6, 6, 7, 4,		// ����
            8, 9, 10, 10, 11, 8,	// ���
            12, 13, 14, 14, 15, 12,	// ���
            16, 17, 18, 18, 19, 16, // ����
            20, 21, 22, 22, 23, 20	// ����
        };
        return meshData;
	}

    // ���̍쐬
    template<class VertexType>
    inline Meshdata<VertexType> CreateSphere(float radius, UINT levels, UINT slices)
    {
        using namespace DirectX;
        
        // ������
        Meshdata<VertexType> meshData = {};
        UINT vertexCount = 2 + (levels - 1) * (slices + 1);
        UINT indexCount = 6 * (levels - 1) * slices;
        UINT vIndex = 0, iIndex = 0;
        meshData.vertices.resize(vertexCount);
        meshData.indices.resize(indexCount);
        VertexData vertexData = {};

        // leves��slices���オ��قǐ��x�����シ��
        float phi = 0.0f, theta = 0.0f;
        float perPhi = XM_PI / levels;
        float perTheta = XM_2PI / slices;
        float x = 0.0f, y = 0.0f, z = 0.0f;


        // ��ԏ�̒��_
        vertexData = { XMFLOAT3(0.0f, radius, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
        InsertVertexElement(meshData.vertices[vIndex++], vertexData);

        for (UINT i = 1; i < levels; i++) {
            phi = perPhi * i;
            for (UINT j = 0; j <= slices; j++) {
                theta = perTheta * j;

                x = radius * sinf(phi) * cosf(theta);
                y = radius * cosf(phi);
                z = radius * sinf(phi) * sinf(theta);

                // �@���͍��W�𐳋K���������̂��g�p����
                XMFLOAT3 position = XMFLOAT3(x, y, z);
                XMFLOAT3 normal;
                XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&position)));
                vertexData = { position, normal, XMFLOAT2(theta / XM_2PI, phi / XM_PI) };
                InsertVertexElement(meshData.vertices[vIndex++], vertexData);
            }
        }

        // ��ԉ��̒��_
        vertexData = { XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
        InsertVertexElement(meshData.vertices[vIndex++], vertexData);

        // �C���f�b�N�X�̐ݒ�
        if (levels > 1)
        {
            for (UINT j = 1; j <= slices; ++j)
            {
                meshData.indices[iIndex++] = 0;
                meshData.indices[iIndex++] = j % (slices + 1) + 1;
                meshData.indices[iIndex++] = j;
            }
        }
        for (UINT i = 1; i < levels - 1; ++i)
        {
            for (UINT j = 1; j <= slices; ++j)
            {
                meshData.indices[iIndex++] = (i - 1) * (slices + 1) + j;
                meshData.indices[iIndex++] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
                meshData.indices[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;

                meshData.indices[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;
                meshData.indices[iIndex++] = i * (slices + 1) + j;
                meshData.indices[iIndex++] = (i - 1) * (slices + 1) + j;
            }
        }
        if (levels > 1)
        {
            for (UINT j = 1; j <= slices; ++j)
            {
                meshData.indices[iIndex++] = (levels - 2) * (slices + 1) + j;
                meshData.indices[iIndex++] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
                meshData.indices[iIndex++] = (levels - 1) * (slices + 1) + 1;
            }
        }

        return meshData;
    }
}
