#pragma once
#include "ResourceBase.h"

namespace Engine
{
    // ���� : Ŭ���� ����
    struct BoneWeightVertex;


    // ���� : �� ����ü
    struct Bone
    {
        string m_NodeName;
        Matrix m_OffsetMatrix;
    };


    // ���� : �� ����ü ������ ���� ���̷��� Ŭ����
    class SkeletonResource : public ResourceBase
    {
    public:
        SkeletonResource();
        virtual ~SkeletonResource();

    public:
        void Create(aiMesh* _aiMesh, vector<BoneWeightVertex>* _vertices);

    public:
        inline shared_ptr<Bone> FindBone(string _key);
        inline void SetBone(shared_ptr<Bone> _bone);

    private:
        vector<shared_ptr<Bone>> m_BoneVec;
        unordered_map<string, shared_ptr<Bone>> m_BoneMap;
    };

    shared_ptr<Bone> SkeletonResource::FindBone(string _key)
    {
        if (m_BoneMap.find(_key) != m_BoneMap.end())
            return m_BoneMap.find(_key)->second;

        return nullptr;
    }
    void SkeletonResource::SetBone(shared_ptr<Bone> _bone)
    {
         m_BoneVec.push_back(_bone);
         m_BoneMap.insert(make_pair(_bone->m_NodeName, _bone));
    }
}