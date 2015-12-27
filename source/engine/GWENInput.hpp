#ifndef GWENINPUT_HPP
#define GWENINPUT_HPP

#include <Gwen/InputHandler.h>
#include <Gwen/Gwen.h>
#include <Gwen/Controls/Canvas.h>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>

namespace glPortal {

class GWENInput {
public:
  GWENInput() :
    canvas(nullptr) {
  }

  void init(Gwen::Controls::Canvas *c) {
    canvas = c;
  }

  unsigned char translateKeyCode(int code) {
    switch (code) {
    case SDLK_BACKSPACE:
      return Gwen::Key::Backspace;

    case SDLK_RETURN:
      return Gwen::Key::Return;

    case SDLK_ESCAPE:
      return Gwen::Key::Escape;

    case SDLK_TAB:
      return Gwen::Key::Tab;

    case SDLK_SPACE:
      return Gwen::Key::Space;

    case SDLK_UP:
      return Gwen::Key::Up;

    case SDLK_DOWN:
      return Gwen::Key::Down;

    case SDLK_LEFT:
      return Gwen::Key::Left;

    case SDLK_RIGHT:
      return Gwen::Key::Right;

    case SDLK_HOME:
      return Gwen::Key::Home;

    case SDLK_END:
      return Gwen::Key::End;

    case SDLK_DELETE:
      return Gwen::Key::Delete;

    case SDLK_LCTRL:
    case SDLK_RCTRL:
      return Gwen::Key::Control;

    case SDLK_LALT:
    case SDLK_RALT:
      return Gwen::Key::Alt;

    case SDLK_LSHIFT:
    case SDLK_RSHIFT:
      return Gwen::Key::Shift;

    default:
      break;
    }
    return Gwen::Key::Invalid;
  }

  bool processEvent(const SDL_Event &evt) {
    if (not canvas) {
      return false;
    }

    switch (evt.type) {
    case SDL_MOUSEMOTION:
      return canvas->InputMouseMoved(evt.motion.x, evt.motion.y, evt.motion.xrel, evt.motion.yrel);

    case SDL_MOUSEWHEEL:
      return canvas->InputMouseWheel(evt.wheel.y);

    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP: {
      int button;
      switch (evt.button.button) {
      case SDL_BUTTON_LEFT:
        button = 0;
        break;
      case SDL_BUTTON_RIGHT:
        button = 1;
        break;
      case SDL_BUTTON_MIDDLE:
        button = 2;
        break;
      }
      return canvas->InputMouseButton(button, evt.type == SDL_MOUSEBUTTONDOWN);
    }

    /*case ALLEGRO_EVENT_KEY_CHAR:
      {
        return canvas->InputCharacter(event.keyboard.unichar);
      }*/

    case SDL_KEYDOWN:
    case SDL_KEYUP: {
      bool down = (evt.type == SDL_KEYDOWN);

      if (evt.key.keysym.sym and down and
          evt.key.keysym.sym >= ' ' and evt.key.keysym.sym <= '~') {
        return canvas->InputCharacter(evt.key.keysym.sym);
      }

      unsigned char key = translateKeyCode(evt.key.keysym.sym);
      return canvas->InputKey(key, down);
    }
    }

    return false;
  }

protected:
  Gwen::Controls::Canvas *canvas;
};

} /* namespace glPortal */

#endif /* GWENINPUT_HPP */
