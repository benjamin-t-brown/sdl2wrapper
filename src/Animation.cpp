#include "Animation.h"

#include <iostream>

namespace SDL2Wrapper
{

Animation::Animation()
	: name(""), sprite("invisible"), spriteIndex(0), frame(0), maxFrames(0), loop(true)
{
}

Animation::Animation(const std::string& nameA, const bool loopA)
	: name(nameA), sprite("invisible"), spriteIndex(0), frame(0), maxFrames(0), loop(loopA)
{
}

Animation::Animation(const Animation& a)
	: name(a.name), sprite(a.sprite), spriteIndex(a.spriteIndex), frame(a.frame), maxFrames(a.maxFrames), loop(a.loop)
{
	for (auto& pair: a.sprites)
	{
		addSprite(pair.first, pair.second);
	}
}

Animation& Animation::operator=(const Animation& a)
{
	if (this != &a)
	{
		sprites.clear();
		for (auto& pair: a.sprites)
		{
			addSprite(pair.first, pair.second);
		}
		name = a.name;
		sprite = a.sprite;
		spriteIndex = a.spriteIndex;
		frame = a.frame;
		maxFrames = a.maxFrames;
		loop = a.loop;
	}
	return *this;
}


bool Animation::isInitialized() const
{
	return sprites.size() > 0;
}

const std::string& Animation::getCurrentSpriteName() const
{
	return sprite;
}

std::string Animation::toString() const
{
	return name + " " + sprite;
}

void Animation::addSprite(const std::string& spriteName, int frames)
{
	if (!sprites.size())
	{
		sprite = spriteName;
	}
	sprites.push_back(std::make_pair(spriteName, frames));
}

void Animation::start()
{
	spriteIndex = 0;
	frame = 0;
	if (sprites.size())
	{
		maxFrames = sprites[0].second;
		sprite = sprites[0].first;
	}
}

void Animation::update()
{
	if (sprites.size())
	{
		frame++;
		if (frame >= maxFrames)
		{
			frame = 0;
			spriteIndex++;
			if (spriteIndex >= (int)sprites.size())
			{
				if (loop)
				{
					spriteIndex = 0;
				}
				else
				{
					spriteIndex--;
				}
			}
			maxFrames = sprites[spriteIndex].second;
			sprite = sprites[spriteIndex].first;
		}
	}
}

AnimationDefinition::AnimationDefinition(const std::string& nameA, const bool loopA) : Animation(nameA, loopA)
{

}

} // namespace SDL2Wrapper
