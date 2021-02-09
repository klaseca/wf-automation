#include "input_emulation.hpp"
#include <iostream>
#include <random>

using namespace std;

void InputEmulation::mouseMoveAbs(int x, int y) {
  msabs.type = INPUT_MOUSE;
  msabs.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
  msabs.mi.dx = 65535 / GetSystemMetrics(SM_CXSCREEN) * x;
  msabs.mi.dy = 65535 / GetSystemMetrics(SM_CYSCREEN) * y;
  SendInput(1, &msabs, sizeof(INPUT));
}

void InputEmulation::keyPress(char keyCode, int sleepTime) {
  key.type = INPUT_KEYBOARD;
  key.ki.wVk = keyCode;
  key.ki.wScan = MapVirtualKey(key.ki.wVk, 0);
  key.ki.dwFlags = 0;
  SendInput(1, &key, sizeof(INPUT));

  Sleep(sleepTime);

  key.ki.dwFlags = KEYEVENTF_KEYUP;
  SendInput(1, &key, sizeof(INPUT));

  Sleep(64);
}

void InputEmulation::mouseMove() {
  random_device rd;
  mt19937 mt(rd());
  uniform_int_distribution<> uid(-50, 50);

  int x = uid(mt);
  int y = uid(mt);

  ms.type = INPUT_MOUSE;
  ms.mi.dwFlags = MOUSEEVENTF_MOVE;
  ms.mi.dx = +x;
  ms.mi.dy = +y;
  SendInput(1, &ms, sizeof(INPUT));

  Sleep(100);

  ms.mi.dx = -x;
  ms.mi.dy = -y;
  SendInput(1, &ms, sizeof(INPUT));
}

void InputEmulation::leftClick() {
  lc.type = INPUT_MOUSE;
  lc.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  SendInput(1, &lc, sizeof(INPUT));

  Sleep(64);

  lc.mi.dwFlags = MOUSEEVENTF_LEFTUP;
  SendInput(1, &lc, sizeof(INPUT));
}

void InputEmulation::randMove() {
  random_device rd;
  mt19937 mt(rd());
  uniform_int_distribution<> uid(1, 4);
  uniform_int_distribution<> uidSt(70, 200);

  int rand = uid(mt);
  int sleepTime = uidSt(mt);

  switch (rand) {
  case 1:
    keyPress('W', sleepTime);
    keyPress('S', int(sleepTime * 1.2));
    break;
  case 2:
    keyPress('S', sleepTime);
    keyPress('W', int(sleepTime * 0.8));
    break;
  case 3:
    keyPress('A', sleepTime);
    keyPress('D', sleepTime);
    break;
  case 4:
    keyPress('D', sleepTime);
    keyPress('A', sleepTime);
    break;
  default:
    break;
  }
}

void InputEmulation::checkCursorPos(const POINT &cursorPos) {
  POINT p{};
  GetCursorPos(&p);

  if (p.x != cursorPos.x || p.y != cursorPos.y) {
    SetCursorPos(cursorPos.x, cursorPos.y);
  }
}
