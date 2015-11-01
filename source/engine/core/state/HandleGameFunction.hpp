#ifndef HANDLE_GAME_FUNCTION_HPP
#define HANDLE_GAME_FUNCTION__HPP
namespace glPortal {
class Game;
typedef std::function<int (Game &game)> HandleGameFunction;
}  
#endif /* HANDLE_GAME_FUNCTION_HPP */
