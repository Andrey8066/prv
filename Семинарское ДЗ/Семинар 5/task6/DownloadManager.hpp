#pragma once

#include "FileDownload.hpp"
#include "SimpleSemaphore.hpp"

#include <atomic>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <thread>

class DownloadManager {
private:
  std::queue<FileChunk> download_queue;

  SimpleSemaphore active_downloads;

  SimpleSemaphore chunk_downloads;

  std::mutex queue_mutex;

  std::mutex file_mutex;

  std::mutex cout_mutex;

  std::atomic<int> completed_files{0};

  std::atomic<int> completed_chunks{0};

  std::map<int, FileDownload *> files;

public:
  DownloadManager(int max_active_files, int max_chunks)
      : active_downloads(max_active_files), chunk_downloads(max_chunks) {}

  void add_file(FileDownload &file) {

    active_downloads.acquire();

    {
      std::lock_guard<std::mutex> lock(file_mutex);

      files[file.file_id] = &file;
    }

    {
      std::lock_guard<std::mutex> lock(queue_mutex);

      for (auto &chunk : file.chunks) {

        download_queue.push(chunk);
      }
    }

    {
      std::lock_guard<std::mutex> out(cout_mutex);

      std::cout << "[ADD FILE] file = " << file.file_id
                << " | chunks = " << file.chunks.size() << std::endl;
    }
  }

  inline void process_chunk(FileChunk chunk) {

    {
      std::lock_guard<std::mutex> out(cout_mutex);

      std::cout << "[DOWNLOAD START] Thread " << std::this_thread::get_id()
                << " | file = " << chunk.file_id
                << " | chunk = " << chunk.chunk_id << std::endl;
    }

    chunk.download();

    completed_chunks++;

    {
      std::lock_guard<std::mutex> lock(file_mutex);

      auto file = files[chunk.file_id];

      file->mark_chunk_downloaded();

      if (file->is_complete()) {

        completed_files++;

        active_downloads.release();

        std::lock_guard<std::mutex> out(cout_mutex);

        std::cout << "[FILE COMPLETE] file = " << file->file_id << std::endl;
      }
    }

    {
      std::lock_guard<std::mutex> out(cout_mutex);

      std::cout << "[DOWNLOAD END] Thread " << std::this_thread::get_id()
                << " | file = " << chunk.file_id
                << " | chunk = " << chunk.chunk_id << std::endl;
    }
  }

  void download_worker() {

    while (true) {

      FileChunk chunk;

      {
        std::lock_guard<std::mutex> lock(queue_mutex);

        if (download_queue.empty())
          return;

        chunk = download_queue.front();

        download_queue.pop();
      }

      chunk_downloads.acquire();

      process_chunk(chunk);

      chunk_downloads.release();

      std::this_thread::yield();
    }
  }

  int get_completed_files() const { return completed_files.load(); }

  int get_completed_chunks() const { return completed_chunks.load(); }
};