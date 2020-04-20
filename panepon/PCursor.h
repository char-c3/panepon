#pragma once
#include <Siv3D.hpp>
class PCursor {
	int32 x;
	int32 y;
	int32 size;

public:
	PCursor(int32 x = 0, int32 y = 0);
	void Draw(int32 lift = 0);
	Point GetPos();
};

