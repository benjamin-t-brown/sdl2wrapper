#pragma once

#include <memory>

#include "SDL2Includes.h"

namespace SDL2Wrapper
{

struct Sprite
{
  public:
	const std::string name;
	const int cx;
	const int cy;
	const int cw;
	const int ch;
	const std::unique_ptr<SDL_Texture, SDL_Deleter>& image;
	Sprite(const std::string& nameA, const int cxA, const int cyA, const int cwA, const int chA, const std::unique_ptr<SDL_Texture, SDL_Deleter>& imageA);
};

} // namespace SDL2Wrapper
