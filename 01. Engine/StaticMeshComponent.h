#pragma once
#include "RenderComponent.h"

namespace Engine
{
    // ���� : Ŭ���� ����
    class StaticMeshSceneResource;
    class StaticMesh;
    class Material;
    class Shader;
    class StaticMeshInstance;
    class Node;
    class AnimationResource;

    // ���� : ����ƽ �޽� ������Ʈ ���� ����ü
    struct StaticMeshComponentInfo
    {
        RenderComponentInfo m_RenderComponentInfo;
        string m_FilePath;
    };

    // -------------------------------------------------------------------------
    // ���� : ����ƽ �޽� ������Ʈ Ŭ����
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
        void NodeSetting(shared_ptr<AnimationResource> _animationResource, vector<shared_ptr<Node>>& _nodeVec, shared_ptr<Node>& _rootNode);

    private:
        string m_FilePath;
        float m_CurrentTime;

        shared_ptr<Node> m_pRootNode;
        vector<shared_ptr<Node>> m_pNodeVec;
        shared_ptr<StaticMeshSceneResource> m_pStaticMesheScene;
        shared_ptr<Shader> m_pShader;

        vector<shared_ptr<StaticMeshInstance>> m_pStaticMeshInstanceVec;
    };
    // -------------------------------------------------------------------------
}