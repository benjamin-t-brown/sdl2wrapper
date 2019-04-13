#include "Sprite.h"

namespace SDL2Wrapper
{

Sprite::Sprite(const std::string& nameA, const int cxA, const int cyA, const int cwA, const int chA, const std::unique_ptr<SDL_Texture, SDL_Deleter>& imageA)
	: name(nameA), cx(cxA), cy(cyA), cw(cwA), ch(chA), image(imageA)
{
}
} // namespace SDL2Wrapper