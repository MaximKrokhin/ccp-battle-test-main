#include "Features/Unit.hpp"
#include "IO/Events/MarchEnded.hpp"
#include "IO/Events/MarchStarted.hpp"
#include "IO/Events/UnitAttacked.hpp"

namespace features::units
{
	void Unit::SetTarget(uint32_t x, uint32_t y, core::Environment& env)
	{
		target_x = x;
		target_y = y;
		env.Log<sw::io::MarchStarted>(GetId(), GetX(), GetY(), target_x, target_y);
	}

	bool Unit::March(core::Environment& env)
	{
		if (!CanMove())
		{
			return false;
		}
		if (HasTarget())
		{
			return StepToTarget(env);
		}
		return MoveRandom(env);
	}

	void Unit::TakeDamage(const uint32_t damage, const uint32_t damage_dealer_id, core::Environment& env)
	{
		auto damage_received = std::min(GetHP(), damage);
		hp -= damage_received;
		env.Log<sw::io::UnitAttacked>(damage_dealer_id, GetId(), damage_received, GetHP());
		if (hp == 0)
		{
			env.MoveToDead(GetId());
		}
	}

	bool Unit::StepToTarget(core::Environment& env)
	{
		int dx = std::clamp(
			static_cast<int>(target_x) - static_cast<int>(GetX()),
			-static_cast<int>(move_speed),
			static_cast<int>(move_speed));
		int dy = std::clamp(
			static_cast<int>(target_y) - static_cast<int>(GetY()),
			-static_cast<int>(move_speed),
			static_cast<int>(move_speed));

		uint32_t new_x = static_cast<int>(GetX()) + dx;
		uint32_t new_y = static_cast<int>(GetY()) + dy;

		if (env.CheckCoordinate(new_x, new_y) && env.IsCellEmpty(new_x, new_y))
		{
			bool res = env.UpdatePosition(GetId(), new_x, new_y);

			if (new_x == target_x && new_y == target_y)
			{
				target_x = NO_TARGET;
				target_y = NO_TARGET;
				env.Log<sw::io::MarchEnded>(GetId(), new_x, new_y);
			}

			return res;
		}
		return false;
	}

	bool Unit::MoveRandom(core::Environment& env)
	{
		static std::mt19937 rnd(std::random_device{}());
		std::vector<std::pair<uint32_t, uint32_t>> possible;

		for (int dx = -static_cast<int>(move_speed); dx <= static_cast<int>(move_speed); ++dx)
		{
			for (int dy = -static_cast<int>(move_speed); dy <= static_cast<int>(move_speed); ++dy)
			{
				if (((dx == 0) && (dy == 0)))
				{
					continue;
				}
				int nx = static_cast<int>(GetX()) + dx;
				int ny = static_cast<int>(GetY()) + dy;

				if (!env.CheckCoordinate(nx, ny))
				{
					continue;
				}

				if (env.IsCellEmpty(nx, ny))
				{
					possible.push_back({nx, ny});
				}
			}
		}

		if (possible.empty())
		{
			std::cout << "no possible\n";
			return false;
		}

		std::uniform_int_distribution<size_t> dist(0, possible.size() - 1);
		std::pair<uint32_t, uint32_t> next = possible[dist(rnd)];

		return env.UpdatePosition(GetId(), next.first, next.second);
	}

}
