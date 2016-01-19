#include "Window.hpp"

#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

#include <epoxy/gl.h>

#include <SDL2/SDL.h>

#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/Button.h>
#include <Gwen/Controls/CheckBox.h>
#include <Gwen/Controls/TextBox.h>
#include <Gwen/Controls/TreeControl.h>
#include <chrono>
#include <thread>
#include <assets/texture/TextureLoader.hpp>

#include <engine/core/diag/Throwables.hpp>
#include <engine/GWENInput.hpp>
#include <engine/renderer/GWENRenderer.hpp>
#include <engine/env/Environment.hpp>
#include <engine/env/System.hpp>

namespace glPortal {

const int Window::DEFAULT_WIDTH = 800;
const int Window::DEFAULT_HEIGHT = 600;
const char* Window::DEFAULT_TITLE = "GlPortal";

Window::Window() :
  width(0),
  height(0),
  window(nullptr) {
}

Window::~Window() = default;

void Window::initEpoxy() {
  const int glver = epoxy_gl_version(), glmaj = glver/10, glmin = glver%10;
  const std::string verstr = std::to_string(glmaj) + '.' + std::to_string(glmin);
  System::Log(Verbose, "Window") << "OpenGL " << verstr;
  if (glver < 30) {
    throw Exception::Error("Window", std::string("OpenGL Version ") + verstr + " is unsupported, "
      "required minimum is 3.0");
  }
}

void Window::create(const char *title) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  Config &config = Environment::getConfig();

  if (config.getAntialiasLevel() > 0) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, config.getAntialiasLevel());
  }
  
  int flags = SDL_WINDOW_OPENGL;
  if (config.isFullscreen()) {
    flags |= SDL_WINDOW_BORDERLESS;
  }

  SDL_DisplayMode dispMode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
  SDL_GetDesktopDisplayMode(0, &dispMode);

  int width = config.getWidth();
  int height = config.getHeight();

  if (width == 0) {
    width = dispMode.w;
  }
  if (height == 0) {
    height = dispMode.h;
  }

  // Explicitly request a Forward-Compatible GL 3.0 Core context
  // i.e. enforce using non-deprecated functions
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height, flags);

  context = SDL_GL_CreateContext(window);

  initEpoxy();

  this->width = width;
  this->height = height;

  glViewport(0, 0, width, height);

  // Allows unbound framerate if vsync is disabled
  SDL_GL_SetSwapInterval(config.hasVsync() ? 1 : 0);

  // Lock cursor in the middle of the screen
  lockMouse();

  gwenRenderer = std::make_unique<GWENRenderer>();
  gwenSkin = std::make_unique<Gwen::Skin::TexturedBase>(gwenRenderer.get());
  gwenSkin->Init((Environment::getDataDir() + "/gui/DefaultSkin.png").c_str());
  gwenCanvas = std::make_unique<Gwen::Controls::Canvas>(gwenSkin.get());
  gwenInput = std::make_unique<GWENInput>();
  gwenRenderer->Init();
  gwenInput->init(gwenCanvas.get());

#if 0 // Testing code
  { using namespace Gwen::Controls;
    WindowControl *win = new WindowControl(gwenCanvas.get());
    win->SetTitle("Texture cache");
    win->SetBounds( 30, 30, 500, 200 );
    TreeControl *tree = new TreeControl(win, "tree");
    tree->SetBounds(0, 0, 200, 186);
    std::thread thr([tree]() {
      while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        Base::List &l = tree->GetChildNodes();
        for (auto it : TextureLoader::getTextureCache()) {
          bool add = true;
          for (Base *b : l) {
            if (((TreeNode*)b)->GetName() == it.first) {
              add = false;
              break;
            }
          }
          if (add) {
            TreeNode *n = tree->AddNode(it.first);
            n->SetName(it.first);
          }
        }
      }
    }); thr.detach();
  }
#endif
}

void Window::setFullscreen() {
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
}

void Window::swapBuffers() {
  SDL_GL_SwapWindow(window);
}

void Window::getSize(int *width, int *height) const {
  SDL_GetWindowSize(window, width, height);
}

void Window::close() {
  SDL_HideWindow(window);

  gwenInput.release();
  gwenCanvas.release();
  gwenSkin.release();
  gwenRenderer.release();

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  window = nullptr;

  SDL_Quit();
}

void Window::lockMouse() {
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Window::unlockMouse() {
  SDL_SetRelativeMouseMode(SDL_FALSE);
}

} /* namespace glPortal */
