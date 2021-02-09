#pragma once
#include <string>

struct FileState {
  std::string content;
  bool isNotFile;
};

struct State {
  std::wstring name;
  bool isWriting;
};
