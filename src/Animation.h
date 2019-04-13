#pragma once

#include <memory>
#include <vector>
namespace SDL2Wrapper
{
class AnimationDefinition;

class Animation
{
	std::string name;
	std::string sprite;
	int spriteIndex;
	int frame;
	int maxFrames;
	bool loop;
	std::vector<std::pair<std::string, int>> sprites;

  public:
	Animation();
	Animation(const std::string& nameA, const bool loopA);
	Animation(const Animation& a);

	Animation& operator=(const Animation& a);

	bool isInitialized() const;
	const std::string& getCurrentSpriteName() const;
	std::string toString() const;

	void addSprite(const std::string& spriteName, const int frames);
	void start();
	void update();
};

class AnimationDefinition : public Animation
{
  public:
	AnimationDefinition(const std::string& nameA, const bool loopA);
};
} // namespace SDL2Wrapper