#pragma once
#include "RenderComponent.h"

namespace GraphicsEngine
{
    class StaticMeshSceneResource;
    class StaticMesh;
    class Material;
    class Shader;
    class StaticMeshInstance;
    class Node;
    class AnimationResource;
}

namespace Engine
{
    // 건재 : 클래스 선언


    // 건재 : 스태틱 메시 컴포넌트 정보 구조체
    struct StaticMeshComponentInfo
    {
        RenderComponentInfo m_RenderComponentInfo;
        string m_FilePath;
    };

    // 건재 : 스태틱 메시 컴포넌트 클래스
    class StaticMeshComponent : public RenderComponent
    {
    public:
        StaticMeshComponent();
        virtual ~StaticMeshComponent();

    public:
        void Setting(StaticMeshComponentInfo _info);

    public:
        virtual void Init() override;
        virtual void Update(float _deltaTime) override;
        virtual void Render() override;

    private:
        void NodeSetting(shared_ptr<GraphicsEngine::AnimationResource> _animationResource, vector<shared_ptr<GraphicsEngine::Node>>& _nodeVec, shared_ptr<GraphicsEngine::Node>& _rootNode);

    private:
        string m_FilePath;
        float m_CurrentTime;

        shared_ptr<GraphicsEngine::Node> m_pRootNode;
        vector<shared_ptr<GraphicsEngine::Node>> m_pNodeVec;
        shared_ptr<GraphicsEngine::StaticMeshSceneResource> m_pStaticMesheScene;
        shared_ptr<GraphicsEngine::Shader> m_pShader;

        vector<shared_ptr<GraphicsEngine::StaticMeshInstance>> m_pStaticMeshInstanceVec;
    };
}