#pragma once
#include <Siv3D.hpp>
#include "Panel.h"
#include "PCursor.h"

using PanelRow = Array<Panel>;
using PanelMat = Array<PanelRow>;

class Field {
	// パネル関連
	PanelMat panelMat;
	PCursor cursor;
	int32 columnSize;
	int32 rowSize;
	int32 panelSize;
	int32 panelPadding;

	// swap関連
	bool swapping;
	Stopwatch swapSw;
	double swapTime; // sec
	void startSwap();
	void updateSwap();
	void drawSwap();
	void endSwap();
	bool isThisSwapping(int32 c, int32 r);
	
	// 落下関連
	void drop();
public:
	Field(int32 rowSize = 12,
		int32 columnSize = 6,
		int32 panelSize = 32,
		int32 panelPadding = 2,
		double swapTime = 0.033);

	void Init();

	void Update();

	void Draw(int32 x = 0, int32 y = 0);
};

