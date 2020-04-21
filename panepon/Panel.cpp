#include "Panel.h"

Panel::Panel(Panel::Type type, int32 size) 
	: type(type),
	  color(Panel::PanelColor(type)),
      size(size) {
}

void Panel::Draw(int32 x, int32 y, int32 padding) {
	rect = Rect(x + padding, y + padding, size - padding);
	rect.draw(color);
}

Panel::Type Panel::GetType() {
	return type;
}
