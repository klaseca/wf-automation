#pragma once
#include <windows.h>

class InputEmulation {
private:
  INPUT key;
  INPUT lc;
  INPUT ms;
  INPUT msabs;

public:
  void mouseMoveAbs(int x, int y);
  void keyPress(char keyCode, int sleepTime = 64);
  void mouseMove();
  void leftClick();
  void randMove();
  void checkCursorPos(const POINT &cursorPos);
};
