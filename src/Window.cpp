#include "Window.h"
#include "Store.h"

namespace SDL2Wrapper
{

int Window::instanceCount = 0;
const double Window::targetFrameMS = 16.66666;

Window::Window()
	: events(*this)
{
}
Window::Window(const std::string& title, int widthA, int heightA)
	: events(*this), currentFontSize(18), deltaTime(0), isSoundEnabled(true), globalAlpha(255)
{
	Window::instanceCount++;
	createWindow(title, widthA, heightA);
}

Window::~Window()
{
	Window::instanceCount--;
	if (Window::instanceCount == 0)
	{
		Store::clear();
		Mix_Quit();
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
	}
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
		throw std::string("[SDL2Wrapper] ERROR Window could not be created! SDL Error: " + std::string(SDL_GetError()));
	}

	renderer = std::unique_ptr<SDL_Renderer, SDL_Deleter>(
		SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE),
		SDL_Deleter());
	SDL_SetRenderDrawColor(renderer.get(), 0x55, 0x55, 0x55, 0xFF);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		throw std::string("[SDL2Wrapper] ERROR SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

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
	double d = Window::targetFrameMS / deltaTime;
	return d;
}

void Window::setAnimationFromDefinition(const std::string& name, Animation& anim) const
{
	anim = Animation(Store::getAnimationDefinition(name));
}

const SDL_Color Window::makeColor(Uint8 r, Uint8 g, Uint8 b) const
{
	SDL_Color c = {r, g, b};
	return c;
}

void Window::disableSound()
{
	isSoundEnabled = false;
}
void Window::enableSound()
{
	isSoundEnabled = true;
}
void Window::playSound(const std::string& name)
{
	if (!isSoundEnabled)
	{
		return;
	}

	Mix_Chunk* sound = Store::getSound(name);
	Mix_PlayChannel(-1, sound, 0);
}
void Window::playMusic(const std::string& name)
{
	if (!isSoundEnabled)
	{
		return;
	}

	Mix_Music* music = Store::getMusic(name);
	if (Mix_PlayingMusic())
	{
		stopMusic();
	}
	Mix_PlayMusic(music, -1);
}
void Window::stopMusic()
{
	if (!isSoundEnabled)
	{
		return;
	}

	if (Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
}

SDL_Texture* Window::getTextTexture(const std::string& text, const int x, const int y, const int sz, const SDL_Color& color)
{
	if (!currentFontName.size())
	{
		throw std::string("[SDL2Wrapper] ERROR No font has been set.");
	}

	const std::string key = text + std::to_string(sz) + std::to_string(color.r) + std::to_string(color.g) + std::to_string(color.b);
	SDL_Texture* tex = Store::getTextTexture(key);
	if (tex)
	{
		return tex;
	}
	else
	{
		TTF_Font* font = Store::getFont(currentFontName, sz);
		SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), color);
		SDL_Texture* texPtr = SDL_CreateTextureFromSurface(renderer.get(), surf);
		SDL_FreeSurface(surf);
		Store::storeTextTexture(key, texPtr);
		return Store::getTextTexture(key);
	}
}

void Window::drawSprite(const std::string& name, const int x, const int y, const bool centered)
{
	Sprite& sprite = Store::getSprite(name);
	SDL_Texture* tex = sprite.image;
	SDL_SetTextureAlphaMod(tex, globalAlpha);
	SDL_Rect pos = {x + (centered ? -sprite.cw / 2 : 0), y + (centered ? -sprite.ch / 2 : 0), sprite.cw, sprite.ch};
	SDL_Rect clip = {sprite.cx, sprite.cy, sprite.cw, sprite.ch};
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
	SDL_Texture* tex = getTextTexture(text, x, y, currentFontSize, color);

	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &(w), &(h));
	SDL_SetTextureAlphaMod(tex, globalAlpha);
	SDL_Rect pos = {x, y, w, h};
	SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer.get(), tex, NULL, &pos);
}

void Window::drawTextCentered(const std::string& text, const int x, const int y, const SDL_Color& color)
{
	SDL_Texture* tex = getTextTexture(text, x, y, currentFontSize, color);

	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &(w), &(h));
	SDL_SetTextureAlphaMod(tex, globalAlpha);
	SDL_Rect pos = {x - w / 2, y - h / 2, w, h};
	SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer.get(), tex, NULL, &pos);
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
		if (!freq)
		{
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
		events.update();
		SDL_RenderPresent(renderer.get());
		firstLoop = false;
	}
}
} // namespace SDL2Wrapper
