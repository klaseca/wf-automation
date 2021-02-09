#pragma once
#include <functional>
#include <iostream>

class Timer {
  bool isWorkState = false;

public:
  void setTimeout(std::function<void()> fn, int delay);
  void setInterval(std::function<void()> fn, int interval);
  void stop();
  bool isWork();
};
