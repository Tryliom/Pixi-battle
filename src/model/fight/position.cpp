#include "model/fight/position.h"

Position::Position(int x, int y)
{
    this->x = x;
    this->y = y;
}

Position Position::operator+(const Position& other) const
{
	return {this->x + other.x, this->y + other.y};
}

Position Position::operator-(const Position& other) const
{
	return {this->x - other.x, this->y - other.y};
}

Position Position::operator*(const Position& other) const
{
	return {this->x * other.x, this->y * other.y};
}

Position Position::operator/(const Position& other) const
{
	return {this->x / other.x, this->y / other.y};
}

Position Position::operator*(int value) const
{
	return {this->x * value, this->y * value};
}

Position Position::operator/(int value) const
{
	return {this->x / value, this->y / value};
}

Position Position::operator*(float value) const
{
	return {static_cast<int>(static_cast<float>(this->x) * value), static_cast<int>(static_cast<float>(this->y) * value)};
}

Position Position::operator/(float value) const
{
	return {static_cast<int>(static_cast<float>(this->x) / value), static_cast<int>(static_cast<float>(this->y) / value)};
}

bool Position::operator==(const Position& other) const
{
	return this->x == other.x && this->y == other.y;
}

void Position::operator+=(const Position& other)
{
	this->x += other.x;
	this->y += other.y;
}
