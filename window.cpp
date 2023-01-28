#include "window.h"

// within this file we want to declare that we can see within the namespace of the class
using namespace gamelib;

Window::Window(const std::string &windowTitle, int windowWidth, int windowHeight)
{
  std::seed_seq seed{
      std::random_device{}(),
      static_cast<unsigned int>(::time(nullptr)),
      static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())};
  rng.seed(seed);

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    throw std::runtime_error("Unable to initialize SDL:" + std::string(SDL_GetError()));
  }
  ::atexit(SDL_Quit);

  sdlWindow = std::shared_ptr<SDL_Window>(
      SDL_CreateWindow(
          windowTitle.c_str(),
          SDL_WINDOWPOS_CENTERED,
          SDL_WINDOWPOS_CENTERED,
          windowWidth, windowHeight, 0),
      [](SDL_Window *windowPtr)
      { SDL_DestroyWindow(windowPtr); });
  if (!sdlWindow.get())
  {
    throw std::runtime_error("Unable to create SDL window:" + std::string(SDL_GetError()));
  }

  sdlRenderer = std::shared_ptr<SDL_Renderer>(
      SDL_CreateRenderer(
          sdlWindow.get(),
          -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE),
      [](SDL_Renderer *rendererPtr)
      {
        SDL_DestroyRenderer(rendererPtr);
      });
  if (!sdlRenderer.get())
  {
    throw std::runtime_error("Unable to create SDL renderer:" + std::string(SDL_GetError()));
  }

  running = true;
  lastTime = static_cast<float>(SDL_GetTicks() * 0.001f);
}

bool Window::isOpen() const
{
  return running;
}

void Window::close()
{
  running = false;
}

void Window::processEvents()
{
  while (SDL_PollEvent(&sdlEvent))
  {
    switch (sdlEvent.type)
    {
    case SDL_EventType::SDL_QUIT:
    {
      running = false;
    }
    break;
    case SDL_EventType::SDL_WINDOWEVENT:
    {
      switch (sdlEvent.window.type)
      {
      case SDL_WINDOWEVENT_FOCUS_GAINED:
      {
        focused = true;
      }
      break;
      case SDL_WINDOWEVENT_FOCUS_LOST:
      {
        focused = false;
      }
      break;
      }
    }
    break;
    case SDL_EventType::SDL_KEYDOWN:
    {
      for (auto &mapping : keymap)
      {
        if (mapping.second == sdlEvent.key.keysym.scancode)
        {
          keysdown.emplace(mapping.first);
        }
      }
    }
    break;
    case SDL_EventType::SDL_KEYUP:
    {
      for (auto &mapping : keymap)
      {
        if (mapping.second == sdlEvent.key.keysym.scancode)
        {
          keysdown.erase(mapping.first);
        }
      }
    }
    break;
    default:
      break;
    }
  }
}

bool Window::isKeyPressed(const std::string &keyId)
{
  return keysdown.count(keyId) != 0;
}

float Window::resetClock()
{
  float currentTime = static_cast<float>(SDL_GetTicks() * 0.001f);
  float deltaTime = currentTime - lastTime;
  lastTime = currentTime;
  return deltaTime;
}

void Window::prepareRender()
{
  SDL_SetRenderDrawColor(sdlRenderer.get(), 0, 0, 0, 255);
  SDL_RenderClear(sdlRenderer.get());
}

void Window::presentRender()
{
  SDL_RenderPresent(sdlRenderer.get());
}

const std::shared_ptr<SDL_Renderer> &Window::getRenderer() const
{
  return sdlRenderer;
}

int Window::getRandomInRangeInt(int lowInclusive, int highInclusive)
{
  std::uniform_int_distribution<int> dist(lowInclusive, highInclusive);
  return dist(rng);
}

double Window::getRandomInRangeDouble(double lowInclusive, double highInclusive)
{
  std::uniform_real_distribution<double> dist(lowInclusive, highInclusive);
  return dist(rng);
}
