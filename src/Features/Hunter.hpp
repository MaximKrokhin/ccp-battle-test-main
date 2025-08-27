#pragma once
#include "Features/Unit.hpp"

namespace features::units
{
	class Hunter : public Unit
	{
		uint32_t agility;
		uint32_t strength;
		uint32_t range;

	public:
		static constexpr const char* UNIT_TYPE = "Hunter";

		Hunter(uint32_t id, uint32_t x, uint32_t y, uint32_t hp, uint32_t agility, uint32_t strength, uint32_t range) :
				Unit(id, x, y, hp),
				agility(agility),
				strength(strength),
				range(range)
		{}

		bool IsValid() const override;
		void Act(core::Environment& env) override;
	};
}
