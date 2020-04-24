#include "PCursor.h"

PCursor::PCursor(int32 x, int32 y, int32 panelSize) 
	: x(x), y(y), panelSize(panelSize) {
}

void PCursor::Update(int32 columnSize, int32 rowSize) {
	if (KeyLeft.down()) {
		x = Clamp(x - 1, 0, columnSize - 2);
	} else if (KeyRight.down()) {
		x = Clamp(x + 1, 0, columnSize - 2);
	} else if (KeyUp.down()) {
		y = Clamp(y - 1, 0, rowSize - 1);
	} else if (KeyDown.down()) {
		y = Clamp(y + 1, 0, rowSize - 1);
	}
}

void PCursor::Draw(int32 lift) {
	auto innerColor = ColorF(1, 1, 1, 1);
	auto innerStroke = 3;
	auto outerColor = ColorF(0, 0, 0, 1);
	auto outerStroke = 2;
	auto leftRect = Rect(
						(x + 0) * panelSize + outerStroke,
						y * panelSize + outerStroke - lift,
						panelSize - 2 * outerStroke);
	auto rightRect = Rect(
						(x + 1) * panelSize + outerStroke,
						y * panelSize + outerStroke - lift,
						panelSize - 2 * outerStroke);

	leftRect.drawFrame(innerStroke, 0, innerColor);
	rightRect.drawFrame(innerStroke, 0, innerColor);
	leftRect.drawFrame(0, outerStroke, outerColor);
	rightRect.drawFrame(0, outerStroke, outerColor);
}

Point PCursor::GetPos() {
	return Point(x, y);
}

void PCursor::Flow() {
	y--;
}
