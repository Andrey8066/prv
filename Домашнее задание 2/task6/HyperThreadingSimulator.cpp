#include "HyperThreadingSimulator.hpp"
#include "iostream"

HyperThreadingSimulator::HyperThreadingSimulator(int n) : vt1(n), vt2(n) {}
void HyperThreadingSimulator::execute() {
  bool vt1Status = false;
  bool vt2Status = false;
  do {
    long long v1 = this->vt1.run();
    vt1Status = (v1 == 0);
    if (!vt1Status)
      std::cout << "Поток 1 вычислил значение факториала " << v1 << std::endl;
    long long v2 = this->vt2.run();
    vt2Status = (v2 == 0);
    if (!vt2Status)
      std::cout << "Поток 2 вычислил значение факториала " << v2 << std::endl;
  } while (!(vt1Status && vt2Status));
  return;
}