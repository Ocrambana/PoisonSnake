#pragma once

#include "Board.h"
#include "Location.h"
#include <vector>

class Snake
{
public:
	Snake(const Location& loc);
	~Snake() = default;

	void MoveBy(const Location& deltaLoc);
	Location GetNextHeadLocation(const Location& deltaLoc) const;
	void Grow();
	void Draw(Board& brd) const;
	bool IsInTileExceptEnd(const Location& loc) const;
	bool IsInTile(const Location& loc) const;
	void Reset(const Location& loc);

private:
	class Segment
	{
	public:
		Segment(const Location& loc, const Color color);
		void Follow(const Segment& next);
		void Draw(Board& brd) const;
		void MoveBy(const Location& deltaLoc);
		const Location& GetLocation() const;

	private:
		Location loc;
		Color c;
	};

private:
	std::vector<Snake::Segment> segments;
	static constexpr Color headColor = Colors::Yellow;
	static constexpr Location defLoc = {0,0};
	static constexpr Color bodyColorScheme[] = {
		{10, 100, 32},
		{10, 130, 48},
		{18, 160, 48},
		{10, 130, 48}
	};
	static constexpr int nBodyColors{ 4 };

	bool TileCheck(std::vector<Segment>::const_iterator begin, std::vector<Segment>::const_iterator end, Location loc) const;
	Color GetBodyColor();
	
};

