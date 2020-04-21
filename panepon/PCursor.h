#pragma once
#include <Siv3D.hpp>
class PCursor {
	int32 x;
	int32 y;
	int32 panelSize;

public:
	PCursor(int32 x = 0, int32 y = 0, int32 panelSize = 32);
	void Draw(int32 lift = 0);
	Point GetPos();
};

