#pragma once
#include "types.h"
#include <cstring>
#include <vector>
#include <sstream>
#include <iostream>
#include <SDL3/SDL.h>

struct FaceValue
{
  int vertexNum;
  int texcoord;
  int normal;
};

class Mesh
{
public:
  std::vector<triangle> tris;

  SDL_Texture *texture;

  void CreateCube();

  void LoadFromObj(std::string fileName);

  FaceValue ParseFace(std::string word, char delimiter);

  void LoadTexture(SDL_Renderer *renderer, const std::string &filePath);
};
