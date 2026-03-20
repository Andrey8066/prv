#include <cstddef>
#include <semaphore>
#include <string>
class Order {
public:
  Order(size_t pd, size_t dd, size_t i);
  void processOrder();
  void deliverOrder();
  void print();
  std::binary_semaphore readyForDeliver;
  std::string getStatus();

private:
  size_t processingDelay;
  size_t delieveryDelay;
  size_t id;
  std::string status = "-";
};