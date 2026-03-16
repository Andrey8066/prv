#include "VirtualThread.hpp"
#include <algorithm>
#include <iostream>

VirtualThread::VirtualThread(int n) {
  for (int i = 0; i < n; i++) {
    taskVector.emplace_back(Task());
  }
}

void VirtualThread::printTask(Task &t, int threadId) {
  std::cout << "Номер потока " << threadId << ", номер шага "
            << t.getCurrentStep() << "/" << t.getSteps()
            << ", приоритет задачи " << t.getPriority()
            << ", результат вычисления " << t.getValue() << std::endl;
}

void VirtualThread::runStep(int threadId) {
  std::sort(taskVector.begin(), taskVector.end(),
            [](Task a, Task b) { return a.getPriority() < b.getPriority(); });
  Task *task = &taskVector.back();
  task->run();
  printTask(*task, threadId);

  if (task->getCurrentStep() == task->getSteps())
    taskVector.pop_back();
}

bool VirtualThread::hasTask() { return !taskVector.empty(); }