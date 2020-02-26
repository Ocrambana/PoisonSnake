#include "Snake.h"
#include <algorithm>
#include <assert.h>
#include <iterator>

Snake::Snake(const Location & loc)
{
	Segment s(loc, Snake::headColor);
	segments.push_back(s);
}

void Snake::MoveBy(const Location& deltaLoc)
{
	for (size_t i{ segments.size() - 1 }; i > 0; --i)
	{
		segments[i].Follow(segments[i - 1]);
	}

	segments[0].MoveBy(deltaLoc);
}

Location Snake::GetNextHeadLocation(const Location & deltaLoc) const
{
	Location l(segments[0].GetLocation());
	l.Add(deltaLoc);
	return l;
}

void Snake::Grow()
{
	Color c = GetBodyColor();
	Segment s(segments.back().GetLocation(), c);
	segments.push_back(s);
}

Color Snake::GetBodyColor()
{
	return bodyColorScheme[( segments.size() - 1 ) % nBodyColors];
}


void Snake::Draw(Board& brd) const
{
	auto draw = [&brd](Segment s) { s.Draw(brd); };
	for_each(segments.begin(), segments.end(), draw);
}

Snake::Segment::Segment(const Location& loc, const Color color)
	:
	loc(loc),
	c(color)
{}

void Snake::Segment::Follow(const Segment & next)
{
	loc = next.loc;
}

void Snake::Segment::Draw(Board & brd) const
{
	brd.DrawCell(loc, c);
}

void Snake::Segment::MoveBy(const Location & deltaLoc)
{
	assert(abs(deltaLoc.x) + abs(deltaLoc.y) == 1);
	loc.Add(deltaLoc);
}

const Location& Snake::Segment::GetLocation() const
{
	return loc;
}

bool Snake::IsInTileExceptEnd(const Location & loc) const
{
	return TileCheck(segments.begin(), segments.end() - 1, loc);
}

bool Snake::IsInTile(const Location & loc) const
{
	return TileCheck(segments.begin(), segments.end(), loc);
}

void Snake::Reset(const Location & loc)
{
	segments.clear();
	Segment newHead(loc, Snake::headColor);
	segments.push_back(newHead);
}

bool Snake::TileCheck(std::vector<Segment>::const_iterator begin, std::vector<Segment>::const_iterator end, Location loc) const
{
	auto filter = [loc](Segment s) {
		Location l = s.GetLocation();
		return loc == l;
	};

	return std::any_of(begin, end, filter);
}
