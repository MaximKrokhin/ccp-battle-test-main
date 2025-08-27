#pragma once

#include <cstdint>
#include <string>

namespace sw::io
{
	struct UnitNotSpawned
	{
		constexpr static const char* Name = "UNIT_NOT_SPAWNED";

		uint32_t unitId{};
		std::string unitType{};
		uint32_t x{};
		uint32_t y{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("unitType", unitType);
			visitor.visit("x", x);
			visitor.visit("y", y);
		}
	};
}
