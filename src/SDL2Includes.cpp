#include "SDL2Includes.h"

namespace SDL2Wrapper
{
void SDL_Deleter::operator()(SDL_Window* p) const
{
	SDL_DestroyWindow(p);
}
void SDL_Deleter::operator()(SDL_Renderer* p) const
{
	SDL_DestroyRenderer(p);
}
void SDL_Deleter::operator()(SDL_Texture* p) const
{
	SDL_DestroyTexture(p);
}
void SDL_Deleter::operator()(TTF_Font* p) const
{
	TTF_CloseFont(p);
}
} // namespace SDL2Wrapper