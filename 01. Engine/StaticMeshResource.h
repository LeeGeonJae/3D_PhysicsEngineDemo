#pragma once
#include "ResourceBase.h"

namespace Engine
{
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
    class StaticMeshResource 
    {
    public:
        StaticMeshResource();
        virtual ~StaticMeshResource();

    public:
        void Create(aiMesh* _mesh);

    private:
        vector<Vertex> m_Vertices;
        vector<UINT> m_Indices;

        ID3D11Buffer* m_pVertexBuffer = nullptr;
        ID3D11Buffer* m_pIndexBuffer = nullptr;
    };


    // 스테틱 메시의 집합을 가진 리소스 클래스
    class StaticMeshSceneResource : public ResourceBase
    {
    public:
        StaticMeshSceneResource();
        virtual ~StaticMeshSceneResource();

    public:
        void Create(string _path);

    private:
        vector<StaticMeshResource> m_StaticMeshResources;
        vector<Material> m_Materials;

        Vector3 m_AABBmin;
        Vector3 m_AABBmax;
    };
}