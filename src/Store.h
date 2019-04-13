#pragma once

#include <map>
#include <memory>
#include <iostream>

#include "SDL2Includes.h"

#include "Animation.h"
#include "Sprite.h"

namespace SDL2Wrapper
{

class Store
{
	Store();
	~Store();
	static std::unique_ptr<SDL_Renderer, SDL_Deleter>* rendererPtr;
	static std::map<std::string, std::unique_ptr<SDL_Texture, SDL_Deleter>> textures;
	static std::map<std::string, std::unique_ptr<SDL_Texture, SDL_Deleter>> textTextures;
	static std::map<std::string, std::unique_ptr<Sprite>> sprites;
	static std::map<std::string, std::unique_ptr<AnimationDefinition>> anims;
	static std::map<std::string, std::unique_ptr<TTF_Font, SDL_Deleter>> fonts;
	//static std::map<std::string, SDL_Texture*> fonttextures;

	static void createSprite(const std::string& name, std::unique_ptr<SDL_Texture, SDL_Deleter>& tex);
  public:
  	static void setRenderer(std::unique_ptr<SDL_Renderer, SDL_Deleter>& rendererA);

	static void createTexture(const std::string& name, const std::string& path);
	static void createFont(const std::string& name, const std::string& path);
	static void createSprite(const std::string& name, const std::string& textureName, const int x, const int y, const int w, const int h);
	static std::unique_ptr<AnimationDefinition>& createAnimationDefinition(const std::string& name, const bool loop);
  	static void storeTextTexture(const std::string& name, SDL_Texture* tex);

  	static void logSprites();
  	static void logAnimationDefinitions();
  	static void logFonts();

	static std::unique_ptr<SDL_Texture, SDL_Deleter>& getTexture(const std::string& name);
	static std::unique_ptr<SDL_Texture, SDL_Deleter>& getTextTexture(const std::string& name);
	static std::unique_ptr<Sprite>& getSprite(const std::string& name);
	static std::unique_ptr<AnimationDefinition>& getAnimationDefinition(const std::string& name); 
	static std::unique_ptr<TTF_Font, SDL_Deleter>& getFont(const std::string& name, const int sz, const bool isOutline = false);
};

} // namespace SDL2Wrapper
