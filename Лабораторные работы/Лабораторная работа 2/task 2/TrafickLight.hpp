#include <iostream>
#include <mutex>

#pragma once
class TrafficLight {
private:
  size_t id;
  char current_green;
  std::mutex coutMtx;

public:
  TrafficLight(size_t i) : current_green(' '), id{i} {}

  void setGreen(char dir) {
    coutMtx.lock();
    current_green = dir;
    std::cout << "Светофор " << id << " зеленый: " << dir << std::endl;
    coutMtx.unlock();
  }

  void setAllRed() {
    coutMtx.lock();
    current_green = ' ';
    std::cout << "Светофор " << id << " везде красный" << std::endl;
    coutMtx.unlock();
  }

  char getCurrent() { return current_green; }
};