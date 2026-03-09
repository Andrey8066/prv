
#include "BankMutex.h"
#include "iostream"
#include <mutex>
#include <string>

using namespace std;

std::mutex BankMutex::coutMtx;

BankMutex::BankMutex() {
  balance = rand();
  id = rand();
  cout << "Балланс счета №" << id << " : " << to_string(balance) << endl;
}
void BankMutex::decrease(long long sum) {
  {
    std::lock_guard<std::mutex> lock(mtx);
    balance -= sum;
  }
  // cout << "Балланс счета №" << id << " уменьшен  на " << to_string(sum)
  //   << ", теперь балланс счета: " << to_string(balance) << endl;
}
void BankMutex::increase(long long sum) {
  {
    std::lock_guard<std::mutex> lock(mtx);
    balance += sum;
  }
  // cout << "Балланс счета №" << id << " увеличен  на " << to_string(sum)
  //   << ", теперь балланс счета: " << to_string(balance) << endl;
}

long long BankMutex::getBalance() { return balance; }

string BankMutex::print() {
  std::lock_guard<std::mutex> lock(mtx);
  string res = "Балланс счета №" + to_string(id) + " : " + to_string(balance);
  cout << res << endl;
  return res;
}
