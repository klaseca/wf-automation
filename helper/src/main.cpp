#include "actions/actions.hpp"
#include "resources/resource.hpp"
#include "state/state.hpp"
#include <input_emulation/input_emulation.hpp>
#include <sstream>
#include <strsafe.h>
#include <timer/timer.hpp>
#include <windows.h>

using namespace std;

NOTIFYICONDATA pnid;

State state{L"Helper", false, false, false, false, {700, 430},
  {VK_F4, VK_F1, VK_F2, VK_F3}};
InputEmulation im{};
Timer shortTimer{};
Timer longTimer{};

void createTray(NOTIFYICONDATA &pnid, HWND &hwnd) {
  pnid.cbSize = sizeof(pnid);
  pnid.hWnd = hwnd;
  pnid.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(APP_ICON));
  pnid.uID = 15;
  pnid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
  pnid.uCallbackMessage = 15666;
  StringCchCopy(pnid.szTip, ARRAYSIZE(pnid.szTip), state.name.c_str());

  Shell_NotifyIcon(NIM_ADD, &pnid);
}

LRESULT WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp) {
  switch (uMsg) {
    /*case WM_CREATE:
      Shell_NotifyIcon(NIM_ADD, &pnid);
      ShowWindow(hWnd, SW_HIDE);
      break;*/
  case WM_CLOSE:
    Shell_NotifyIcon(NIM_ADD, &pnid);
    ShowWindow(hWnd, SW_HIDE);
    break;
  case 15666:
    if (lp == WM_LBUTTONDBLCLK) {
      Shell_NotifyIcon(NIM_DELETE, &pnid);
      PostQuitMessage(0);
    }
    break;
  default:
    return DefWindowProc(hWnd, uMsg, wp, lp);
    break;
  }

  return 0;
}

LRESULT CALLBACK LLKeyProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION && wParam == WM_KEYUP) {
    auto p = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam);
    auto keyCode = static_cast<char>(p->vkCode);

    if (keyCode == state.key.runApp) {
      if (state.isRunApp) {
        state.isRunApp = false;
      } else {
        state.isRunApp = true;
        shortAction(shortTimer, state, im);
        longAction(longTimer, state, im);
      }
    } else if (keyCode == state.key.checkCursorPos) {
      wstring message;

      if (state.isCheckCursorPos) {
        state.isCheckCursorPos = false;
        message = L"Check cursor position disabled";
      } else {
        state.isCheckCursorPos = true;
        message = L"Check cursor position enabled";
      }

      MessageBoxW(nullptr, message.c_str(), state.name.c_str(), MB_OK);
    } else if (keyCode == state.key.space) {
      wstring message;

      if (state.isSpace) {
        state.isSpace = false;
        message = L"Press space disabled";
      } else {
        state.isSpace = true;
        message = L"Press space enabled";
      }

      MessageBoxW(nullptr, L"Press space enabled", state.name.c_str(), MB_OK);
    } else if (keyCode == state.key.getCursorPos) {
      GetCursorPos(&state.cursorPosition);

      wstringstream ss;
      ss << L"New cursor position: x=" << state.cursorPosition.x << L", y="
         << state.cursorPosition.y;

      MessageBoxW(nullptr, ss.str().c_str(), state.name.c_str(), MB_OK);
    }
  }
  return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
  HWND hMainWnd;
  MSG uMsg;

  WNDCLASSEX wc{};

  wc.cbSize = sizeof(WNDCLASSEX);
  wc.hbrBackground = (HBRUSH)GetStockObject(0);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(APP_ICON));
  wc.hInstance = hInstance;
  wc.lpfnWndProc = WindowProc;
  wc.lpszClassName = state.name.c_str();

  if (!RegisterClassEx(&wc)) {
    MessageBoxW(nullptr, L"Failed to initialize WNDCLASSEX", state.name.c_str(),
      MB_OK | MB_ICONERROR);
    return EXIT_FAILURE;
  }

  hMainWnd = CreateWindowEx(0, state.name.c_str(), state.name.c_str(),
    WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

  if (!hMainWnd) {
    MessageBoxW(nullptr, L"Failed to initialize main window",
      state.name.c_str(), MB_OK | MB_ICONERROR);
    return EXIT_FAILURE;
  }

  ShowWindow(hMainWnd, SW_HIDE);
  UpdateWindow(hMainWnd);

  createTray(pnid, hMainWnd);

  HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, LLKeyProc, 0, 0);

  while (GetMessage(&uMsg, nullptr, 0, 0)) {
    TranslateMessage(&uMsg);
    DispatchMessage(&uMsg);
  }

  UnhookWindowsHookEx(hook);

  return uMsg.wParam;
}