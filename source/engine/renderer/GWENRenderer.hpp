#ifndef GWEN_RENDERERS_HPP
#define GWEN_RENDERERS_HPP

#include <memory>

#include <Gwen/Gwen.h>
#include <Gwen/BaseRender.h>

namespace glPortal {

class VBO;

class GWENRenderer : public Gwen::Renderer::Base {
public:
  struct Vertex {
    float x, y;
    float u, v;
    unsigned char r, g, b, a;
  };

  GWENRenderer();
  ~GWENRenderer();

  void Init() final override;

  void Begin() final override;
  void End() final override;

  void SetDrawColor(Gwen::Color color) final override;
  void DrawFilledRect(Gwen::Rect rect) final override;

  void StartClip() final override;
  void EndClip() final override;

  void DrawTexturedRect(Gwen::Texture *tex, Gwen::Rect rect, float u1 = 0.0f,
    float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f) final override;
  void LoadTexture(Gwen::Texture *tex) final override;
  void FreeTexture(Gwen::Texture *tex) final override;
  Gwen::Color PixelColour(Gwen::Texture *tex, unsigned int x, unsigned int y,
    const Gwen::Color &col_default) final override;

protected:

  static const int MaxVerts = 1024;

  void Flush();
  void AddVert(int x, int y, float u = 0.0f, float v = 0.0f);

  std::unique_ptr<VBO> vbo;

  Gwen::Color color;
  int vertNum;
  Vertex vertices[MaxVerts];
};

} /* namespace glPortal */

#endif
