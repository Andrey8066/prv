#include <mutex>
template <typename T> class Account {
private:
  T balance;
  std::mutex mtx;

public:
  Account(T balance);
  T getBalance();
  T deposit(T amount);
  T withdraw(T amount);
};

template <typename T> T Account<T>::getBalance() { return this->balance; }
template <typename T> T Account<T>::deposit(T amount) {
  mtx.lock();
  balance += amount;
  mtx.unlock();
  return this->balance;
}
template <typename T> T Account<T>::withdraw(T amount) {
  mtx.lock();
  balance -= amount;
  mtx.unlock();
  return this->balance;
}

template <typename T>
Account<T>::Account(T initial) : balance(initial), mtx() {}