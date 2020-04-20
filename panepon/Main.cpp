
# include <Siv3D.hpp> // OpenSiv3D v0.4.2
# include "Panel.h"



void Main()
{
	// 背景を水色にする
	Scene::SetBackground(ColorF(0.8, 0.9, 1.0));

	Panel panel(240, 240, Panel::Type::BLUE);
	while (System::Update())
	{
		panel.Draw();
	}
}
