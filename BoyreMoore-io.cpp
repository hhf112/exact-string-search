#include <fstream>
#include <functional>
#include <iostream>
#include <string>

#include "BoyreMoore.h"
void BoyreMoore::startStream(int chnk, const std::string &p) {
  path = p;
  chunkSize = chnk;
  file = std::fstream(path, std::ios::in);

  if (!file) {
    std::cerr << "unable to open file\n";
    exit(1);
  }
  buffer.resize(chunkSize, 'a');
}

bool BoyreMoore::readChunk(char *put) {
  if (!put)
    file.read(buffer.data(), chunkSize);
  else
    file.read(put, chunkSize);
  return !file.eof();
}

void BoyreMoore::forStream(std::function<void(const std::string &)> action) {
  while (readChunk()) {
    action(buffer);
  }
}
