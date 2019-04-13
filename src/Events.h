#pragma once

#include "SDL2/SDL.h"

#include <stack>
#include <map>
#include <memory>
#include <functional>
#include <iostream>

class EventRoute;

class Events {
private:
	std::stack<std::unique_ptr<EventRoute>> routes;
	std::map<std::string, bool> keys;
public:
	bool isMouseDown;
	bool isRightMouseDown;
	int mouseX;
	int mouseY;
	int mouseDownX;
	int mouseDownY;

	Events();
	~Events();
	bool isKeyPressed(const std::string& name) const;
	bool isCtrl() const;

	void pushRoute();
	void popRoute();
	void setMouseEvent(const std::string& name, std::function<void(int,int)> cb);
	void setKeyboardEvent(const std::string& name, std::function<void(const std::string&)> cb);

	void mousedown(int x, int y, int button);
	void mouseup(int x, int y, int button);
	void mousemove(int x, int y);
	void keydown(int key);
	void keyup(int key);
};