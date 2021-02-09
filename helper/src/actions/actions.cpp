#include "actions.hpp"
#include "./../state/state.hpp"
#include <input_emulation/input_emulation.hpp>
#include <random_interval/get_interval.hpp>
#include <timer/timer.hpp>

void shortAction(Timer &timer, const State &state, InputEmulation &im) {
  int rand = getRandomInterval(1500, 2000);

  timer.setTimeout(
    [&] {
      if (state.isCheckCursorPos && state.isRunApp) {
        im.checkCursorPos(state.cursorPosition);
      }
      if (state.isRunApp) {
        im.keyPress(0x34);
      }
      if (state.isRunApp) {
        im.leftClick();
      }
      if (state.isRunApp) {
        shortAction(timer, state, im);
      }
    },
    rand);
}

void longAction(Timer &timer, const State &state, InputEmulation &im) {
  int rand = getRandomInterval(4500, 6500);

  timer.setTimeout(
    [&] {
      if (state.isRunApp) {
        im.mouseMove();
      }
      if (state.isRunApp) {
        im.randMove();
      }
      if (state.isSpace && state.isRunApp) {
        im.keyPress(VK_SPACE);
      }
      if (state.isRunApp) {
        longAction(timer, state, im);
      }
    },
    rand);
}