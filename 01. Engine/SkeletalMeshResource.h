#pragma once
#include "ResourceBase.h"

namespace Engine
{
    // ���� : Ŭ���� ����
    class Material;
    class SkeletonResource;
    class Node;


    // ���� : �� ����ġ�� �޴� ���ؽ� ����ü
    struct BoneWeightVertex
    {
        Vector3 m_Position;
        Vector2 m_TexCoord;
        Vector3 m_Normal;
        Vector3 m_Tangent;
        Vector3 m_BiTangents;

        int m_BlendIndeces[4] = {};
        float m_BlendWeights[4] = {};

        void AddBoneData(int boneIndex, float weight)
        {
            assert(m_BlendWeights[0] == 0.0f || m_BlendWeights[1] == 0.f || m_BlendWeights[2] == 0.f || m_BlendWeights[3] == 0.f);

            for (int i = 0; i < 4; i++)
            {
                if (m_BlendWeights[i] == 0.f)
                {
                    m_BlendIndeces[i] = boneIndex;
                    m_BlendWeights[i] = weight;
                    return;
                }
            }
        }
    };


    // -------------------------------------------------------------------------
    // ���� : ���� ���� ���̷�Ż �޽� ���ҽ� Ŭ����
    class SkeletalMesh
    {
    public:
        SkeletalMesh();
        virtual ~SkeletalMesh();

    public:
        void Create(aiMesh* _aiMesh, shared_ptr<SkeletonResource> _skeleton);

    private:
        void createBuffer();

    public:
        inline const string& GetName();
        inline vector<BoneWeightVertex> GetVertices();
        inline vector<UINT> GetIndices();
        inline ComPtr<ID3D11Buffer> GetVertexBuffer();
        inline ComPtr<ID3D11Buffer> GetIndexBuffer();
        inline void SetMaterial(shared_ptr<Material> _material);
        inline shared_ptr<Material> GetMaterial();

    private:
        string m_Name;

        vector<BoneWeightVertex> m_BoneWeightVertexVec;
        vector<UINT> m_IndexVec;
        shared_ptr<Material> m_pMaterial;

        ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
        ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
    };

    // ���� : SkeletalMesh�� Get & Set �Լ�
    const string& SkeletalMesh::GetName()
    {
        return m_Name;
    }
    vector<BoneWeightVertex> SkeletalMesh::GetVertices()
    {
        return m_BoneWeightVertexVec;
    }
    vector<UINT> SkeletalMesh::GetIndices()
    {
        return m_IndexVec;
    }
    ComPtr<ID3D11Buffer> SkeletalMesh::GetVertexBuffer()
    {
        return m_pVertexBuffer;
    }
    ComPtr<ID3D11Buffer> SkeletalMesh::GetIndexBuffer()
    {
        return m_pIndexBuffer;
    }
    void SkeletalMesh::SetMaterial(shared_ptr<Material> _material)
    {
        m_pMaterial = _material;
    }
    shared_ptr<Material> SkeletalMesh::GetMaterial()
    {
        return m_pMaterial;
    }
    // -------------------------------------------------------------------------


    // -------------------------------------------------------------------------
    // ���� : FBX ���� ��� ���̷�Ż �޽� ���ҽ� ������ ������ �ִ� ���ҽ� Ŭ����
    class SkeletalMeshSceneResource : public ResourceBase
    {
    public:
        SkeletalMeshSceneResource();
        virtual ~SkeletalMeshSceneResource();

    public:
        void Create(const aiScene* _aiScene, shared_ptr<SkeletonResource> _skeleton);

    public:
        inline vector<SkeletalMesh>& GetSkeletalMeshVec();
        inline DirectX::BoundingBox GetBoundingBox();

    private:
        vector<SkeletalMesh> m_SkeletalMeshVec;

        Vector3 m_AABBmin;
        Vector3 m_AABBmax;
        DirectX::BoundingBox m_BoundingBox;
    };

    // ���� : SkeletalMeshSceneResource�� Get & Set �Լ�
    vector<SkeletalMesh>& SkeletalMeshSceneResource::GetSkeletalMeshVec()
    {
        return m_SkeletalMeshVec;
    }
    DirectX::BoundingBox SkeletalMeshSceneResource::GetBoundingBox()
    {
        return m_BoundingBox;
    }
    // -------------------------------------------------------------------------
}