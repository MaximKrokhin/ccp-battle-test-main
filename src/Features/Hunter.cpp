#include "Features/Hunter.hpp"

namespace features::units
{
	bool Hunter::IsValid() const
	{
		return Unit::IsValid() && strength > 0 && agility > 0 && range > 0;
	}

	void Hunter::Act(core::Environment& env)
	{
		auto distant = env.GetObjectsInRange<core::search_policy::AliveOnly>(this->GetX(), this->GetY(), 2, range);
		if (!distant.empty())
		{
			auto& target = *distant[rand() % distant.size()];
			target.TakeDamage(agility, GetId(), env);
			return;
		}

		auto adjacent = env.GetAdjacentObjects<core::search_policy::AliveOnly>(this->GetX(), this->GetY());
		if (!adjacent.empty())
		{
			auto& target = *adjacent[rand() % adjacent.size()];
			target.TakeDamage(strength, GetId(), env);
			return;
		}

		March(env);
	}
}
