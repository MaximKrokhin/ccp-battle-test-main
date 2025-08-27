#pragma once
#include "Features/Object.hpp"
#include "Core/Environment.hpp"
#include <random>
#include <vector>

namespace features::units
{

	class Unit : public Object
	{
	public:
		static constexpr const char* UNIT_TYPE = "Unit";
		static constexpr uint32_t NO_TARGET = std::numeric_limits<uint32_t>::max();

		Unit(uint32_t id, uint32_t x, uint32_t y, uint32_t hp, uint32_t move_speed = 1) :
				Object(id, x, y),
				hp(hp),
				move_speed(move_speed)
		{}

		~Unit() = default;

		uint32_t GetHP() const
		{
			return hp;
		}

		bool IsAlive() override
		{
			return GetHP() > 0;
		}

		bool OccupiesCell() override
		{
			return IsAlive();
		}

		bool IsValid() const override
		{
			return Object::IsValid() && hp >= 0 && move_speed >= 0;
		}

		void SetTarget(const uint32_t x, const uint32_t y, core::Environment& env);
		bool March(core::Environment& env);
		void TakeDamage(const uint32_t damage, const uint32_t damage_dealer_id, core::Environment& env);
		virtual void Act(core::Environment& map) = 0;

	protected:
		bool StepToTarget(core::Environment& env);
		bool MoveRandom(core::Environment& env);

	private:
		bool CanMove() const
		{
			return move_speed > 0;
		}

		bool HasTarget() const
		{
			return !(target_x == NO_TARGET || target_y == NO_TARGET);
		}

	protected:
		uint32_t hp;
		uint32_t move_speed{1};
		uint32_t target_x{NO_TARGET};
		uint32_t target_y{NO_TARGET};
	};

}
