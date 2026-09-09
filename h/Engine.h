#pragma once

class Renderer;
class Camera;
class SDL_Window;
class SDL_Renderer;

class Engine
{
private:
  Renderer* Erenderer;
  Camera* Ecamera;

public:
  SDL_Window* window;
  SDL_Renderer* sdlRenderer;

  float frame = 0;

#define HEIGHT 700
#define WIDTH 700

  float GetAspectRatio(SDL_Window* window);

  bool Initialize();

  void run();

  void MoveCamera(const bool keyStetes[], const float speed);

  void DirectCamera(const bool keyStates[], const float speed);

  void clean();
};