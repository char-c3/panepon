#include "Field.h"

Field::Field(int32 rowSize, int32 columnSize, int32 panelSize, int32 panelPadding) 
	: columnSize(columnSize),
	  rowSize(rowSize),
      panelSize(panelSize),
      panelPadding(panelPadding),
	  cursor(PCursor(0, 0, panelSize)) {
	for (auto c : Iota(columnSize)) {
		panelMat << PanelRow();
		for (auto r : Iota(rowSize)) {
			panelMat[c] << Panel(Panel::Type::NONE, panelSize);
		}
	}
}

void Field::Init() {
	for (auto c : Iota(columnSize)) {
		for (auto r : Iota(rowSize)) {
			auto type = static_cast<Panel::Type>((r + c) % 6 + 1);
			panelMat[c][r] = Panel(type, panelSize);
		}
	}
}

void Field::Draw(int32 x, int32 y) {
	for (auto c : Iota(columnSize)) {
		for (auto r : Iota(rowSize)) {
			panelMat[c][r].Draw(c * panelSize, r * panelSize, panelPadding);
		}
	}

	cursor.Draw();
}

void Field::Swap() {
	auto cpos = cursor.GetPos();
	auto c = cpos.x;
	auto r = cpos.y;

	auto tmp = panelMat[c][r];

	panelMat[c][r] = panelMat[c + 1][r];
	panelMat[c + 1][r] = tmp;
}
