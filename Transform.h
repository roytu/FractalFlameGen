#pragma once

#include "ConstD.h"

class Transform
{
public:
	D2 position;
	D1 rotation;
	D2 scale;
	D1 color;

	D3 getNextPoint(D3 point);
};

