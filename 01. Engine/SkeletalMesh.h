#pragma once
#include "ResourceBase.h"

namespace Engine
{
    class SkeletalMesh : public ResourceBase
    {
    public:
        SkeletalMesh();
        virtual ~SkeletalMesh();

    public:
        void Create(string _path) override;

    private:

    };

}