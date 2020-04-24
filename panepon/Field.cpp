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
	auto h = (r - flowRate) * panelSize;

	left .Draw((c     + rate) * panelSize, h, panelPadding);
	right.Draw((c + 1 - rate) * panelSize, h, panelPadding);
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
			
			// detonator判定
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
		
		// NONEなら無視する
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
		// 縦に3つ以上連なっていないと、消えていると判断されない
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
		// 横も同様に3つ以上連なっていないと、消えていると判断されない
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

void Field::updateFlow() {
	if (next.size() == 0) {
		for (auto c : Iota(columnSize)) {
			next << Panel(static_cast<Panel::Type>(Random(1, 6)), panelSize);
		}
		flowSw.restart();
	}

	flowRate = flowSw.sF() / flowTime;

	if (flowRate >= 1) {
		flowRate = 0;
		// ゲームオーバー判定開始
		// ゲームオーバーなら終わらせる

		// ゲームオーバーじゃない & 猶予時間終了したらせり上げる
		flow();
		// せりあがったブロックを中心に消去判定を行う
		flowSw.restart();
	}
}

void Field::flow() {
	for (auto r : Iota(1, rowSize)) {
		for (auto c : Iota(columnSize)) {
			panelMat[r - 1][c] = panelMat[r][c];
		}
	}

	for (auto c : Iota(columnSize)) {
		panelMat[rowSize - 1][c] = next[c];
	}

	next.clear();
	cursor.Flow();
}


Field::Field(
	int32 rowSize,
	int32 columnSize,
	int32 panelSize,
	int32 panelPadding,
	double swapTime,
	double dropTime,
	double flowTime
) : columnSize(columnSize),
	rowSize(rowSize),
	panelSize(panelSize),
	panelPadding(panelPadding),
	cursor(PCursor((columnSize - 1) / 2, (rowSize - 1) / 2, panelSize)),
	swapping(false),
	swapTime(swapTime),
	swapSw(),
	dropTime(dropTime),
	dropSw(),
	erasers(),
	next(),
	flowSw(),
	flowRate(),
	flowTime(flowTime) {
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
			if (r <= 3 || c == 1 && r <= 6) {
				continue;
			}
			auto type = static_cast<Panel::Type>((r + c) % 6 + 1);
			panelMat[r][c] = Panel(type, panelSize);
		}
	}
	
	flowSw.start();
}

void Field::Update() {
	bool stopping = swapping || erasers.size() != 0;
	if (stopping) {
		flowSw.pause();
	}

	updateErasers();
	drop();

	if (KeyZ.down()) {
		startSwap();
	}
		
	if (swapping) {
		updateSwap();
	}

	cursor.Update(columnSize, rowSize);

	if (stopping) {
		return;
	}

	flowSw.resume();

	updateFlow();
	
}

void Field::Draw(int32 x, int32 y) {
	for (auto r : Iota(rowSize)) {
		for (auto c : Iota(columnSize)) {
			if (isThisSwapping(c, r)) {
				continue;
			}

			panelMat[r][c].Draw(c * panelSize, (r - flowRate) * panelSize, panelPadding);
		}
	}

	for (auto c : Iota(next.size())) {
		next[c].DrawFlowing(c * panelSize, (rowSize - flowRate) * panelSize, panelPadding);
	}

	if (swapping) {
		drawSwap();
	}

	cursor.Draw(panelSize * flowRate);
}
