#include "timer.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

using namespace std;

void Timer::setTimeout(function<void()> fn, int delay) {
  thread{[=]() {
    this_thread::sleep_for(chrono::milliseconds(delay));
    fn();
  }}.detach();
}

void Timer::setInterval(function<void()> fn, int interval) {
  isWorkState = true;
  thread{[=]() {
    while (isWorkState) {
      if (!isWorkState)
        return;
      this_thread::sleep_for(chrono::milliseconds(interval));
      if (!isWorkState)
        return;
      fn();
    }
  }}.detach();
}

void Timer::stop() { isWorkState = false; }

bool Timer::isWork() { return isWorkState; }
