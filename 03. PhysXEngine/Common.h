#pragma once

#include <directxtk\SimpleMath.h>
#include <vector>
#include <string>

namespace physics
{
#pragma region EnumClass
	/// <summary>
	/// �ݶ��̴� Ÿ��
	/// Ʈ���� : �浹���� �ʰ� ��ġ�� ������ �̺�Ʈ�� ȣ��
	/// �ݸ��� : �浹�ǰ� �ݸ��� �̺�Ʈ�� ȣ��
	/// </summary>
	enum class EColliderType
	{
		TRIGGER,
		COLLISION
	};

	/// <summary>
	/// �ݹ� �Լ��� ���޵Ǵ� �ݸ��� �̺�Ʈ ����
	/// </summary>
	enum class ECollisionEventType
	{
		ENTER_OVERLAP = 0,
		ON_OVERLAP,
		END_OVERLAP,
		ENTER_COLLISION,
		ON_COLLISION,
		END_COLLISION,
	};

	/// <summary>
	/// ���� ��
	/// </summary>
	enum class EArticulationAxis
	{
		SWING_1,
		SWING_2,
		TWIST
	};

	/// <summary>
	/// ���� ���
	/// </summary>
	enum class EArticulationMotion
	{
		LOCKED = 0,
		LIMITED = 1,
		FREE = 2
	};
#pragma endregion

	/// <summary>
	/// ���� ���� ���� ������ : �߷�, �浹 ��Ʈ����
	/// </summary>
	struct PhysicsEngineInfo
	{
		DirectX::SimpleMath::Vector3 gravity;
		int collisionMatrix[16];
	};

	/// <summary>
	/// �ݸ��� : �浹 ������
	/// </summary>
	struct CollisionData
	{
		unsigned int myId;
		unsigned int otherId;
		unsigned int myLayerNumber;
		unsigned int otherLayerNumber;
		std::vector<DirectX::SimpleMath::Vector3> ContectPoints;
		bool isDead = false;
	};

	/// <summary>
	/// ����ĳ��Ʈ : �ε��� ������Ʈ���� ����
	/// </summary>
	struct RayCastData
	{
		unsigned int hitSize = 0;
		std::vector<unsigned int> id;
		std::vector<unsigned int> layerNumber;
		std::vector<DirectX::SimpleMath::Vector3> contectPoints;
	};

#pragma region GetSetData
	/// <summary>
	/// ���� �������� ������ �ٵ� �������� �ְ� �޴� GetSet ����ü
	/// </summary>
	struct RigidBodyGetSetData
	{
		DirectX::SimpleMath::Matrix transform = {};				// Ʈ������ �����Դϴ�.
		DirectX::SimpleMath::Vector3 linearVelocity = {};		// ���� 0�̸� StaticBody �Դϴ�.
		DirectX::SimpleMath::Vector3 angularVelocity = {};		// ���� 0�̸� StaticBody �Դϴ�.
	};

	struct CharacterControllerGetSetData
	{
		DirectX::SimpleMath::Vector3 position;					// ĳ���� ��Ʈ�ѷ��� ��ġ
	};

	struct CharacterMovementGetSetData
	{
		DirectX::SimpleMath::Vector3 velocity;					// ĳ���� ��Ʈ�ѷ��� x,y,z�� �ӵ�
		bool isFall;											// ĳ���Ͱ� �������� �ִ��� üũ ����
	};
#pragma endregion

#pragma region Resource
	struct ConvexMeshResoureceInfo
	{
		DirectX::SimpleMath::Vector3* vertices;					// �� ���ؽ�
		int vertexSize;											// �� ���ؽ� ������
		unsigned char convexPolygonLimit = 4;					// ������ �޽ÿ� ������ ������ �ִ� �� ( �ּ� : 4�� �̻�, �ִ� 256�� �̸� )
	};
	struct MaterialResourceInfo
	{
		float staticFriction = 1.f;									// ���� ���� ��� ( 0.f ~ 1.f )
		float dynamicFriction = 1.f;								// ���� ���� ��� ( 0.f ~ 1.f )
		float restitution = 1.f;									// ���� ��� ( 0.f ~ 1.f ) 
		float density = 1.f;										// �е� ( 0.f ~ 1.f )
	};
#pragma endregion

#pragma region Collider
	constexpr unsigned int unregisterID = 0;

	/// <summary>
	/// �ݶ��̴� : ���� �浹 ó���� ���� ������Ʈ ����
	/// </summary>
	struct ColliderInfo
	{
		unsigned int id = unregisterID;
		unsigned int layerNumber = 0;
		DirectX::SimpleMath::Matrix collisionTransform;
		float staticFriction = 1.f;									// ���� ���� ���
		float dynamicFriction = 1.f;								// ���� ���� ���
		float restitution = 1.f;									// ���� ���
		float density = 1.f;										// �е�
	};

	struct BoxColliderInfo
	{
		ColliderInfo colliderInfo;
		DirectX::SimpleMath::Vector3 boxExtent;
	};

	struct SphereColliderInfo
	{
		ColliderInfo colliderInfo;
		float raidus;
	};

	struct CapsuleColliderInfo
	{
		ColliderInfo colliderInfo;
		float raidus;
		float halfHeight;
	};

	struct ConvexMeshColliderInfo
	{
		ColliderInfo colliderInfo;
		DirectX::SimpleMath::Vector3* vertices;					// �� ���ؽ�
		int vertexSize;											// �� ���ؽ� ������
		unsigned char convexPolygonLimit = 4;					// ������ �޽ÿ� ������ ������ �ִ� �� ( �ּ� : 4�� �̻�, �ִ� 256�� �̸� )
	};
#pragma endregion

#pragma region CharacterController
	/// <summary>
	/// ĳ���� �����Ʈ : ĳ���� �̵��� ���õ� ������
	/// </summary>
	struct CharacterMovementInfo
	{
		float maxSpeed = 5.f;									// �̵� �ִ� �ӵ� : ĳ���Ͱ� ������ �� �ִ� �ִ� �ӵ�
		float acceleration = 100.f;								// ���ӵ� : ĳ���Ͱ� �Է� ���� ���� �� �̵� ���ӵ�
		float staticFriction = 0.4f;							// ���� ���� ��� : ĳ���Ͱ� �̵� �� ������ �� ĳ���Ͱ� �޴� ������ ( 0.0f ~ 1.f )
		float dynamicFriction = 0.1f;							// ���� ���� ��� : �̵� �߿� ĳ���Ͱ� �޴� ������ ( 0.0f ~ 1.f )
		float jumpSpeed = 2.f;									// ����(y��) �ӵ�
		float jumpXZAcceleration = 10.f;						// ���� �߿� �̵�(XZ��) ���ӵ� ��
		float jumpXZDeceleration = 0.2f;						// ���� �߿� �̵�(XZ��) ���� �� ( 0.0 ~ 1.0 )
		float gravityWeight = 9.8f;								// �⺻ �߷� ���� �� �� ������ ����ġ�� �� �ְ� ���� �� ���� �ٸ��� ������ �� �ֽ��ϴ�.
	};

	/// <summary>
	/// ĳ���� ��Ʈ�ѷ� : ĳ���� ��Ʈ�ѷ� ������ ���� ��Ʈ�ѷ� ������
	/// </summary>
	struct CharacterControllerInfo
	{
		unsigned int id = unregisterID;							// ĳ���� ��Ʈ�ѷ� ���̵�
		unsigned int layerNumber = 0;							// �浹 ��Ʈ���� ���̾� �ѹ�

		DirectX::SimpleMath::Vector3 position{ 0.f, 0.f, 0.f };	// ĳ���� ��Ʈ�ѷ��� ��ġ�ϴ� ó�� ���� ��ġ
		float height = 30.f;									// ĳ���� ��Ʈ�ѷ�(ĸ��)�� ����
		float radius = 20.f;									// ĳ���� ��Ʈ�ѷ�(ĸ��)�� ������
		float stepOffset = 0.0f;								// ĳ���� ��Ʈ�ѷ��� ������ �� �ִ� 
		float slopeLimit = 0.7f;								// ĳ���Ͱ� �ɾ� �ö� �� �ִ� �ִ� ����
		float contactOffset = 0.1f;								// ��Ʈ�ѷ��� ���� ������ : ��ġ ���е� ������ �����ϱ� ���� ����մϴ�.
	};
#pragma endregion

#pragma region CharacterPhysics
	struct JointAxisInfo
	{
		EArticulationMotion motion = EArticulationMotion::LIMITED;		// ��� ����
		float limitsLow = -60.f;										// ���� ( Limit�� �� ȸ�� �Ʒ� ���� ���� : -180.0 ~ 0.0 ) 
		float limitsHigh = 60.f;										// ���� ( Limit�� �� ȸ�� �� ���� ���� : 0.0 ~ 180.0 )
	};

	struct CharacterJointInfo
	{
		JointAxisInfo Swing1AxisInfo;					// Swing1( X���� �߽����� �� ȸ�� )
		JointAxisInfo Swing2AxisInfo;					// Swing2( Y���� �߽����� �� ȸ�� )
		JointAxisInfo TwistAxisInfo;					// Twist( Z���� �߽����� �� ȸ�� )
		DirectX::SimpleMath::Matrix localTransform;		// ����Ʈ�� ���� ��ǥ
		float stiffness = 0.f;							// ���� : ������ ��ǥ ��ġ�� �̵��Ϸ��� ���� ũ��
		float damping = 1.f;							// ���� ��� : ��� ���� ���׷� ( ������ �����ϰ� �ε巴�� �����̵� �� �� �ְ� )
		float maxForce = 1.f;							// �ִ� �� : ���� ����̺갡 ������ �� �ִ� �ִ� ��
	};

	struct CharacterLinkInfo
	{
		std::string boneName;							// �ش� ��(��ũ)�� �̸�
		std::string parentBoneName;						// �θ� ��(��ũ)�� �̸�
		float density = 1.f;							// �е�
		CharacterJointInfo JointInfo;					// ����Ʈ ����
		DirectX::SimpleMath::Matrix localTransform;		// ���� ��ǥ
	};

	struct CharacterPhysicsInfo
	{
		std::string modelPath;							// �� ����(�� �����͸� ������ �ִ�) ���
		unsigned int id = unregisterID;					// ���̵�
		unsigned int layerNumber = 0;					// ���̾� �ѹ�
		DirectX::SimpleMath::Matrix worldTransform;		// ���� ��ǥ
		float staticFriction = 1.f;						// ���� ���� ���
		float dynamicFriction = 1.f;					// ���� ���� ���
		float restitution = 1.f;						// ���� ���
		float density = 1.f;							// �е�
	};
#pragma endregion

#pragma region PhysicsCloth
	struct PhysicsVertex
	{
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector2 uv;
		DirectX::SimpleMath::Vector3 normal;
		DirectX::SimpleMath::Vector3 tangent;
		DirectX::SimpleMath::Vector3 biTangent;
	};

	struct PhysicsClothMaterialInfo
	{
		float friction = 0.8f;
		float damping = 0.001f;
		float adhesion = 1e+7f;
		float viscosity = 0.001f;
		float vorticityConfinement = 0.5f;
		float surfaceTension = 0.005f;
		float cohesion = 0.05f;
		float lift = 0.f;
		float drag = 0.f;
		float cflCoefficient = 1.f;
		float gravityScale = 2.f;
	};

	struct PhysicsClothInfo
	{
		unsigned int id = 0;
		unsigned int layerNumber = 0;
		PhysicsClothMaterialInfo materialInfo;
		DirectX::SimpleMath::Matrix worldTransform = {};
		void* DirectXBuffer = nullptr;
		DirectX::SimpleMath::Vector3* vertices = nullptr;
		DirectX::SimpleMath::Vector2* uv = nullptr;
		void* vertexBuffer = nullptr;
		void* indexBuffer = nullptr;
		unsigned int vertexSize = 0;
		unsigned int* indices = nullptr;
		unsigned int indexSize = 0;
		unsigned int* activeVertexIndices = nullptr;
		unsigned int activeVertexSize = 0;
		float totalClothMass = 1.f;
	};

	struct PhysicsClothGetData
	{
		DirectX::SimpleMath::Matrix worldTransform;
	};

	struct PhysicsClothSetData
	{
		DirectX::SimpleMath::Matrix worldTransform;
	};
#pragma endregion
}