#include <cstddef>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

#include "../BoyreMoore.h"

#define MAX_CHUNK_LIMIT 128 *  1048576

void BoyreMoore::startStream(int chnk, const std::string &p) {
  path = p;
  chunkSize = std::min(chnk, MAX_CHUNK_LIMIT);
  file = std::fstream(path, std::ios::in);

  if (!file) {
    std::cerr << "unable to open file\n";
    exit(1);
  }
  buffer.resize(chunkSize, 'a');
}

void BoyreMoore::forStream(
    size_t patternlen, const std::function<void(const std::string &)> &action) {
  if (patternlen == 0)
    return;
  buffer.resize(chunkSize + patternlen - 1, 'a');

  if (!file.read(buffer.data(), chunkSize + patternlen - 1)) {
    std::cerr << "failed to read from file\n";
    return;
  }

  while (file.gcount()) {
    action(buffer);
    std::memcpy(buffer.data(), buffer.data() + chunkSize, patternlen - 1);
    file.read(buffer.data() + patternlen - 1, chunkSize);
  }
}
