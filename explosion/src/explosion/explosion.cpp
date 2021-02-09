#include "explosion.hpp"
#include "./../state/state.hpp"
#include <chrono>
#include <input_emulation/input_emulation.hpp>
#include <random_interval/get_interval.hpp>
#include <thread>
#include <timer/timer.hpp>
#include <windows.h>

void explosionClick() {
  INPUT lc{};
  lc.type = INPUT_MOUSE;
  lc.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  SendInput(1, &lc, sizeof(INPUT));

  Sleep(4000);

  lc.mi.dwFlags = MOUSEEVENTF_LEFTUP;
  SendInput(1, &lc, sizeof(INPUT));
}

void startExplosion(Timer &t, const State &state, InputEmulation &im) {
  using namespace std::chrono_literals;

  int randTime = getRandomInterval(500, 1500);

  t.setTimeout(
    [&]() {
      if (state.isExplosion)
        im.keyPress('5');
      if (state.isExplosion)
        std::this_thread::sleep_for(1000ms);
      if (state.isExplosion)
        explosionClick();
      if (state.isExplosion)
        std::this_thread::sleep_for(3000ms);
      if (state.isExplosion)
        im.keyPress(VK_SPACE);
      if (state.isExplosion)
        std::this_thread::sleep_for(500ms);
      if (state.isExplosion)
        startExplosion(t, state, im);
    },
    randTime);
}
