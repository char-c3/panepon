#include "PCursor.h"

PCursor::PCursor(int32 r, int32 c) : r(r), c(c) {
	size = 32;
}

void PCursor::Draw(int32 lift) {
	auto innerColor = ColorF(1, 1, 1, 1);
	auto innerStroke = 3;
	auto outerColor = ColorF(0, 0, 0, 1);
	auto outerStroke = 2;
	auto leftRect = Rect((r + 0) * 32 + outerStroke, c * 32 + outerStroke - lift, 32 - 2 * outerStroke);
	auto rightRect = Rect((r + 1) * 32 + outerStroke, c * 32 + outerStroke - lift, 32 - 2 * outerStroke);
	leftRect.drawFrame(innerStroke, 0, innerColor);
	rightRect.drawFrame(innerStroke, 0, innerColor);
	leftRect.drawFrame(0, outerStroke, outerColor);
	rightRect.drawFrame(0, outerStroke, outerColor);
}