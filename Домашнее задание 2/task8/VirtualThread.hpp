#include "Task.hpp"
#include <vector>

class VirtualThread {
private:
  int threadId;
  std::vector<Task> taskVector;
  void printTask(Task &t, int threadId);

public:
  VirtualThread(int n);
  void runStep(int threadId);
  bool hasTask();
};