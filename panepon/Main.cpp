
#include <Siv3D.hpp> // OpenSiv3D v0.4.2
#include "Panel.h"
#include "PCursor.h"

void Main()
{
	// 背景を水色にする
	Scene::SetBackground(ColorF(0.8, 0.9, 1.0));
	
	std::vector<std::shared_ptr<Panel>> panels;
	PCursor cursor;

	for (int32 r = 0; r < 12; r++) {
		for (int32 c = 0; c < 6; c++) {
			if (r <= 1 && c == 1) {
				continue;
			}
			panels.push_back(std::make_shared<Panel>(Panel(c * 32, r * 32, static_cast<Panel::Type>(Random(1, 6)))));
		}
	}
	
	while (System::Update())
	{
		for (auto panel : panels) {
			panel->Draw(0, 0, 2);
		}

		cursor.Draw();

		if (KeyZ.down()) {
			// swap
			auto cpos = cursor.GetPos();

			std::shared_ptr<Panel> left = nullptr, right = nullptr;
			for (auto panel : panels) {
				auto ppos = panel->GetPos();

				if (ppos.x == cpos.x * 32 && ppos.y == cpos.y * 32) {
					left = panel;
				}
				if (ppos.x == (cpos.x + 1) * 32 && ppos.y == cpos.y * 32) {
					right = panel;
				}
			}

			if (left != nullptr) {
				auto lpos = left->GetPos();
				left->SetPos(Point(lpos.x + 32, lpos.y));
			}

			if (right != nullptr) {
				auto rpos = right->GetPos();
				right->SetPos(Point(rpos.x - 32, rpos.y));
			}
			
		}
	}
}
