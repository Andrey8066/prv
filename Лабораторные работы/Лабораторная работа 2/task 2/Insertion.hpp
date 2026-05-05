#include "Car.hpp"
#include "TrafickLight.hpp"
#include <chrono>
#include <map>
#include <mutex>
#include <queue>
#include <semaphore>

class Intersection {
private:
  int id;

  std::map<char, std::queue<Car>> queues;
  TrafficLight light;

  std::mutex mtx;
  std::counting_semaphore<10> sem;

  bool emergency_flag;

  const int MAX_CAPACITY = 20;
  const int JAM_THRESHOLD = 15;

public:
  Intersection(int id) : id(id), sem(5), emergency_flag(false), light(id) {
    queues['N'] = std::queue<Car>();
    queues['S'] = std::queue<Car>();
    queues['E'] = std::queue<Car>();
    queues['W'] = std::queue<Car>();
  }

  void addCar(const Car &car) {
    std::lock_guard<std::mutex> lock(mtx);

    queues[car.direction].push(car);

    if (car.is_emergency) {
      emergency_flag = true;
    }
  }

  char checkOverload() {
    for (auto &[dir, q] : queues) {
      if ((double)q.size() / MAX_CAPACITY > 0.7) {
        return dir;
      }
    }
    return ' ';
  }

  void passCars(char dir, int limit = 5) {
    int count = 0;

    while (!queues[dir].empty() && count < limit) {
      sem.acquire();

      Car car = queues[dir].front();
      queues[dir].pop();

      std::cout << "Машина " << car.id << " проехала (" << dir << ")\n";

      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      sem.release();
      count++;
    }
  }

  void normalCycle() {
    for (char dir : {'N', 'S', 'E', 'W'}) {
      light.setGreen(dir);
      passCars(dir);
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
  }

  void adaptiveCycle(char dir) {
    std::cout << "Адаптивный режим " << dir << std::endl;

    light.setGreen(dir);
    passCars(dir, 10);
  }

  void handleEmergency() {
    std::cout << "!!! Экстренные службы !!!\n";

    light.setAllRed();

    for (auto &[dir, q] : queues) {
      if (!q.empty() && q.front().is_emergency) {
        light.setGreen(dir);
        passCars(dir, 10);
        break;
      }
    }

    emergency_flag = false;
  }

  bool detectJam() {
    for (auto &[dir, q] : queues) {
      if (q.size() > JAM_THRESHOLD) {
        return true;
      }
    }
    return false;
  }

  void handleJam() {
    std::cout << "!!! Обнаружена пробка !!!\n";

    for (auto &[dir, q] : queues) {
      light.setGreen(dir);
      passCars(dir, 10);
    }
  }

  void run() {
    while (true) {

      if (emergency_flag) {
        handleEmergency();
        mtx.unlock();
        continue;
      }

      char overloaded = checkOverload();

      if (overloaded != ' ') {
        adaptiveCycle(overloaded);
      } else {
        normalCycle();
      }

      if (detectJam()) {
        handleJam();
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  }
};
