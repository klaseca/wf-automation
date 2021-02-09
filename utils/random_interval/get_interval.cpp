#include "get_interval.hpp"
#include <random>

int getRandomInterval(const int min, const int max) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<> uid(min, max);

  return uid(mt);
}