#include "Mesh.h"
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void
Mesh::CreateCube()
{
  tris = {

    // SOUTH face
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } } },
    { { { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } } },

    // EAST face
    { { { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } } },
    { { { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f } } },

    // NORTH face
    { { { 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f } } },
    { { { 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } } },

    // WEST face
    { { { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } } },
    { { { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },

    // TOP face
    { { { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } } },
    { { { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f } } },

    // BOTTOM face
    { { { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } } },
    { { { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } } }
  };
};

void Mesh::LoadFromObj(std::string fileName)
{
  std::ifstream obj(fileName);

  if (!obj)
  {
    std::cerr << "Error code: " << strerror(errno);
    return;
  }

  std::vector<vec3d> verts, normals;
  std::vector<vec2d> textures;
  std::string line;

  while (std::getline(obj, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::stringstream s(line);
    std::string type;

    s >> type;

    if (type == "v")
    {
      float x, y, z;
      if (s >> x >> y >> z)
      {
        verts.push_back({x, y, z});
      }
    }

    else if (type == "vt")
    {
      float u, v;
      if (s >> u >> v)
      {
        textures.push_back({u, v});
      }
    }

    else if (type == "vn")
    {
      float x, y, z;
      if (s >> x >> y >> z)
      {
        normals.push_back({x, y, z});
      }
    }

    else if (type == "f")
    {
      std::string a, b, c, d;
      FaceValue pointIndexes[4];

      if (s >> a >> b >> c)
      {
        pointIndexes[0] = ParseFace(a, '/');
        pointIndexes[1] = ParseFace(b, '/');
        pointIndexes[2] = ParseFace(c, '/');

        int tricount = 1;
        if (s >> d)
        {
          pointIndexes[3] = ParseFace(d, '/');
          tricount++;
        }

        for (int i = 0; i < tricount; i++)
        {

          this->tris.push_back( // TODO: add boundery check

              {{verts[pointIndexes[0].vertexIndex - 1],
                verts[pointIndexes[i + 1].vertexIndex - 1],
                verts[pointIndexes[i + 2].vertexIndex - 1]},

               {textures[pointIndexes[0].texcoordIndex - 1],
                textures[pointIndexes[i + 1].texcoordIndex - 1],
                textures[pointIndexes[i + 2].texcoordIndex - 1]},

               {normals[pointIndexes[0].normalIndex - 1],
                normals[pointIndexes[i + 1].normalIndex - 1],
                normals[pointIndexes[i + 2].normalIndex - 1]}});
        }
      }
    }
  }
}

FaceValue Mesh::ParseFace(std::string word, char delimiter)
{
  FaceValue faceValue;
  std::string token;
  std::stringstream ss(word);

  if (std::getline(ss, token, delimiter) && !token.empty())
    faceValue.vertexIndex = std::stoi(token);

  if (std::getline(ss, token, delimiter) && !token.empty())
    faceValue.texcoordIndex = std::stoi(token);

  if (std::getline(ss, token, delimiter) && !token.empty())
    faceValue.normalIndex = std::stoi(token);

  return faceValue;
}

void Mesh::LoadTexture(SDL_Renderer *renderer, const std::string &filePath)
{
  int width = 0;
  int height = 0;
  int channels = 0;
  stbi_uc *pixelData = stbi_load(filePath.c_str(), &width, &height, &channels, 4);

  if (!pixelData)
  {
    std::cerr << "Mesh::LoadTexture" << "Error code: " << strerror(errno);
    return;
  }

  // width * 4 -> bytes of data for each row
  SDL_Surface *surface = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGBA32, pixelData, width * 4);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_DestroySurface(surface);
  stbi_image_free(pixelData);

  this->texture = texture; // you should sdl_destroy texture when you're done or memory leak
}
