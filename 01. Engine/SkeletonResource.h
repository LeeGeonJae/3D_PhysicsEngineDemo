#pragma once
#include "ResourceBase.h"

namespace Engine
{
    // 클래스 선언
    struct BoneWeightVertex;


    // 본 구조체
    struct Bone
    {
        string m_NodeName;
        Matrix m_OffsetMatrix;
    };


    // 본 구조체 집합을 담은 스켈레톤 클래스
    class SkeletonResource : public ResourceBase
    {
    public:
        SkeletonResource();
        virtual ~SkeletonResource();

    public:
        void Create(aiMesh* _aiMesh, vector<BoneWeightVertex>* _vertices);

    public:
        inline shared_ptr<Bone> FindBone(string _key);

    private:
        unordered_map<string, shared_ptr<Bone>> m_BoneMap;
    };


    shared_ptr<Bone> SkeletonResource::FindBone(string _key)
    {
        if (m_BoneMap.find(_key) != m_BoneMap.end())
            return m_BoneMap.find(_key)->second;

        return nullptr;
    }
}