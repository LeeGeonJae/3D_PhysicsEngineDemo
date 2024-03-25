#include "pch.h"
#include "Helper.h"

#include "AnimationResource.h"
#include "SkeletonResource.h"
#include "Node.h"
#include "StaticMeshResource.h"

namespace GraphicsEngine
{
	void aiMatrixToMatrix(const aiMatrix4x4& _aiMatrix, Matrix& _matrix)
	{
		_matrix = DirectX::SimpleMath::Matrix(
			_aiMatrix.a1, _aiMatrix.b1, _aiMatrix.c1, _aiMatrix.d1,
			_aiMatrix.a2, _aiMatrix.b2, _aiMatrix.c2, _aiMatrix.d2,
			_aiMatrix.a3, _aiMatrix.b3, _aiMatrix.c3, _aiMatrix.d3,
			_aiMatrix.a4, _aiMatrix.b4, _aiMatrix.c4, _aiMatrix.d4
		);
	}
}
