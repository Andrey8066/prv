#include "Task.hpp"
#include <chrono>
#include <random>
#include <thread>

Task::Task() {
  std::random_device rd;
  std::mt19937 m(rd());
  std::uniform_int_distribution<long long> dist(0, 1000);
  this->value = dist(m) % 50 + 1;
  this->priority = dist(m) % 10 + 1;
  this->durationMs = dist(m) % 800 + 200;
  this->steps = dist(m) % 4 + 2;
  this->currentStep = 1;
}

int Task::getValue() { return this->value; };
int Task::getPriority() { return this->priority; };
long long Task::getDurationMs() { return this->durationMs; };
int Task::getSteps() { return this->steps; };
int Task::getCurrentStep() { return this->currentStep; };
void Task::run() {
  std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
  currentStep++;
}