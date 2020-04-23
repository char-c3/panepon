
#include <Siv3D.hpp> // OpenSiv3D v0.4.2
#include "Panel.h"
#include "PCursor.h"
#include "Field.h"

void Main()
{
	// 背景を水色にする
	Scene::SetBackground(ColorF(0.8, 0.9, 1.0));
	
	Field field(12, 6, 32, 2, 0.033, 0.016);
	field.Init();

	while (System::Update())
	{
		ClearPrint();

		field.Update();
		field.Draw();
	}
}
