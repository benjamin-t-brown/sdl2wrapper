#include "Timer.h"

BoolTimer::BoolTimer(int maxFramesA, bool& refA)
	: removeFlag(false), ref(refA), maxFrames(maxFramesA), frame(0)
{
}

const bool BoolTimer::shouldRemove()
{
	return removeFlag;
}
void BoolTimer::remove()
{
	removeFlag = true;
}
void BoolTimer::update()
{
	if (!removeFlag)
	{
		frame++;
		if (frame > maxFrames)
		{
			remove();
			ref = !ref;
		}
	}
}
