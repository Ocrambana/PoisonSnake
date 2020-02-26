#pragma once

#include "Graphics.h"
#include "Location.h"
#include <random>

enum BorderCellsType {None, Goal, Obstacle, Poison};

class Board
{
public:
	Board(Graphics& gfx);
	void DrawCell(const Location& loc, Color c);
	void DrawBorder();
	void DrawContent();
	int GetGridWidth() const;
	int GetGridHeight() const;
	bool IsInsideBoard(const Location &loc) const;
	void ResetBoard();

	/* obstacles management */
	bool CheckForObstacle(const Location &loc) const;
	void SpawnObstacle(std::mt19937_64& rng, const class Snake& snake);

	/* Poison management */
	bool CheckForPoison(const Location &loc) const;
	void SpawnPoison(std::mt19937_64& rng, const class Snake& snake);
	void ConsumePoison(const Location& loc);

	/* Goal Management */
	bool CheckForGoal(const Location& loc) const;
	void SpawnGoal(std::mt19937_64& rng, const class Snake& snake);
	void RespawnGoal(const Location& loc, std::mt19937_64 &rng, const Snake& snake);

private:
	Location GetFreeLocation(std::mt19937_64 &rng, const Snake& snake);

private:
	static constexpr int 
		dimension{ 20 },
		width{35},
		height{25},
		padding{3},
		border{5},
		xOffset{50},
		yOffset{40};

	static constexpr Color	
		borderColor = Colors::White,
		obstacleColor = Colors::Gray,
		foodColor = Colors::Red,
		poisonColor = Colors::MakeRGB(221, 160, 221);

	std::uniform_int_distribution<int>	
		xDist,
		yDist;

	BorderCellsType content[width * height] = { None };
	Graphics& gfx;
};