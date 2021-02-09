#pragma once
#include <windows.h>
#include <string>

struct Key {
  char runApp;
  char getCursorPos;
  char checkCursorPos;
  char space;
};

struct State {
  std::wstring name;
  bool isRunApp;
  bool isTimer;
  bool isCheckCursorPos;
  bool isSpace;
  POINT cursorPosition;
  Key key;
};
