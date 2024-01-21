#pragma once

namespace Engine
{
	// 건재 : 클래스 선언
	class AnimationResource;
	class SkeletonResource;
	class Node;

	// 건재 : aiMatrix를 Matrix로 변환해주는 전역 함수
	void aiMatrixToMatrix(const aiMatrix4x4& _aiMatrix, Matrix& _matrix);
}
