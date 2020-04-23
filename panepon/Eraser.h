#pragma once
#include <Siv3D.hpp>
#include "Panel.h"
class Eraser {
	double flashTime;
	double preEraseTime;
	double erasingTime;
	int32 numErased;
	double postEraseTime;
	double Elapsed;
	Stopwatch sw;
	Array <Panel*> panels;

public:
	Eraser(
		Array<Panel*> panels,
		double flashTime = 0.66,
		double preEraseTime = 0.25,
		double eraseingTime = 0.166,
    	double postEraseTime = 0.5);

	void Update();

	bool Accomplished();
};

