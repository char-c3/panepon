#pragma once
# include <Siv3D.hpp>
class Panel {
public:
	enum Type {
		NONE,
		RED,
		CYAN,
		GREEN,
		YELLOW,
		PURPLE,
		BLUE,
	};

private:
	ColorF color;
	Type type;
	Rect rect;
	int32 size;

	bool erasing;
	bool tempErased;
	
public:
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

	Panel(Panel::Type type, int32 size = 32);

	void Draw(int32 x, int32 y, int32 padding = 0);

	void DrawFlowing(int32 x, int32 y, int32 padding = 0);

	Type GetType();

	void StartFlash();

	void EndFlash();

	void EraseTemporally();

	void EraseComplete();

	bool IsErasing();
};