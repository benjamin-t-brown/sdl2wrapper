#include "Window.h"
#include "Store.h"

namespace SDL2Wrapper
{

int Window::instanceCount = 0;

Window::Window(const std::string& title, int widthA, int heightA)
	: currentFontSize(18), deltaTime(0), globalAlpha(255)
{
	Window::instanceCount++;
	createWindow(title, widthA, heightA);
}

Window::~Window()
{
	// Window::instanceCount--;
	// if (Window::instanceCount == 0)
	// {
	// 	SDL_Quit();
	// }
}

void Window::createWindow(const std::string& title, const int w, const int h)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	colorkey = 0x00FFFFFF;
	width = w;
	height = h;

	window = std::unique_ptr<SDL_Window, SDL_Deleter>(
		SDL_CreateWindow(title.c_str(),
						 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
						 width, height,
						 SDL_WINDOW_SHOWN),
		SDL_Deleter());
	if (window == NULL)
	{
		throw std::runtime_error("[SDL2Wrapper] ERROR Window could not be created! SDL Error: " + std::string(SDL_GetError()));
	}

	renderer = std::unique_ptr<SDL_Renderer, SDL_Deleter>(
		SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE),
		SDL_Deleter());
	SDL_SetRenderDrawColor(renderer.get(), 0x55, 0x55, 0x55, 0xFF);

	Store::setRenderer(renderer);
}

Events& Window::getEvents()
{
	return events;
}

void Window::setCurrentFont(const std::string& fontName, const int sz)
{
	currentFontName = fontName;
	currentFontSize = sz;
}
const std::string& Window::getCurrentFontName() const
{
	return currentFontName;
}
const int Window::getCurrentFontSize() const
{
	return currentFontSize;
}
const double Window::getDeltaTime() const
{
	return deltaTime;
}
const double Window::getFrameRatio() const
{
	double d = 16.666666 / deltaTime;
	return d;
}

void Window::setAnimationFromDefinition(const std::string& name, Animation& anim) const
{
	anim = static_cast<SDL2Wrapper::Animation>(*SDL2Wrapper::Store::getAnimationDefinition(name));
}

const SDL_Color Window::makeColor(Uint8 r, Uint8 g, Uint8 b) const
{
	SDL_Color c = {r, g, b};
	return c;
}

std::unique_ptr<SDL_Texture, SDL_Deleter>& Window::getTextTexture(const std::string& text, const int x, const int y, const int sz, const SDL_Color& color)
{
	if (!currentFontName.size())
	{
		throw std::string("[SDL2Wrapper] ERROR No font has been set.");
	}

	const std::string key = text + std::to_string(sz) + std::to_string(color.r) + std::to_string(color.g) + std::to_string(color.b);
	std::unique_ptr<SDL_Texture, SDL_Deleter>& tex = Store::getTextTexture(key);
	if (tex)
	{
		return tex;
	}
	else
	{
		std::unique_ptr<TTF_Font, SDL_Deleter>& font = Store::getFont(currentFontName, sz);
		SDL_Surface* surf = TTF_RenderText_Solid(font.get(), text.c_str(), color);
		SDL_Texture* texPtr = SDL_CreateTextureFromSurface(renderer.get(), surf);
		SDL_FreeSurface(surf);
		Store::storeTextTexture(key, texPtr);
		return Store::getTextTexture(key);
	}
}

void Window::drawSprite(const std::string& name, const int x, const int y, const bool centered)
{
	std::unique_ptr<Sprite>& sprite = Store::getSprite(name);
	SDL_Texture* tex = sprite->image.get();
	SDL_SetTextureAlphaMod(tex, globalAlpha);
	SDL_Rect pos = {x + (centered ? -sprite->cw / 2 : 0), y + (centered ? -sprite->ch / 2 : 0), sprite->cw, sprite->ch};
	SDL_Rect clip = {sprite->cx, sprite->cy, sprite->cw, sprite->ch};
	SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer.get(), tex, &clip, &pos);
}

void Window::drawAnimation(Animation& anim, const int x, const int y, const bool centered, const bool updateAnim)
{
	if (anim.isInitialized())
	{
		drawSprite(anim.getCurrentSpriteName(), x, y, centered);
		if (updateAnim)
		{
			anim.update();
		}
	}
	else
	{
		throw std::string("[SDL2Wrapper] ERROR Anim has not been initialized: '" + anim.toString() + "'");
	}
}

void Window::drawText(const std::string& text, const int x, const int y, const SDL_Color& color)
{
	std::unique_ptr<SDL_Texture, SDL_Deleter>& tex = getTextTexture(text, x, y, currentFontSize, color);

	int w, h;
	SDL_QueryTexture(tex.get(), NULL, NULL, &(w), &(h));
	SDL_SetTextureAlphaMod(tex.get(), globalAlpha);
	SDL_Rect pos = {x, y, w, h};
	SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer.get(), tex.get(), NULL, &pos);
}

void Window::drawTextCentered(const std::string& text, const int x, const int y, const SDL_Color& color)
{
	std::unique_ptr<SDL_Texture, SDL_Deleter>& tex = getTextTexture(text, x, y, currentFontSize, color);

	int w, h;
	SDL_QueryTexture(tex.get(), NULL, NULL, &(w), &(h));
	SDL_SetTextureAlphaMod(tex.get(), globalAlpha);
	SDL_Rect pos = {x - w / 2, y - h / 2, w, h};
	SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer.get(), tex.get(), NULL, &pos);
}

void Window::startRenderLoop(std::function<bool(void)> cb)
{
	bool loop = true;
	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 last = 0;
	double freq = 0;
	bool firstLoop = true;

	while (loop)
	{
		freq = (double)SDL_GetPerformanceFrequency();
		if (!freq) {
			freq = 1;
		}
		last = now;
		now = SDL_GetPerformanceCounter();
		if (firstLoop)
		{
			deltaTime = 16.6666;
		}
		else
		{
			deltaTime = ((now - last) * 1000 / (double)SDL_GetPerformanceFrequency());
		}

		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				loop = false;
				break;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				events.keydown(e.key.keysym.sym);
			}
			else if (e.type == SDL_KEYUP)
			{
				events.keyup(e.key.keysym.sym);
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				events.mousemove(x, y);
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				events.mousedown(x, y, (int)e.button.button);
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				events.mouseup(x, y, (int)e.button.button);
			}
		}
		if (!loop)
		{
			break;
		}

		SDL_RenderClear(renderer.get());
		loop = cb();
		SDL_RenderPresent(renderer.get());
		firstLoop = false;
	}
}
} // namespace SDL2Wrapper
