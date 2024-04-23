#pragma once

namespace PhysicsEngine
{
	enum class ActorType
	{
		PLAYER,
		MONSTER,
		WALL,
		TILE,

		END
	};

	class ActorUserData
	{
	public:
		ActorUserData(ActorType _type);
		~ActorUserData();

	public:
		inline const ActorType& GetActorType();

	private:
		ActorType m_Type;
	};

	const ActorType& ActorUserData::GetActorType()
	{
		return m_Type;
	}
}
