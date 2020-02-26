#include "Goal.h"

Goal::Goal(std::mt19937_64 rng, const Board & brd, const Snake & snake)
	:
	xDist(0, brd.GetGridWidth() - 1),
	yDist(0, brd.GetGridHeight() - 1)
{
	Respawn(rng, brd, snake);
}

void Goal::Respawn(std::mt19937_64 &rng, const Board & brd, const Snake & snake)
{
	Location newLoc;
	do
	{
		newLoc.x = xDist(rng);
		newLoc.y = yDist(rng);
	} while (snake.IsInTile(newLoc) || brd.CheckForObstacle(newLoc) || brd.CheckForPoson(newLoc));

	loc = newLoc;
}

void Goal::Draw(Board & brd) const
{
	brd.DrawCell(loc,c);
}

const Location& Goal::GetLocation() const
{
	return loc;
}
