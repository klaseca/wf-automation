#pragma once
#include "./../state/state.hpp"
#include <input_emulation/input_emulation.hpp>
#include <timer/timer.hpp>

void explosionClick();

void startExplosion(Timer &t, const State &state, InputEmulation &im);
