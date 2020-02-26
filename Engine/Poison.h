#pragma once

#include "Graphics.h"
#include "Board.h"
#include "Location.h"
#include <random>

class Poison
{
public:
	Poison();
	void Draw(Board& brd) const;
	void Consume(Location& loc);
	bool HasPoison(const Location& loc) const;
private:
	static constexpr int poolsNumber = 500;
	static constexpr Color poisonColor = Colors::MakeRGB(153, 50, 204);
	Location pools[poolsNumber];

};

