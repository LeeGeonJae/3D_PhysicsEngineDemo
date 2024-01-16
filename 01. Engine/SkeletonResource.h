#pragma once
#include "ResourceBase.h"

namespace Engine
{
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

    private:
        vector<Bone> m_Bones;
    };
}