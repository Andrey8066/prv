#pragma once

#include <chrono>
#include <iostream>
#include <thread>

class FileChunk {
public:
  int chunk_id;

  int file_id;

  size_t size;

  FileChunk() = default;

  FileChunk(int chunkId, int fileId, size_t size)
      : chunk_id(chunkId), file_id(fileId), size(size) {}

  void download() {

    std::this_thread::sleep_for(std::chrono::milliseconds(200 + rand() % 1000));
  }
};