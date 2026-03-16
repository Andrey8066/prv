#include "iostream"
#include <chrono>

int main() {
  int n;
  std::cout << "Введите количество секунд: ";
  std::cin >> n;
  std::chrono::hours h =
      std::chrono::duration_cast<std::chrono::hours>((std::chrono::seconds(n)));
  std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(
      std::chrono::duration_cast<std::chrono::minutes>(
          std::chrono::seconds(n)) -
      std::chrono::duration_cast<std::chrono::minutes>(h));
  std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(
      std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::seconds(n)) -
      std::chrono::duration_cast<std::chrono::seconds>(h) -
      std::chrono::duration_cast<std::chrono::seconds>(m));

  std::cout << h.count() << " час " << m.count() << " минута " << s.count()
            << " секунда" << std::endl;

  return 0;
}