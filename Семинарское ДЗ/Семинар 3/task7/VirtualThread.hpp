#include <mutex>
#include <string>
#include <vector>

#pragma once
class VirtualThread {
public:
  VirtualThread(int threadId, int n);
  void runNextTask(bool &status);
  void run();

private:
  std::mutex coutMutex;
  int threadId;
  int id;
  std::vector<std::string> q;
};