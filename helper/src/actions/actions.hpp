#pragma once
#include "./../state/state.hpp"
#include <input_emulation/input_emulation.hpp>
#include <timer/timer.hpp>

void shortAction(Timer &timer, const State &state, InputEmulation &im);

void longAction(Timer &timer, const State &state, InputEmulation &im);