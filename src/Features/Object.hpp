#pragma once
#include <cstdint>

namespace features
{

	class Object
	{
	public:
		static constexpr const char* UNIT_TYPE = "Object";

		Object(uint32_t id, uint32_t x, uint32_t y) :
				id(id),
				x(x),
				y(y)
		{}

		virtual bool IsAlive()
		{
			return false;
		}

		virtual bool OccupiesCell()
		{
			return false;
		}

		virtual bool IsValid() const
		{
			return x >= 0 && y >= 0;
		}

		virtual ~Object() = default;

		uint32_t GetId() const
		{
			return id;
		}

		uint32_t GetX() const
		{
			return x;
		}

		uint32_t GetY() const
		{
			return y;
		}

		void SetPos(uint32_t new_x, uint32_t new_y)
		{
			x = new_x;
			y = new_y;
		}

	private:
		uint32_t id;
		uint32_t x;
		uint32_t y;
	};

}
