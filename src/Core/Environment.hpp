#pragma once
#include "IO/Events/UnitNotSpawned.hpp"
#include "IO/Events/UnitSpawned.hpp"
#include "IO/System/EventLog.hpp"
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

namespace features
{
	class Object;

	namespace units
	{
		class Unit;
	}
}

namespace core
{

	namespace detail
	{
		struct PairHash
		{
			std::size_t operator()(const std::pair<uint32_t, uint32_t>& p) const noexcept
			{
				return std::hash<int>()(p.first) ^ (std::hash<uint32_t>()(p.second) << 1);
			}
		};

		struct PairEq
		{
			std::size_t operator()(
				const std::pair<uint32_t, uint32_t>& lhs, const std::pair<uint32_t, uint32_t>& rhs) const noexcept
			{
				return lhs == rhs;
			}
		};

	}

	namespace search_policy
	{
		struct AllObjects
		{
			bool operator()(const std::shared_ptr<features::Object>& /*object*/);
		};

		struct AliveOnly
		{
			bool operator()(const std::shared_ptr<features::Object>& object);
		};
	}

	class Environment
	{
	public:
		using UnitsVector = std::vector<std::shared_ptr<features::units::Unit>>;

		static std::unique_ptr<Environment> CreateMap(
			const uint32_t width, const uint32_t height, sw::EventLog& logger);

		uint64_t GetCurrentTick() const
		{
			return _current_tick;
		}

		template <typename EventType, typename... Args>
		void Log(Args&&... args) const
		{
			_logger.get().log(GetCurrentTick(), EventType{std::forward<Args>(args)...});
		}

		void IncreaseCurrentTick()
		{
			++_current_tick;
		}

		uint32_t GetWidth() const
		{
			return _width;
		}

		uint32_t GetHeight() const
		{
			return _height;
		}

		bool IsCellEmpty(uint32_t x, uint32_t y) const;

		bool CheckCoordinate(const uint32_t x, const uint32_t y) const
		{
			return (x >= 0) && (y >= 0) && (x < _width) && (y < _height);
		}

		template <typename UnitType, typename... Ts>
		void SpawnUnit(Ts&&... args)
		{
			auto unit = std::make_shared<UnitType>(std::forward<Ts>(args)...);
			uint32_t x = unit->GetX();
			uint32_t y = unit->GetY();
			uint32_t id = unit->GetId();
			if (CheckCoordinate(x, y) && IsCellEmpty(x, y) && unit->IsValid() && units_map.find(id) == units_map.end())
			{
				alive_units.push_back(unit);

				auto it = std::prev(alive_units.end());
				units_map[id] = it;

				objects_map[{x, y}] = unit;
				Log<sw::io::UnitSpawned>(id, unit->UNIT_TYPE, x, y);
			}
			else
			{
				Log<sw::io::UnitNotSpawned>(id, unit->UNIT_TYPE, x, y);
			}
		}

		const std::list<std::shared_ptr<features::units::Unit>>& GetAlive() const
		{
			return alive_units;
		}

		const UnitsVector& GetDead() const
		{
			return dead_units;
		}

		std::shared_ptr<features::units::Unit> FindById(uint32_t id);
		bool UpdatePosition(uint32_t id, uint32_t new_x, uint32_t new_y);
		bool RemoveObject(uint32_t id);

		template <typename SearchPolicy = search_policy::AllObjects>
		UnitsVector GetObjectsInRange(const unit32_t pos_x, const unit32_t pos_y, uint32_t min_r, uint32_t max_r) const
		{
			UnitsVector res;

			uint32_t idx = 0;
			for (int dy = -static_cast<int>(max_r); dy <= static_cast<int>(max_r); ++dy)
			{
				for (int dx = -static_cast<int>(max_r); dx <= static_cast<int>(max_r); ++dx, ++idx)
				{
					uint32_t dist = std::abs(dx) + std::abs(dy);
					if (dist < min_r || dist > max_r)
					{
						continue;
					}

					int x = static_cast<int>(pos_x) + dx;
					int y = static_cast<int>(pos_y) + dy;

					if (!CheckCoordinate(x, y))
					{
						continue;
					}

					auto obj = GetObjectAt(x, y);
					if (obj != nullptr)
					{
						if (auto u = std::dynamic_pointer_cast<features::units::Unit>(obj))
						{
							if (SearchPolicy{}(u))
							{
								res.push_back(u);
							}
						}
					}
				}
			}
			return res;
		}

		template <typename SearchPolicy = search_policy::AllObjects>
		UnitsVector GetAdjacentObjects(const unit32_t pos_x, const unit32_t pos_y) const
		{
			return GetObjectsInRange<SearchPolicy>(pos_x, pos_y, 1, 1);
		}

		void MoveToDead(uint32_t id);

	protected:
		Environment(uint32_t width, uint32_t height, sw::EventLog& logger) :
				_width(width),
				_height(height),
				_logger(logger)
		{}

	private:
		std::shared_ptr<features::Object> GetObjectAt(const uint32_t x, const uint32_t y) const;

		uint32_t _width{0U};
		uint32_t _height{0U};
		uint64_t _current_tick = 0;
		std::reference_wrapper<sw::EventLog> _logger;

		std::list<std::shared_ptr<features::units::Unit>> alive_units;
		UnitsVector dead_units;
		std::unordered_map<uint32_t, std::list<std::shared_ptr<features::units::Unit>>::iterator> units_map;
		std::unordered_map<
			std::pair<uint32_t, uint32_t>,
			std::shared_ptr<features::Object>,
			detail::PairHash,
			detail::PairEq>
			objects_map;
	};

}
