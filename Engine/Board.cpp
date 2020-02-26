#include "Board.h"
#include "Snake.h"
#include <assert.h>

Board::Board(Graphics& gfx)
	:
	gfx(gfx),
	xDist(0, width - 1),
	yDist(0, height - 1)
{}

void Board::DrawCell(const Location & loc, Color c)
{
	assert( loc.x >= 0);
	assert( loc.x <= width);
	assert( loc.y >= 0);
	assert( loc.y <= height);
	gfx.DrawRectDim(xOffset + loc.x * dimension, yOffset + loc.y * dimension, dimension - padding, dimension - padding, c);
}

void Board::DrawBorder()
{
	int xLimit{ xOffset + width * dimension },
		yLimit{ yOffset + height * dimension };

	for (int j{ yOffset - border }; j <= yLimit + border; j++)
		for (int i{ xOffset - border }; i <= xLimit + border; i++)
		{
			bool conditionToBeColored =
				(i > 0 && j > 0) && (i < Graphics::ScreenWidth && j < Graphics::ScreenHeight) &&
				(i < xOffset || i > xLimit) || (j < yOffset || j > yLimit);

			if (conditionToBeColored)
			{
				gfx.PutPixel(i,j, borderColor);
			}
		}
}

void Board::DrawContent()
{
	for (int j{0}; j < height; j++)
		for (int i{ 0 }; i < width; i++)
		{
			switch (content[j * width + i])
			{
				case Goal:
					DrawCell({i,j}, foodColor);
					break;
				
				case Obstacle:
					DrawCell({i,j}, obstacleColor);
					break;

				case Poison:
					DrawCell({i,j}, poisonColor);
					break;

				default:
					break;
			}
		}
}

int Board::GetGridWidth() const
{
	return width;
}

int Board::GetGridHeight() const
{
	return height;
}

bool Board::IsInsideBoard(const Location & loc) const
{
	return loc.x >= 0 && loc.x < width && loc.y >= 0 && loc.y < height;
}

void Board::ResetBoard()
{
	const int length = width * height;
	for (int i{ 0 }; i < length; i++)
	{
		content[i] = None;
	}
}

bool Board::CheckForObstacle(const Location & loc) const
{
	return content[loc.y * width + loc.x] == Obstacle;
}

void Board::SpawnObstacle(std::mt19937_64 & rng, const Snake & snake)
{
	Location newLoc = GetFreeLocation(rng, snake);

	content[newLoc.y * width + newLoc.x] = Obstacle;
}

bool Board::CheckForPoison(const Location & loc) const
{
	return content[loc.y * width + loc.x] == Poison;
}

void Board::SpawnPoison(std::mt19937_64 & rng, const Snake & snake)
{
	Location newLoc = GetFreeLocation(rng, snake);

	content[newLoc.y * width + newLoc.x] = Poison;
}

void Board::ConsumePoison(const Location & loc)
{
	assert(content[loc.y * width + loc.x] == Poison );
	content[loc.y * width + loc.x] = None;
}

bool Board::CheckForGoal(const Location & loc) const
{
	return content[loc.y * width + loc.x] == Goal;
}

void Board::SpawnGoal(std::mt19937_64 & rng, const Snake & snake)
{
	Location newLoc = GetFreeLocation(rng, snake);

	content[newLoc.y * width + newLoc.x] = Goal;
}

void Board::RespawnGoal(const Location & loc, std::mt19937_64 & rng, const Snake & snake)
{
	assert(content[loc.y * width + loc.x] == Goal);

	Location newLoc = GetFreeLocation(rng, snake);

	content[loc.y * width + loc.x] = None;
	content[newLoc.y * width + newLoc.x] = Goal;
}

Location Board::GetFreeLocation(std::mt19937_64 & rng, const Snake & snake)
{
	Location newLoc;
	do
	{
		newLoc.x = xDist(rng);
		newLoc.y = yDist(rng);
	} while (snake.IsInTile(newLoc) || CheckForPoison(newLoc) || CheckForGoal(newLoc) || CheckForObstacle(newLoc));

	return newLoc;
}
