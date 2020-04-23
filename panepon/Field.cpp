#include "Field.h"

void Field::startSwap() {
	if (swapping) {
		return;
	}

	auto cpos = cursor.GetPos();
	auto r = cpos.y;
	auto c = cpos.x;

	if (panelMat[r][c].IsErasing() || panelMat[r][c + 1].IsErasing()) {
		return;
	}

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

	auto erasedPanels = getErasedPanels({Point(c, r), Point(c + 1, r)});
	auto eraser = Eraser(erasedPanels);
	erasers << eraser;
}

bool Field::isThisSwapping(int32 c, int32 r) {
	auto cpos = cursor.GetPos();
	return swapping && (c == cpos.x || c == cpos.x + 1) && r == cpos.y;
}

void Field::drop() {
	if (dropSw.sF() < dropTime) {
		return;
	}
	dropSw.restart();

	Array<Point> detonators;
	auto cpos = cursor.GetPos();
	for (auto r : Iota(rowSize - 1, 0, -1)) {
		for (auto c : Iota(columnSize)) {
			if (panelMat[r][c].GetType() != Panel::Type::NONE) {
				continue;
			}
			panelMat[r][c] = panelMat[r - 1][c];
			panelMat[r - 1][c] = Panel(Panel::Type::NONE, panelSize);
			
			// detonator”»’è
			bool thisIsDetonator = true;
			for (auto dr : Iota(r, rowSize)) {
				bool thereIsBlank = panelMat[dr][c].GetType() == Panel::Type::NONE;
				if (thereIsBlank) {
					thisIsDetonator = false;
					break;
				}
				bool thereIsSwapping = swapping && (dr == cpos.y || dr == cpos.y + 1);
				if (thereIsSwapping) {
					break;
				}
			}

			if (thisIsDetonator) {
				detonators << Point(c, r);
			}
		}
	}

	auto erasedPanels = getErasedPanels(detonators);
	auto eraser = Eraser(erasedPanels);
	erasers << eraser;
}

Array<Panel*> Field::getErasedPanels(Array<Point> detonators) {
	Array<Array<bool> > erasedFlags;
	for (auto r : Iota(rowSize)) {
		erasedFlags << Array<bool>(columnSize, false);
	}
	
	for (auto detonator : detonators) {
		auto r = detonator.y;
		auto c = detonator.x;
		
		// NONE‚È‚ç–³Ž‹‚·‚é
		if (panelMat[r][c].GetType() == Panel::Type::NONE) {
			continue;
		}

		// vertical
		auto top = r;
		auto bottom = r;
		// search to top
		for (auto dr : Iota(r, -1, -1)) {
			if (panelMat[dr][c].GetType() != panelMat[r][c].GetType()) {
				break;
			}
			top = dr;
		}
		for (auto dr : Iota(r, rowSize)) {
			if (panelMat[dr][c].GetType() != panelMat[r][c].GetType()) {
				break;
			}
			bottom = dr;
		}
		// c‚É3‚ÂˆÈã˜A‚È‚Á‚Ä‚¢‚È‚¢‚ÆAÁ‚¦‚Ä‚¢‚é‚Æ”»’f‚³‚ê‚È‚¢
		if (bottom - top + 1 >= 3) {
			for (auto dr : Iota(top, bottom + 1)) {
				erasedFlags[dr][c] = true;
			}
		}

		// horizontal
		auto left = c;
		auto right = c;
		for (auto dc : Iota(c, -1, -1)) {
			if (panelMat[r][dc].GetType() != panelMat[r][c].GetType()) {
				break;
			}
			left = dc;
		}
		for (auto dc : Iota(c, columnSize)) {
			if (panelMat[r][dc].GetType() != panelMat[r][c].GetType()) {
				break;
			}
			right = dc;
		}
		// ‰¡‚à“¯—l‚É3‚ÂˆÈã˜A‚È‚Á‚Ä‚¢‚È‚¢‚ÆAÁ‚¦‚Ä‚¢‚é‚Æ”»’f‚³‚ê‚È‚¢
		if (right - left + 1 >= 3) {
			for (auto dc : Iota(left, right + 1)) {
				erasedFlags[r][dc] = true;
			}
		}
	}

	Array<Panel*> ret;
	for (auto r : Iota(rowSize)) {
		for (auto c : Iota(columnSize)) {
			if (erasedFlags[r][c]) {
				ret << &panelMat[r][c];
			}
		}
	}
	return ret;
}

void Field::updateErasers() {
	for (auto&& eraser : erasers) {
		eraser.Update();
	}

	erasers.remove_if([](Eraser eraser) {
		return eraser.Accomplished();
	});
}

Field::Field(
	int32 rowSize,
	int32 columnSize,
	int32 panelSize,
	int32 panelPadding,
	double swapTime,
	double dropTime) 
	: columnSize(columnSize),
	  rowSize(rowSize),
      panelSize(panelSize),
      panelPadding(panelPadding),
	  cursor(PCursor(0, 0, panelSize)),
      swapping(false),
	  swapTime(swapTime),
	  swapSw(),
	  dropTime(dropTime),
	  dropSw(),
	  erasers() {
	for (auto r : Iota(rowSize)) {
		panelMat << PanelRow();
		for (auto c : Iota(columnSize)) {
			panelMat[r] << Panel(Panel::Type::NONE, panelSize);
		}
	}
	dropSw.start();
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
	updateErasers();
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
