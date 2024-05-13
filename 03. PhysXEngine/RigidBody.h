#pragma once

namespace physics
{
	class RigidBody
	{
	public:
		/// <summary>
		/// �ݶ��̴� Ÿ�԰� ID, CollisionMatrix�� layerNumber�� �Է� �޽��ϴ�.
		/// </summary>
		/// <param name="colliderType"> �ݶ��̴� Ÿ�� </param>
		/// <param name="id"> ���ӿ��� ���� ���̵� </param>
		/// <param name="layerNumber"> �ݸ��� ��Ʈ������ ���̾� �ѹ� </param>
		RigidBody(physics::EColliderType colliderType, unsigned int id, unsigned int layerNumber);
		virtual ~RigidBody();

		/// <summary>
		/// ���� �������� �Ҵ���� ���̵� ��ȯ�մϴ�.
		/// </summary>
		inline const unsigned int GetID() const;

		/// <summary>
		/// ���� �������� �Ҵ� ���� ���̾� �ѹ��� ��ȯ�մϴ�.
		/// </summary>
		inline const unsigned int GetLayerNumber() const;

		/// <summary>
		/// ������ �ٵ��� �ݶ��̴� Ÿ���� ��ȯ�մϴ�.
		/// </summary>
		inline const EColliderType& GetColliderType() const;

	protected:
		unsigned int mID;
		unsigned int mLayerNumber;
		physics::EColliderType	mColliderType;
	};

	const unsigned int RigidBody::GetID() const
	{
		return mID;
	}
	const unsigned int RigidBody::GetLayerNumber() const
	{
		return mLayerNumber;
	}
	const EColliderType& RigidBody::GetColliderType() const
	{
		return mColliderType;
	}
}