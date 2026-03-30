#include "queueGenerator.hpp"
#include <memory>
#include <thread>
int main() {
  std::shared_ptr<Claster> c = std::make_shared<Claster>();
  QueueGenerator qg(c);
  std::thread qgThread(&QueueGenerator::run, &qg);
  std::thread cThread(&Claster::run, c.get());

  qgThread.detach();
  cThread.join();
  return 0;
}