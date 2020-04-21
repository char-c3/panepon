#include "PCursor.h"

PCursor::PCursor(int32 x, int32 y, int32 panelSize) 
	: x(x), y(y), panelSize(panelSize) {
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