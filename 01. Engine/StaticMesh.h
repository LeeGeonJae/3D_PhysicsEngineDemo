#pragma once
#include "ResourceBase.h"

namespace Engine
{
    class StaticMesh : public ResourceBase
    {
    public:
        StaticMesh();
        virtual ~StaticMesh();

    public:
        void Create(string _path) override;

    private:

    };

}