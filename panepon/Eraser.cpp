#include "Eraser.h"

Eraser::Eraser(
	Array<Panel*> panels,
	double flashTime,
	double preEraseTime,
	double erasingTime,
	double postEraseTime) :
	panels(panels),
	flashTime(flashTime),
	preEraseTime(preEraseTime),
	erasingTime(erasingTime),
	numErased(0),
	postEraseTime(postEraseTime),
	sw() {}


void Eraser::Update() {
	if (panels.size() == 0) {
		return;
	}

	if (!sw.isStarted()) {
		sw.start();
	}

	// flash
	if (sw.sF() < flashTime) {
		for (auto panel : panels) {
			panel->StartFlash();
		}
		return;
	}

	// pre erase
	if (sw.sF() < flashTime + preEraseTime) {
		for (auto panel : panels) {
			panel->EndFlash();
		}
		return;
	}

	// erasing
	if (sw.sF() > flashTime + preEraseTime + erasingTime * numErased && numErased <= panels.size()) {
		for (auto i : Iota(numErased)) {
			panels[i]->EraseTemporally();
		}
		numErased++;
		return;
	}

	// post erase
	if (sw.sF() < flashTime + preEraseTime + erasingTime * panels.size() + postEraseTime) {
		return;
	}

	// end erase
	for (auto panel : panels) {
		panel->EraseComplete();
	}
	panels.clear();
}
