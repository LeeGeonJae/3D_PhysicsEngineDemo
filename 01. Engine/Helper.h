#pragma once

namespace Engine
{
	// ���� : Ŭ���� ����
	class AnimationResource;
	class SkeletonResource;
	class Node;

	// ���� : aiMatrix�� Matrix�� ��ȯ���ִ� ���� �Լ�
	void aiMatrixToMatrix(const aiMatrix4x4& _aiMatrix, Matrix& _matrix);
}
