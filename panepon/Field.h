#pragma once
#include <Siv3D.hpp>
#include "Panel.h"
#include "PCursor.h"

using PanelRow = Array<Panel>;
using PanelMat = Array<PanelRow>;

class Field {
	PanelMat panelMat;

	PCursor cursor;

	int32 columnSize;
	int32 rowSize;
	int32 panelSize;
	int32 panelPadding;
public:
	Field(int32 rowSize = 12,
		int32 columnSize = 6,
		int32 panelSize = 32,
		int32 panelPadding = 2);

	void Init();

	void Draw(int32 x = 0, int32 y = 0);

	void Swap();
};

