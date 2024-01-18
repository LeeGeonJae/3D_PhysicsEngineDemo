#pragma once
#include "ResourceBase.h"

namespace Engine
{
    // 클래스 선언
    class Material;


    // 기본 버텍스 구조체
    struct Vertex
    {
        Vector3 m_Position;
        Vector2 m_TexCoord;
        Vector3 m_Normal;
        Vector3 m_Tangent;
        Vector3 m_BiTangents;
    };


    // 버텍스를 갖는 스태틱 메시 클래스
    class StaticMesh
    {
    public:
        StaticMesh();
        virtual ~StaticMesh();

    public:
        void Create(aiMesh* _aiMesh);

    private:
        void createBuffer();

    private:
        string m_Name;

        vector<Vertex> m_VertexVec;
        vector<UINT> m_IndexVec;

        ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
        ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
    };


    // 스테틱 메시의 집합을 가진 리소스 클래스
    class StaticMeshSceneResource : public ResourceBase
    {
    public:
        StaticMeshSceneResource();
        virtual ~StaticMeshSceneResource();

    public:
        void Create(const aiScene* _aiScene);

    private:
        vector<StaticMesh> m_StaticMeshVec;
        vector<shared_ptr<Material>> m_pMaterialVec;

        Vector3 m_AABBmin;
        Vector3 m_AABBmax;
        BoundingBox m_BoundingBox;
    };
}