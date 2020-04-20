
#include <Siv3D.hpp> // OpenSiv3D v0.4.2
#include "Panel.h"
#include "PCursor.h"

void Main()
{
	// 背景を水色にする
	Scene::SetBackground(ColorF(0.8, 0.9, 1.0));
	
	std::vector<std::shared_ptr<Panel>> panels;
	PCursor cursor;

	for (int32 r = 0; r < 6; r++) {
		for (int32 c = 0; c < 12; c++) {
			panels.push_back(std::make_shared<Panel>(Panel(r * 32, c * 32, Panel::Type::BLUE)));
		}
	}
	
	while (System::Update())
	{
		for (auto panel : panels) {
			panel->Draw(0, 0, 2);
		}

		cursor.Draw();
	}
}
