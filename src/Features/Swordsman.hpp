#pragma once
#include "Features/Unit.hpp"

namespace core
{
	class Environment;
}

namespace features::units
{
	class Swordsman : public Unit
	{
		uint32_t agility;
		uint32_t strength;
		uint32_t range;

	public:
		static constexpr const char* UNIT_TYPE = "Swordsman";

		Swordsman(uint32_t id, uint32_t x, uint32_t y, uint32_t hp, uint32_t strength) :
				Unit(id, x, y, hp),
				strength(strength)
		{}

		bool IsValid() const override;
		void Act(core::Environment& env) override;
	};
}
