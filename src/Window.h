#pragma once

#include <memory>

#include <functional>

#include "SDL2Includes.h"

#include "Animation.h"
#include "Events.h"
#include "Sprite.h"

namespace SDL2Wrapper
{

class Window
{
	std::unique_ptr<SDL_Window, SDL_Deleter> window;
	std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer;
	void (*onresize)(void);
	Events events;

	SDL_Texture* getTextTexture(const std::string& text, const int x, const int y, const int sz, const SDL_Color& color);
	SDL_Texture* getEmptyTexture(int w, int h);
	void onResize(int w, int h);
	void clear();
	void swap();
	void createWindow(const std::string& title = "SDL2Wrapper", const int w = 800, const int h = 600);

	std::string currentFontName;
	int currentFontSize;
	double deltaTime;
	
	static int instanceCount;

  public:
	int width;
	int height;
	int countedFrames;
	int fps;
	int globalAlpha;
	Uint32 colorkey;

	static const double targetFrameMS;

	Window();
	Window(const std::string& title, int widthA, int heightA);
	~Window();

	Events& getEvents();
	void setCurrentFont(const std::string& fontName, const int sz);
	const std::string& getCurrentFontName() const;
	const int getCurrentFontSize() const;
	const double getDeltaTime() const;
	const double getFrameRatio() const;

	void setAnimationFromDefinition(const std::string& name, Animation& anim) const;

	const SDL_Color makeColor(Uint8 r, Uint8 g, Uint8 b) const;

	void drawSprite(const std::string& name, const int x, const int y, const bool centered = true);
	void drawAnimation(Animation& anim, const int x, const int y, const bool centered = true, const bool updateAnim = true);
	void drawText(const std::string& text, const int x, const int y, const SDL_Color& color);
	void drawTextCentered(const std::string& text, const int x, const int y, const SDL_Color& color);
	void startRenderLoop(std::function<bool(void)> cb);
};
} // namespace SDL2Wrapper