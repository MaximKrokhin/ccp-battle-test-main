#pragma once
#include "Core/Environment.hpp"
#include "Features/Unit.hpp"
#include "IO/Events/Winner.hpp"

namespace sw
{
	class Simulation
	{
	public:
		Simulation(std::unique_ptr<core::Environment> env) :
				env_(std::move(env))
		{}

		void Run()
		{
			auto& alive = env_->GetAlive();
			while (alive.size() > 1)
			{
				for (auto& unit : alive)
				{
					unit->Act(*env_);
				}
				env_->IncreaseCurrentTick();
			}
			if (alive.size())
			{
				auto winner = alive.back();
				env_->Log<io::Winner>(winner->GetId());
			}
		}

	private:
		std::unique_ptr<core::Environment> env_;
	};
}
