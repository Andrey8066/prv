
#include "BankAtomic.h"
#include "iostream"
#include <string>

using namespace std;

BankAtomic::BankAtomic() {
  balance = rand();
  id = rand();
  cout << "Балланс счета №" << id << " : " << to_string(balance.load()) << endl;
}
void BankAtomic::decrease(long long sum) {
  balance.fetch_sub(sum);
  // cout << "Балланс счета №" << id << " уменьшен  на " << to_string(sum)
  //    << ", теперь балланс счета: " << to_string(balance.load()) << endl;
}
void BankAtomic::increase(long long sum) {
  balance.fetch_add(sum);
  // cout << "Балланс счета №" << id << " увеличен  на " << to_string(sum)
  //  << ", теперь балланс счета: " << to_string(balance.load()) << endl;
}

long long BankAtomic::getBalance() { return balance; }

string BankAtomic::print() {
  string res =
      "Балланс счета №" + to_string(id) + " : " + to_string(balance.load());
  cout << res << endl;
  return res;
}
