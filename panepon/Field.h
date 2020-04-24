#pragma once
#include <Siv3D.hpp>
#include "Panel.h"
#include "PCursor.h"
#include "Eraser.h"

using PanelRow = Array<Panel>;
using PanelMat = Array<PanelRow>;

class Field {
	// �p�l���֘A
	PanelMat panelMat;
	PCursor cursor;
	int32 columnSize;
	int32 rowSize;
	int32 panelSize;
	int32 panelPadding;

	// swap�֘A
	bool swapping;
	Stopwatch swapSw;
	double swapTime; // sec
	void startSwap();
	void updateSwap();
	void drawSwap();
	void endSwap();
	bool isThisSwapping(int32 c, int32 r);
	
	// �����֘A
	Stopwatch dropSw;
	double dropTime;
	void drop();

	// �����֘A
	Array<Eraser> erasers;
	Array<Panel*> getErasedPanels(Array<Point> detonators);
	void updateErasers();

	// ���̃p�l���𐶐�
	PanelRow next;
	Stopwatch flowSw;
	double flowRate;
	double flowTime;
	void updateFlow();
	void flow();

public:
	Field(int32 rowSize = 12,
		int32 columnSize = 6,
		int32 panelSize = 32,
		int32 panelPadding = 2,
		double swapTime = 2.0 / 60,
		double dropTime = 1.0 / 120,
		double flowTime = 10);

	void Init();

	void Update();

	void Draw(int32 x = 0, int32 y = 0);
};

