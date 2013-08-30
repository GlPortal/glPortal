#include "Exception.hpp"

Exception::Exception(const char* message){
  this->message = message; 
}

void Exception::print() {
  printf(this->message);
}

const char* Exception::getMessage() {
  return this->message;
}
