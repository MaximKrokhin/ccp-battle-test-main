#pragma once
#include "Core/Environment.hpp"
#include "IO/System/CommandParser.hpp"

#include <Features/Hunter.hpp>
#include <Features/Swordsman.hpp>
#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/EventLog.hpp>
#include <fstream>

namespace core
{
	class EnvironmentInitializer
	{
	public:
		void Init(std::ifstream& file)
		{
			RegisterCommands();
			parser_.parse(file);
		}

		std::unique_ptr<Environment> GetEnvironment()
		{
			return std::move(env_);
		}

	private:
		void RegisterCommands()
		{
			parser_.add<sw::io::CreateMap>([this](auto c) { OnCreateMap(c); })
				.add<sw::io::SpawnSwordsman>([this](auto c) { OnSpawnSwordsman(c); })
				.add<sw::io::SpawnHunter>([this](auto c) { OnSpawnHunter(c); })
				.add<sw::io::March>([this](auto c) { OnMarch(c); });
		}

		void OnCreateMap(const sw::io::CreateMap& command)
		{
			env_ = core::Environment::CreateMap(command.width, command.height, eventLog_);
			if (!env_)
			{
				throw std::runtime_error("Error: Environment not initialized");
			}
		}

		void OnSpawnSwordsman(const sw::io::SpawnSwordsman& command)
		{
			if (!env_)
			{
				return;
			}
			env_->SpawnUnit<features::units::Swordsman>(
				command.unitId, command.x, command.y, command.hp, command.strength);
		}

		void OnSpawnHunter(const sw::io::SpawnHunter& command)
		{
			if (!env_)
			{
				return;
			}
			env_->SpawnUnit<features::units::Hunter>(
				command.unitId, command.x, command.y, command.hp, command.agility, command.strength, command.range);
		}

		void OnMarch(const sw::io::March& command)
		{
			if (!env_)
			{
				return;
			}
			auto unit = env_->FindById(command.unitId);
			if (unit && env_->CheckCoordinate(command.targetX, command.targetY))
			{
				unit->SetTarget(command.targetX, command.targetY, *env_);
			}
		}

	private:
		std::unique_ptr<core::Environment> env_{nullptr};
		sw::io::CommandParser parser_;
		sw::EventLog eventLog_;
	};
}
