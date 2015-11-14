#include "AnsiConsoleLogger.hpp"

#include <algorithm>
#include <iostream>

namespace glPortal {

static const struct LogLevelOutputInfo {
  char letter;
  const char *colorCode;
  const char *bgColorCode;
} outInfo[] = {
  /* This array must have the same order as the LogLevel enum */
  {'V', "238", "235"},
  {'D', "2", "22"},
  {'I', "20", "17"},
  {'W', "220", "94"},
  {'E', "160", "52"},
  {'F', "92", "53"}
};

const char* AnsiConsoleLogger::getName() const {
  return "ANSI/vt100 logger";
}

int getBackgroundColor(const std::string &tag) {
  if (tag.empty()) {
    return 233; // Dark gray
  }
  uint8_t bg = 0xAA;
  for (int i = 0; i < tag.size(); ++i) {
    bg ^= (uint8_t)tag[i] ^ i;
  }
  return bg%(231-15)+16;
}

bool needBlackText(int bgcol) {
  // Gets the color position in the 6x6x6 color cube
  // http://www.pixelbeat.org/docs/terminal_colours/
  const int x = ((bgcol-16)%36)%6, y = ((bgcol-16)%36)/6, z = (bgcol-16)/36;
  // Bottom and right parts of the cube are the brightest. Higher you go, brightest also.
  return x > 3 or y > 3 or (z >= 3 and (x > 2 or y > 2));
}

void AnsiConsoleLogger::log(const std::string &message, LogLevel lvl, const std::string &tag) {
  const LogLevelOutputInfo &oi = outInfo[(int)lvl];
  std::cout << "\x1B[97m\x1B[48;5;" << oi.colorCode << 'm' << oi.letter;
  int bgcol = getBackgroundColor(tag);
  std::cout << "\x1B[48;5;" << bgcol << 'm';
  if (needBlackText(bgcol)) {
    std::cout << "\x1B[30m";
  }
  std::cout << std::string(16-std::min(16, (int)tag.size()), ' ') << tag.substr(0, 16);
  if (enableBackground) {
    std::cout << "\x1B[97m\x1B[48;5;" << oi.bgColorCode << "m ";
  } else {
    std::cout << "\x1B[0m ";
  }
  std::cout << message << "\x1B[0m" << std::endl;
}

} /* namespace glPortal */
