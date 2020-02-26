#pragma once

struct Location
{
	int x;
	int y;

	void Add(const Location& val)
	{
		x += val.x;
		y += val.y;
	}

	bool operator== (const Location& rhs) const
	{
		return rhs.x == x && rhs.y == y;
	}
	
	bool operator!= (const Location& rhs) const
	{
		return ! (rhs.x == x && rhs.y == y);
	}

	Location operator*(const int& rhs) 
	{
		Location l{ x * rhs, y * rhs };
		
		return l;
	}
};