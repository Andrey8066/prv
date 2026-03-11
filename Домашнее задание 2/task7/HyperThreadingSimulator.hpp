#include "VirtualThread.hpp"

#pragma once
class HyperThreadingSimulator {
private:
  VirtualThread vt1;
  VirtualThread vt2;

public:
  HyperThreadingSimulator(int n);
  void execute();
  void process(VirtualThread &vt);
};