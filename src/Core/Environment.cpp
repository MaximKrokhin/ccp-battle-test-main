#include "Core/Environment.hpp"
#include "IO/Events/MapCreated.hpp"
#include "IO/Events/UnitDied.hpp"
#include "IO/Events/UnitMoved.hpp"
#include "Features/Unit.hpp"

namespace core
{

	namespace search_policy
	{

		bool AllObjects::operator()(const std::shared_ptr<features::Object>& /*object*/)
		{
			return true;
		}

		bool AliveOnly::operator()(const std::shared_ptr<features::Object>& object)
		{
			return object->IsAlive();
		}

	}

	bool Environment::IsCellEmpty(uint32_t x, uint32_t y) const
	{
		auto it = objects_map.find({x, y});
		return it == objects_map.end() || !it->second->OccupiesCell();
	}

	std::shared_ptr<features::units::Unit> Environment::FindById(uint32_t id)
	{
		auto it = units_map.find(id);
		if (it == units_map.end())
		{
			return nullptr;
		}
		return *(it->second);
	}

	bool Environment::UpdatePosition(uint32_t id, uint32_t new_x, uint32_t new_y)
	{
		auto obj = FindById(id);

		if (!obj)
		{
			return false;
		}

		objects_map.erase({obj->GetX(), obj->GetY()});
		obj->SetPos(new_x, new_y);
		objects_map[{new_x, new_y}] = obj;
		Log<sw::io::UnitMoved>(obj->GetId(), new_x, new_y);
		return true;
	}

	bool Environment::RemoveObject(uint32_t id)
	{
		auto obj = FindById(id);
		if (!obj)
		{
			return false;
		}

		objects_map.erase({obj->GetX(), obj->GetY()});
		units_map.erase(id);

		alive_units.erase(std::remove(alive_units.begin(), alive_units.end(), obj), alive_units.end());

		return true;
	}

	void Environment::MoveToDead(uint32_t id)
	{
		auto it = units_map.find(id);
		if (it == units_map.end())
		{
			return;
		}

		auto unit_it = it->second;

		dead_units.push_back(*unit_it);
		alive_units.erase(unit_it);
		units_map.erase(it);

		Log<sw::io::UnitDied>(id);
	}

	std::shared_ptr<features::Object> Environment::GetObjectAt(const uint32_t x, const uint32_t y) const
	{
		auto it = objects_map.find({x, y});
		return (CheckCoordinate(x, y) && (it != objects_map.end())) ? it->second : nullptr;
	}

	std::unique_ptr<Environment> Environment::CreateMap(
		const uint32_t width, const uint32_t height, sw::EventLog& logger)
	{
		struct Enabler : public Environment
		{
			Enabler(uint32_t w, uint32_t h, sw::EventLog& logger) :
					Environment(w, h, logger)
			{}
		};

		auto env = std::make_unique<Enabler>(width, height, logger);
		env->Log<sw::io::MapCreated>(width, height);
		return env;
	}

}
