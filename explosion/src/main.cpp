#include "explosion/explosion.hpp"
#include "resources/resource.hpp"
#include "state/state.hpp"
#include <input_emulation/input_emulation.hpp>
#include <strsafe.h>
#include <timer/timer.hpp>
#include <windows.h>

using namespace std;

NOTIFYICONDATA pnid;
Timer t{};
InputEmulation im{};
State state{L"Explosion", false};

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

    if (keyCode == VK_F11) {
      if (!state.isExplosion) {
        state.isExplosion = true;
        startExplosion(t, state, im);
      } else {
        state.isExplosion = false;
      }
    }
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
  HWND hMainWnd;
  MSG uMsg;

  WNDCLASSEX wc;

  wc.cbSize = sizeof(WNDCLASSEX);
  wc.hbrBackground = (HBRUSH)GetStockObject(0);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(APP_ICON));
  wc.hIconSm = wc.hIcon;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = &WindowProc;
  wc.lpszClassName = state.name.c_str();

  if (!RegisterClassEx(&wc)) {
    return EXIT_FAILURE;
  }

  hMainWnd = CreateWindow(state.name.c_str(), state.name.c_str(),
    WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, 0, 0, hInstance, 0);

  if (!hMainWnd) {
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
