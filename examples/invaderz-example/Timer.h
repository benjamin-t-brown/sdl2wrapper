#pragma once

class BoolTimer
{
	bool removeFlag;
	bool& ref;
	int maxFrames;
	int frame;

  public:
	BoolTimer(int maxFramesA, bool& refA);
	const bool shouldRemove();
	void remove();
	void update();
};