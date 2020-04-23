#include "Panel.h"

Panel::Panel(Panel::Type type, int32 size) 
	: type(type),
	  color(Panel::PanelColor(type)),
      size(size),
      erasing(false),
      tempErased(false) {
}

void Panel::Draw(int32 x, int32 y, int32 padding) {
	rect = Rect(x + padding, y + padding, size - padding);
	auto c = color;
	if (tempErased) {
		c.a *= 0;
	} else if (erasing) {
		c.a *= 0.3;
	}
	rect.draw(c);
}

Panel::Type Panel::GetType() {
	return type;
}

void Panel::StartFlash() {
	erasing = true;
}

void Panel::EndFlash() {
	erasing = true;
}

void Panel::EraseTemporally() {
	erasing = true;
	tempErased = true;
}

void Panel::EraseComplete() {
	erasing = false;
	tempErased = false;
	type = Panel::Type::NONE;
}