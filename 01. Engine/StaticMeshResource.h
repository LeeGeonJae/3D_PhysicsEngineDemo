#pragma once
#include "ResourceBase.h"

namespace Engine
{
    // 건재 : 클래스 선언
    class Material;


    // 건재 : 기본 버텍스 구조체
    struct Vertex
    {
        Vector3 m_Position;
        Vector2 m_TexCoord;
        Vector3 m_Normal;
        Vector3 m_Tangent;
        Vector3 m_BiTangents;
    };

    // -------------------------------------------------------------------------
    // 건재 : 버텍스를 갖는 스태틱 메시 클래스
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

    // 건재 : StaticMesh의 Get & Set 함수
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
    // 건재 : FBX 씬의 모든 스켈레탈 메시 리소스 집합을 가지고 있는 리소스 클래스
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

    // 건재 : StaticMeshSceneResource의 Get & Set 함수
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