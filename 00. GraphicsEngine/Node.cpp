#include "pch.h"
#include "Node.h"

#include "Helper.h"

#include "AnimationResource.h"
#include "SkeletonResource.h"

namespace GraphicsEngine
{
	NodeDataResource::NodeDataResource()
		:ResourceBase(ResourceType::NodeData)
	{
	}
	NodeDataResource::~NodeDataResource()
	{
	}



	Node::Node()
	{
	}

	Node::~Node()
	{
	}

	void Node::Update(Matrix _modelTransform, float _currentTime)
	{
		Vector3 position, scale;
		Quaternion rotation;

		// ���� : �ִϸ��̼�
		if (m_pAnimationNode != nullptr)
		{
			float animationTime = _currentTime;
			while (animationTime >= m_pAnimationNode->m_Owner->GetFrameCount())
			{
				animationTime -= m_pAnimationNode->m_Owner->GetFrameCount();
			}
			interpolateAnimationData(animationTime, position, scale, rotation);

			m_Local = Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(position);
		}

		// ���� : �θ� ������ �ش� �θ��� Ʈ������ ���ϱ�
		if (m_pParent != nullptr)
		{
			m_World = m_Local * m_pParent->m_World;
		}
		else
		{
			m_World = m_Local * _modelTransform;
		}

		// ���� : �ڽ� ��� ������Ʈ
		for (auto child : m_pChildrenVec)
		{
			if (child != nullptr)
			{
				child->Update(_modelTransform, _currentTime);
			}
		}
	}

	void Node::interpolateAnimationData(float currentTime, Vector3& outPosition, Vector3& outScaling, Quaternion& outRotation)
	{
		aiVector3D currentPosition;
		aiVector3D currentScale;
		aiQuaternion currentRotation;
		aiVector3D nextPosition;
		aiVector3D nextScale;
		aiQuaternion nextRotation;

		// ���� : �ִϸ��̼� �������� Ű������ ������ ������
		unsigned int numFrames = m_pAnimationNode->m_Owner->GetFrameCount();
		
		// ���� : ù ��° Ű������ ������
		unsigned int frameIndex = 0;

		// ���� : �� ��° Ű������ ������
		unsigned int nextFrameIndex = 1;

		// ���� : ���� �ð��� ���� ����� Ű���������� ����
		for (unsigned int i = 0; i < numFrames - 1; ++i) {
			if (currentTime < m_pAnimationNode->m_KeyFrame[nextFrameIndex].m_Time) {
				break;
			}

			frameIndex = i;
			nextFrameIndex = i + 1;
		}

		// ���� : ���� Ű�����Ӱ� ���� Ű�������� �ð� ���� ���
		float deltaTime = m_pAnimationNode->m_KeyFrame[nextFrameIndex].m_Time - m_pAnimationNode->m_KeyFrame[frameIndex].m_Time;

		// ���� : ���� ���� Ű�����Ӱ� ���� Ű�������� �ð� ���̰� ���ٸ� 1�� ���� ( ��� ������ �ִϸ��̼� ���� ������ ���ư��� ��� �����ϱ� ���� )
		if (deltaTime == 0)
			deltaTime = 1;

		// ���� : ���� �ð��� ���� Ű�����Ӱ� ���� Ű������ ������ ���� ���
		float factor = 0;
		if (m_pAnimationNode->m_KeyFrame[frameIndex].m_Time < currentTime)
			factor = (currentTime - m_pAnimationNode->m_KeyFrame[frameIndex].m_Time) / deltaTime;

		currentPosition.x = m_pAnimationNode->m_KeyFrame[frameIndex].m_Transtation.x;
		currentPosition.y = m_pAnimationNode->m_KeyFrame[frameIndex].m_Transtation.y;
		currentPosition.z = m_pAnimationNode->m_KeyFrame[frameIndex].m_Transtation.z;
		currentScale.x = m_pAnimationNode->m_KeyFrame[frameIndex].m_Scale.x;
		currentScale.y = m_pAnimationNode->m_KeyFrame[frameIndex].m_Scale.y;
		currentScale.z = m_pAnimationNode->m_KeyFrame[frameIndex].m_Scale.z;
		currentRotation.x = m_pAnimationNode->m_KeyFrame[frameIndex].m_Rotation.x;
		currentRotation.y = m_pAnimationNode->m_KeyFrame[frameIndex].m_Rotation.y;
		currentRotation.z = m_pAnimationNode->m_KeyFrame[frameIndex].m_Rotation.z;
		currentRotation.w = m_pAnimationNode->m_KeyFrame[frameIndex].m_Rotation.w;

		nextPosition.x = m_pAnimationNode->m_KeyFrame[nextFrameIndex].m_Transtation.x;
		nextPosition.y = m_pAnimationNode->m_KeyFrame[nextFrameIndex].m_Transtation.y;
		nextPosition.z = m_pAnimationNode->m_KeyFrame[nextFrameIndex].m_Transtation.z;
		nextScale.x = m_pAnimationNode->m_KeyFrame[nextFrameIndex].m_Scale.x;
		nextScale.y = m_pAnimationNode->m_KeyFrame[nextFrameIndex].m_Scale.y;
		nextScale.z = m_pAnimationNode->m_KeyFrame[nextFrameIndex].m_Scale.z;
		nextRotation.x = m_pAnimationNode->m_KeyFrame[nextFrameIndex].m_Rotation.x;
		nextRotation.y = m_pAnimationNode->m_KeyFrame[nextFrameIndex].m_Rotation.y;
		nextRotation.z = m_pAnimationNode->m_KeyFrame[nextFrameIndex].m_Rotation.z;
		nextRotation.w = m_pAnimationNode->m_KeyFrame[nextFrameIndex].m_Rotation.w;

		// ���� : Ű������ �������� ��ȯ�� ���� �����Ͽ� ���
		aiVector3D fianlPosition = currentPosition + factor * (nextPosition - currentPosition);
		aiVector3D fianlScale = currentScale + factor * (nextScale - currentScale);
		aiQuaternion fianlRotation;
		aiQuaternion::Interpolate(fianlRotation, currentRotation, nextRotation, factor);

		outPosition.x = fianlPosition.x;
		outPosition.y = fianlPosition.y;
		outPosition.z = fianlPosition.z;
		outScaling.x = fianlScale.x;
		outScaling.y = fianlScale.y;
		outScaling.z = fianlScale.z;
		outRotation.x = fianlRotation.x;
		outRotation.y = fianlRotation.y;
		outRotation.z = fianlRotation.z;
		outRotation.w = fianlRotation.w;
	}

	void NodeDataResource::processNode(const aiNode* _aiNode, const aiScene* _aiScene)
	{
		NodeData nodeData;
		nodeData.m_Name = _aiNode->mName.C_Str();
		aiMatrixToMatrix(_aiNode->mTransformation, nodeData.m_LocalMatrix);
		if (_aiNode->mParent != nullptr)
			nodeData.m_ParentName = _aiNode->mParent->mName.C_Str();

		for (int i = 0; i < _aiNode->mNumMeshes; i++)
		{
			auto aimesh = _aiScene->mMeshes[_aiNode->mMeshes[i]];
			string meshName = aimesh->mName.C_Str();
			nodeData.m_MeshName.push_back(meshName);
		}
		AddNodeData(nodeData);

		for (int i = 0; i < _aiNode->mNumChildren; i++)
		{
			processNode(_aiNode->mChildren[i], _aiScene);
		}
	}
}

