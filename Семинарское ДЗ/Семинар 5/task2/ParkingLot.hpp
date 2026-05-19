#include "SimpleSemaphre.hpp"
#include <mutex>

using namespace std;

mutex cout_mutex;

class ParkingLot {
private:
  int capacity;

  int occupied = 0;

  SimpleSemaphore semaphore;

  mutex mtx;

  mutex cout_mutex;

  condition_variable cv;

  atomic<int> vip_waiting{0};

public:
  ParkingLot(int cap) : capacity(cap), semaphore(cap) {}

  void park(bool isVIP, int timeout_ms) {

    thread::id tid = this_thread::get_id();

    if (isVIP)
      vip_waiting++;

    auto start = chrono::steady_clock::now();

    while (true) {

      {
        unique_lock<mutex> lock(mtx);

        // Обычные машины ждут VIP
        if (!isVIP && vip_waiting > 0) {

          lock.unlock();

          this_thread::yield();

          continue;
        }
      }

      bool acquired = semaphore.try_acquire_for(timeout_ms);

      if (acquired) {

        {
          lock_guard<mutex> lock(mtx);

          occupied++;

          if (isVIP)
            vip_waiting--;

          lock_guard<mutex> out(cout_mutex);

          cout << "[PARK] Thread " << tid << " | " << (isVIP ? "VIP" : "NORMAL")
               << " | occupied = " << occupied
               << " | free = " << (capacity - occupied) << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(1000 + rand() % 3000));

        this_thread::yield();

        leave();

        return;
      }

      auto now = chrono::steady_clock::now();

      auto elapsed =
          chrono::duration_cast<chrono::milliseconds>(now - start).count();

      if (elapsed >= timeout_ms) {

        if (isVIP)
          vip_waiting--;

        lock_guard<mutex> out(cout_mutex);

        cout << "[TIMEOUT] Thread " << tid << " | "
             << (isVIP ? "VIP" : "NORMAL") << endl;

        return;
      }

      this_thread::yield();
    }
  }

  void leave() {

    {
      lock_guard<mutex> lock(mtx);

      occupied--;

      lock_guard<mutex> out(cout_mutex);

      cout << "[LEAVE] Thread " << this_thread::get_id()
           << " | occupied = " << occupied
           << " | free = " << (capacity - occupied) << endl;
    }

    semaphore.release();

    cv.notify_all();
  }

  void increase_capacity(int value) {

    {
      lock_guard<mutex> lock(mtx);

      capacity += value;

      lock_guard<mutex> out(cout_mutex);

      cout << "[CAPACITY INCREASE] +" << value
           << " | new capacity = " << capacity << endl;
    }

    semaphore.add_permits(value);
  }

  void decrease_capacity(int value) {

    lock_guard<mutex> lock(mtx);

    if (capacity - value >= occupied) {

      capacity -= value;

      lock_guard<mutex> out(cout_mutex);

      cout << "[CAPACITY DECREASE] -" << value
           << " | new capacity = " << capacity << endl;
    }
  }
};
