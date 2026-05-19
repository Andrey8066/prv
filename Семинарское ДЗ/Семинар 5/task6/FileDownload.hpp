#pragma once

#include "FileChunk.hpp"

#include <atomic>
#include <vector>

class FileDownload {
public:
  int file_id;

  std::vector<FileChunk> chunks;

  std::atomic<int> downloaded_chunks{0};

  FileDownload() = default;

  FileDownload(int id, int chunk_count) : file_id(id) {

    for (int i = 0; i < chunk_count; i++) {

      chunks.emplace_back(i, file_id, 1024);
    }
  }

  bool is_complete() const { return downloaded_chunks.load() == chunks.size(); }

  void mark_chunk_downloaded() { downloaded_chunks++; }
};