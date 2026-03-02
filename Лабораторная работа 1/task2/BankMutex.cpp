
#include "BankMutex.h"
#include "iostream"
#include <cstdlib>
#include <mutex>
#include <string>

using namespace std;

std::mutex BankMutex::coutMtx;

BankMutex::BankMutex() {
  balance = rand();
  id = rand();
  std::lock_guard<std::mutex> coutLock(coutMtx);
  cout << "Балланс счета №" << id << " : " << to_string(balance) << endl;
}
void BankMutex::decrease(long long sum) {
  {
    std::lock_guard<std::mutex> lock(mtx);
    balance -= sum;
  }
  std::lock_guard<std::mutex> coutLock(coutMtx);
  cout << "Балланс счета №" << id << " уменьшен  на " << to_string(sum)
       << ", теперь балланс счета: " << to_string(balance) << endl;
}
void BankMutex::increase(long long sum) {
  {
    std::lock_guard<std::mutex> lock(mtx);
    balance += sum;
  }
  std::lock_guard<std::mutex> coutLock(coutMtx);
  cout << "Балланс счета №" << id << " увеличен  на " << to_string(sum)
       << ", теперь балланс счета: " << to_string(balance) << endl;
}

long long BankMutex::getBalance() {
  std::lock_guard<std::mutex> lock(mtx);
  return balance;
}

string BankMutex::print() {
  std::lock_guard<std::mutex> lock(mtx);
  string res = "Балланс счета №" + to_string(id) + " : " + to_string(balance);
  std::lock_guard<std::mutex> coutLock(coutMtx);
  cout << res << endl;
  return res;
}
