#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <iostream>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <vector>
#include <random>

namespace gamelib
{
  class Window
  {
  protected:
    std::shared_ptr<SDL_Window> sdlWindow;
    std::shared_ptr<SDL_Renderer> sdlRenderer;
    SDL_Event sdlEvent;

    std::mt19937 rng;
    std::unordered_map<int, std::string> inverseKeymap;
    std::set<std::string> keysdown;
    float lastTime;
    bool running;
    bool focused;

  public:
    std::unordered_map<std::string, int> keymap;
    Window(const std::string &windowTitle, int windowWidth, int windowHeight);
    bool isOpen() const;
    void close();
    void processEvents();
    bool isKeyPressed(const std::string &keyId);
    float resetClock();
    void prepareRender();
    void presentRender();
    const std::shared_ptr<SDL_Renderer> &getRenderer() const;

    int getRandomInRangeInt(int lowInclusive, int highInclusive);
    double getRandomInRangeDouble(double lowInclusive, double highInclusive);
  };
}

#endif
