#pragma once
#include <Siv3D.hpp>
class PCursor {
	int32 r;
	int32 c;
	int32 size;

public:
	PCursor(int32 r = 0, int32 c = 0);
	void Draw(int32 lift = 0);
};

