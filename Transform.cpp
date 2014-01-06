#include "Transform.h"

D3 Transform::getNextPoint(D3 point)
{
	float x = std::get<0>(point);
	float y = std::get<1>(point);
	float c = std::get<2>(point);

	// Scale
	x *= std::get<0>(scale);
	y *= std::get<1>(scale);

	// Rotation
	// Z-axis goes out of screen
	// TODO: 3D rotation, projection skew
	float r = std::get<0>(rotation);
	float rx, ry;
	rx = x * cos(r) - y * sin(r);
	ry = x * sin(r) + y * cos(r);
	x = rx;
	y = ry;

	// Position
	x += std::get<0>(position);
	y += std::get<1>(position);

	// Color
	c = (std::get<0>(color) + c) / 2;

	return D3(x, y, c);
}