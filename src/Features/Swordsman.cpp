#include "Features/Swordsman.hpp"

namespace features::units
{
	bool Swordsman::IsValid() const
	{
		return Unit::IsValid() && strength > 0;
	}

	void Swordsman::Act(core::Environment& env)
	{
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
