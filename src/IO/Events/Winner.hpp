#pragma once

#include <cstdint>
#include <string>

namespace sw::io
{
	struct Winner
	{
		constexpr static const char* Name = "WINNER";

		uint32_t unitId{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
		}
	};
}
