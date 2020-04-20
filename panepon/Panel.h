#pragma once
# include <Siv3D.hpp>
class Panel {
	int32 x;
	int32 y;
	ColorF color;
	Rect rect;
	int32 size;

public:
	enum Type {
		NONE,
		RED,
		CYAN,
		GREEN,
		YELLOW,
		PURPLE,
		BLUE
	};

	static ColorF PanelColor(Panel::Type panelType) {
		ColorF colors[] = {
			ColorF(0, 0, 0, 0), // NONE
			ColorF(1, 0, 0, 1), // RED
			ColorF(0, 1, 1, 1), // CYAN
			ColorF(0, 1, 0, 1), // GREEN
			ColorF(1, 0, 1, 1), // YELLOW
			ColorF(1, 1, 0, 1), // PURPLE
			ColorF(0, 0, 1, 1), // BLUE
		};
		return colors[panelType];
	}

	Panel(int32 x, int32 y, Panel::Type type);

	void Draw(int32 xoffset = 0, int32 yoffset = 0, int32 padding = 0);


};