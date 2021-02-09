#pragma once
#include "./../state/state.hpp"
#include <list>
#include <string>
#include <vector>

void writeText(const std::string &str);

FileState getFileText(const std::string &path);

std::vector<std::string> split(const std::string &str);

std::vector<int> splitSettings(const std::string &str);

void startWrite(const State &state, const std::vector<std::string> &v,
  std::list<int> &l, std::vector<int> &vi);
