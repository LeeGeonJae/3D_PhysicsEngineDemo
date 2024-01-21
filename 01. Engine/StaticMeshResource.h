#pragma once
#include "ResourceBase.h"

namespace Engine
{
    // ���� : Ŭ���� ����
    class Material;


    // ���� : �⺻ ���ؽ� ����ü
    struct Vertex
    {
        Vector3 m_Position;
        Vector2 m_TexCoord;
        Vector3 m_Normal;
        Vector3 m_Tangent;
        Vector3 m_BiTangents;
    };

    // -------------------------------------------------------------------------
    // ���� : ���ؽ��� ���� ����ƽ �޽� Ŭ����
    class StaticMesh
    {
    public:
        StaticMesh();
        virtual ~StaticMesh();

    public:
        void Create(aiMesh* _aiMesh);

    private:
        void createBuffer();

    public:
        inline const string& GetName();
        inline vector<Vertex> GetVertices();
        inline vector<UINT> GetIndices();
        inline ComPtr<ID3D11Buffer> GetVertexBuffer();
        inline ComPtr<ID3D11Buffer> GetIndexBuffer();
        inline void SetMaterial(shared_ptr<Material> _material);
        inline shared_ptr<Material> GetMaterial();

    private:
        string m_Name;
        shared_ptr<Material> m_pMaterial;

        vector<Vertex> m_VertexVec;
        vector<UINT> m_IndexVec;

        ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
        ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
    };

    // ���� : StaticMesh�� Get & Set �Լ�
    const string& StaticMesh::GetName()
    {
        return m_Name;
    }
    vector<Vertex> StaticMesh::GetVertices() 
    {
        return m_VertexVec;
    }
    vector<UINT> StaticMesh::GetIndices()
    {
        return m_IndexVec;
    }
    ComPtr<ID3D11Buffer> StaticMesh::GetVertexBuffer()
    {
        return m_pVertexBuffer;
    }
    ComPtr<ID3D11Buffer> StaticMesh::GetIndexBuffer()
    {
        return m_pIndexBuffer;
    }
    void StaticMesh::SetMaterial(shared_ptr<Material> _material)
    {
        m_pMaterial = _material;
    }
    shared_ptr<Material> StaticMesh::GetMaterial()
    {
        return m_pMaterial;
    }
    // -------------------------------------------------------------------------


    // -------------------------------------------------------------------------
    // ���� : FBX ���� ��� ���̷�Ż �޽� ���ҽ� ������ ������ �ִ� ���ҽ� Ŭ����
    class StaticMeshSceneResource : public ResourceBase
    {
    public:
        StaticMeshSceneResource();
        virtual ~StaticMeshSceneResource();

    public:
        void Create(const aiScene* _aiScene);

    public:
        inline vector<StaticMesh>& GetStaticMeshVec();
        inline DirectX::BoundingBox GetBoundingBox();

    private:
        vector<StaticMesh> m_StaticMeshVec;

        Vector3 m_AABBmin;
        Vector3 m_AABBmax;
        BoundingBox m_BoundingBox;
    };

    // ���� : StaticMeshSceneResource�� Get & Set �Լ�
    vector<StaticMesh>& StaticMeshSceneResource::GetStaticMeshVec()
    {
        return m_StaticMeshVec;
    }
    DirectX::BoundingBox StaticMeshSceneResource::GetBoundingBox()
    {
        return m_BoundingBox;
    }
    // -------------------------------------------------------------------------
}