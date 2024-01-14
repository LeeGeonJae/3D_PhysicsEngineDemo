#pragma once
#include "ResourceBase.h"

namespace Engine
{
    struct bone
    {
        string m_NodeName;
        int BoneIndex = -1;
        Matrix m_OffsetMatrix;
    };



    class Skeleton : public ResourceBase
    {
    public:
        Skeleton();
        virtual ~Skeleton();

    public:
        void Create(string _path) override;

    private:

    };
}