
#include "Bank.h"
#include "iostream"
#include <cstdlib>
#include <string>

using namespace std;

Bank::Bank() {
  balance = rand();
  id = rand();
  cout << "Балланс счета №" << id << " : " << to_string(balance) << endl;
}
void Bank::decrease(long long sum) {
  balance -= sum;
  // cout << "Балланс счета №" << id << " уменьшен  на " << to_string(sum)
  //    << ", балланс счета: " << to_string(balance) << endl;
}
void Bank::increase(long long sum) {
  balance += sum;
  // cout << "Балланс счета №" << id << " увеличен  на " << to_string(sum)
  //    << ", балланс счета: " << to_string(balance) << endl;
}

long long Bank::getBalance() { return balance; }

string Bank::print() {
  string res = "Балланс счета №" + to_string(id) + " : " + to_string(balance);
  cout << res << endl;
  return res;
}
