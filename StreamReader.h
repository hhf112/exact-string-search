#pragma once
#include <fstream>
#include <functional>
#include <memory>
#include <string>
#include <vector>

class StreamReader {
  const int chunkSize; //bytes
  std::fstream file;
  const std::string path;
  std::vector<char> buffer;

public:
  StreamReader() = delete;
  StreamReader(int chunkSize, const std::string &path)
      : file(path, std::ios::out), path{path}, chunkSize{chunkSize} {
    buffer.resize(chunkSize);
  }

  inline bool chunk() {
    file.read(buffer.data(), chunkSize);
    return file.eof();
  }

  inline void Stream(std::function<void(std::vector<char>)> action) {
    while (chunk()) {
      action(buffer);
    }
  }

  inline std::string getPath() { return path; }
};
