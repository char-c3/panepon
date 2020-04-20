#include "Panel.h"

Panel::Panel(int32 x, int32 y, Panel::Type type) : x(x), y(y), color(Panel::PanelColor(type)) {
	size = 32;
}

void Panel::Draw(int32 xoffset, int32 yoffset, int32 padding) {
	rect = Rect(x + xoffset + padding, y + yoffset + padding, size - padding);
	rect.draw(color);
}
