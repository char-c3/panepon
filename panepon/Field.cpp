#include "Field.h"

void Field::startSwap() {
	if (swapping) {
		return;
	}

	// TODO: “ü‚ê‘Ö‚¦‰Â”\”»’è

	swapSw.start();
	swapping = true;
}

void Field::updateSwap() {
	auto elapsed = swapSw.sF();
	if (elapsed < swapTime) {
		return;
	}

	endSwap();
}

void Field::drawSwap() {
	auto elapsed = swapSw.sF();

	auto cpos = cursor.GetPos();
	auto r = cpos.y;
	auto c = cpos.x;
	
	auto left = panelMat[r][c];
	auto right = panelMat[r][c + 1];
	auto rate = elapsed / swapTime;

	left .Draw((c     + rate) * panelSize, r * panelSize, panelPadding);
	right.Draw((c + 1 - rate) * panelSize, r * panelSize, panelPadding);
}

void Field::endSwap() {
	swapping = false;
	swapSw.reset();

	auto cpos = cursor.GetPos();
	auto r = cpos.y;
	auto c = cpos.x;

	auto tmp = panelMat[r][c];
	panelMat[r][c] = panelMat[r][c + 1];
	panelMat[r][c + 1] = tmp;
}

bool Field::isThisSwapping(int32 c, int32 r) {
	auto cpos = cursor.GetPos();
	return swapping && (c == cpos.x || c == cpos.x + 1) && r == cpos.y;
}

void Field::drop() {
	for (auto r : Iota(rowSize - 1, 0, -1)) {
		for (auto c : Iota(columnSize)) {
			if (panelMat[r][c].GetType() == Panel::Type::NONE) {
				panelMat[r][c] = panelMat[r - 1][c];
				panelMat[r - 1][c] = Panel(Panel::Type::NONE, panelSize);
			}
		}
	}
}

Field::Field(
	int32 rowSize,
	int32 columnSize,
	int32 panelSize,
	int32 panelPadding,
	double swapTime) 
	: columnSize(columnSize),
	  rowSize(rowSize),
      panelSize(panelSize),
      panelPadding(panelPadding),
	  cursor(PCursor(0, 0, panelSize)),
      swapping(false),
	  swapTime(swapTime),
	  swapSw() {
	for (auto r : Iota(rowSize)) {
		panelMat << PanelRow();
		for (auto c : Iota(columnSize)) {
			panelMat[r] << Panel(Panel::Type::NONE, panelSize);
		}
	}
}

void Field::Init() {
	for (auto c : Iota(columnSize)) {
		for (auto r : Iota(rowSize)) {
			if (c == 1 && r <= 6) {
				continue;
			}
			auto type = static_cast<Panel::Type>((r + c) % 6 + 1);
			panelMat[r][c] = Panel(type, panelSize);
		}
	}
}

void Field::Update() {
	drop();

	if (KeyZ.down()) {
		startSwap();
	}
		
	if (swapping) {
		updateSwap();
		return;
	}

	cursor.Update(columnSize, rowSize);
}

void Field::Draw(int32 x, int32 y) {
	for (auto r : Iota(rowSize)) {
		for (auto c : Iota(columnSize)) {
			if (isThisSwapping(c, r)) {
				continue;
			}

			panelMat[r][c].Draw(c * panelSize, r * panelSize, panelPadding);
		}
	}

	if (swapping) {
		drawSwap();
	}

	cursor.Draw();
}
