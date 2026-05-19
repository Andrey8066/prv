#include "DownloadManager.hpp"

int main() {

  srand(time(nullptr));

  DownloadManager manager(2, // active files
                          3  // chunks simultaneously
  );

  FileDownload file1(1, 5);

  FileDownload file2(2, 4);

  FileDownload file3(3, 6);

  manager.add_file(file1);

  manager.add_file(file2);

  std::thread([&]() { manager.add_file(file3); }).detach();

  std::vector<std::thread> workers;

  for (int i = 0; i < 5; i++) {

    workers.emplace_back([&]() { manager.download_worker(); });

    workers.back().detach();

    std::this_thread::yield();
  }

  std::this_thread::sleep_for(std::chrono::seconds(15));

  std::cout << "\nCompleted files: " << manager.get_completed_files()
            << std::endl;

  std::cout << "Completed chunks: " << manager.get_completed_chunks()
            << std::endl;

  return 0;
}