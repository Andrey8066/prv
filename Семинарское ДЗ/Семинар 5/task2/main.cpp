#include "ParkingLot.hpp"

using namespace std;

int main() {

  srand(time(nullptr));

  ParkingLot parking(3);

  vector<thread> cars;

  for (int i = 0; i < 10; i++) {

    bool vip = rand() % 3 == 0;

    cars.emplace_back([&, vip]() { parking.park(vip, 5000); });

    cars.back().detach();

    this_thread::yield();
  }

  this_thread::sleep_for(chrono::seconds(2));

  parking.increase_capacity(2);

  this_thread::sleep_for(chrono::seconds(10));

  return 0;
}