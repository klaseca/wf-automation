#include "text.hpp"
#include <fstream>
#include <input_emulation/input_emulation.hpp>
#include <numeric>
#include <random_interval/get_interval.hpp>
#include <sstream>
#include <timer/timer.hpp>
#include <windows.h>

InputEmulation im{};
Timer t{};

void writeText(const std::string &str) {
  wchar_t wc;
  INPUT pInput;

  for (size_t i = 0; i < str.size(); i++) {
    MultiByteToWideChar(CP_UTF8, 0, &str.at(i), 1, &wc, 1);
    pInput.type = INPUT_KEYBOARD;
    pInput.ki.wVk = 0;
    pInput.ki.time = 10;
    pInput.ki.wScan = wc;
    pInput.ki.dwFlags = KEYEVENTF_UNICODE;
    SendInput(1, &pInput, sizeof(INPUT));

    pInput.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
    SendInput(1, &pInput, sizeof(INPUT));
    Sleep(20);
  }
}

FileState getFileText(const std::string &path) {
  std::ifstream file(path);
  if (file) {
    std::ostringstream ss;
    ss << file.rdbuf();
    return FileState{ss.str(), false};
  } else {
    return FileState{"", true};
  }
}

std::vector<std::string> split(const std::string &str) {
  using namespace std;

  vector<string> v{};
  string delimiter{"\n"};
  size_t start{0U};
  size_t end = str.find(delimiter);

  while (end != string::npos) {
    string s = str.substr(start, end - start);
    v.push_back(s);
    start = end + delimiter.length();
    end = str.find(delimiter, start);
  }

  v.push_back(str.substr(start, end));

  return v;
}

std::vector<int> splitSettings(const std::string &str) {
  using namespace std;

  vector<int> v{};
  string delimiter{" "};
  size_t start{0U};
  size_t end = str.find(delimiter);

  while (end != string::npos) {
    string s = str.substr(start, end - start);
    v.push_back(stoi(s));
    start = end + delimiter.length();
    end = str.find(delimiter, start);
  }

  v.push_back(stoi(str.substr(start, end)));

  return v;
}

void startWrite(const State &state, const std::vector<std::string> &v,
  std::list<int> &l, std::vector<int> &vi) {
  using namespace std;

  wstring a = to_wstring(v.size());

  MessageBoxW(NULL, a.c_str(), state.name.c_str(), MB_OK);

  if (l.empty()) {
    l.resize(v.size());
    iota(begin(l), end(l), 0);
  }

  int randTime = getRandomInterval(vi.at(0), vi.at(1));

  t.setTimeout(
    [=]() mutable {
      int rand = getRandomInterval(0, l.size() - 1);
      auto iter = next(l.begin(), rand);
      if (state.isWriting)
        writeText(v.at(*iter));
      if (state.isWriting)
        im.keyPress(VK_RETURN);
      if (state.isWriting)
        l.erase(iter);
      if (state.isWriting)
        startWrite(state, v, l, vi);
    },
    randTime);
}
