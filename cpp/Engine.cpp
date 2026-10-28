#include "Engine.h"
#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "VecMath.h"
#include "Ilumination.h"
#include <SDL3/SDL.h>
#include <string>

#include "Debug.h"
#include "Profiler.h"

float Engine::GetAspectRatio(SDL_Window *window)
{
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  return static_cast<float>(w) / static_cast<float>(h);
}

bool Engine::Initialize()
{
  (SDL_INIT_VIDEO);
  window = SDL_CreateWindow("window", WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  sdlRenderer = SDL_CreateRenderer(window, nullptr);

  Erenderer = new Renderer(sdlRenderer); // TODO: use the modern version (unique_ptr)
  Ecamera = new Camera();

  if (!window)
  {
    return false;
  }
  return true;
}

void Engine::run()
{
  // Load Objects
  Mesh cube;
  // cube.CreateCube();
  cube.LoadFromObj("../Obj/utah_teapot.obj");
  // cube.LoadTexture(sdlRenderer, "../texture/car_jeep_ren.jpg");

  const float DEG2RAD = M_PI / 180.0f;
  Uint64 lastCounter = SDL_GetPerformanceCounter();

  bool running = true;
  while (running)
  {
    // ScopeTimer timer("Frame time");
    // Get elapsed time since last frame
    Uint64 currentCounter = SDL_GetPerformanceCounter();
    float elapsedTime = (float)(currentCounter - lastCounter) / SDL_GetPerformanceFrequency();
    lastCounter = currentCounter;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_EVENT_QUIT)
      {
        running = false;
      }
    }

    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
    SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);

    // Set universal speed
    float speed = elapsedTime * 10;

    // Camera movement
    const bool *keyStates = SDL_GetKeyboardState(NULL);
    this->MoveCamera(keyStates, speed);
    this->DirectCamera(keyStates, speed * 5);

    Ecamera->UpdateViewMatrix();

    static vec3d continuousRot;
    continuousRot.x += 0 * elapsedTime * DEG2RAD;
    continuousRot.y += 30 * elapsedTime * DEG2RAD;
    continuousRot.z += 0 * elapsedTime * DEG2RAD;

    std::vector<Ilumination> iluminations;
    Ilumination ilumination1, ilumination2;
    ilumination1.SetLightDir({-0.0f, -0.0f, 1.0f});
    ilumination2.SetLightDir({0.0f, 0.0f, -0.5f});
    iluminations.push_back(ilumination1);
    iluminations.push_back(ilumination2);

    Erenderer->DrawMesh(cube, continuousRot, *Ecamera, {0, 0, 20}, iluminations);
    // Debug::DrawGrid3D(*Erenderer, *Ecamera);

    SDL_RenderPresent(sdlRenderer);
  }
  clean();
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Engine::MoveCamera(const bool keyStates[], const float speed)
{
  if (keyStates[SDL_SCANCODE_D])
  {
    Ecamera->MoveCameraLocal({speed, 0, 0});
  }

  if (keyStates[SDL_SCANCODE_A])
  {
    Ecamera->MoveCameraLocal({-speed, 0, 0});
  }

  if (keyStates[SDL_SCANCODE_W])
  {
    Ecamera->MoveCameraLocal({0, 0, speed});
  }

  if (keyStates[SDL_SCANCODE_S])
  {
    Ecamera->MoveCameraLocal({0, 0, -speed});
  }

  if (keyStates[SDL_SCANCODE_C])
  {
    Ecamera->MoveCameraLocal({0, speed, 0});
  }

  if (keyStates[SDL_SCANCODE_X])
  {
    Ecamera->MoveCameraLocal({0, -speed, 0});
  }
}

void Engine::DirectCamera(const bool keyStates[], const float speed)
{
  if (keyStates[SDL_SCANCODE_J])
  {
    Ecamera->ChangeDiraction({0, speed, 0});
  }

  if (keyStates[SDL_SCANCODE_L])
  {
    Ecamera->ChangeDiraction({0, -speed, 0});
  }

  if (keyStates[SDL_SCANCODE_K])
  {
    Ecamera->ChangeDiraction({speed, 0, 0});
  }

  if (keyStates[SDL_SCANCODE_I])
  {
    Ecamera->ChangeDiraction({-speed, 0, 0});
  }
}

void Engine::clean()
{
  delete Erenderer;
  delete Ecamera;
}
