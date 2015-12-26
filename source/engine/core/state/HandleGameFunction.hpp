#ifndef HANDLE_GAME_FUNCTION_HPP
#define HANDLE_GAME_FUNCTION_HPP

namespace glPortal {

class Game;
typedef std::function<void (Game &game)> HandleGameFunction;

} /* namespace glPortal */

#endif /* HANDLE_GAME_FUNCTION_HPP */
