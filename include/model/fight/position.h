#pragma once

struct Position
{
	Position() = default;
    Position(int x, int y);

    int x = 0;
    int y = 0;

	Position operator+(const Position& other) const;
	Position operator-(const Position& other) const;
	Position operator*(const Position& other) const;
	Position operator/(const Position& other) const;
	Position operator*(int value) const;
	Position operator/(int value) const;
	Position operator*(float value) const;
	Position operator/(float value) const;
	bool operator==(const Position& other) const;
	void operator+=(const Position& other);
};
