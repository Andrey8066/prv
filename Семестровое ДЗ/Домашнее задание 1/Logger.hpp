#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <mutex>
#include <sstream>
#include <string>
template <typename T> class Logger {
private:
  std::ofstream log_file;
  std::mutex mtx;
  std::string getTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d_%H-%M-%S");
    return ss.str();
  }
  std::string getString(T &message) {
    std::stringstream ss;

    ss << message;

    std::string res = "[" + getTime() + "]  " + ss.str() + "\n";
    return res;
  }

public:
  Logger<T>(std::string fileName) : log_file(fileName) {}
  Logger<T>(char &fileName) : log_file(&fileName) {}
  Logger<T>() {
    std::chrono::time_point<std::chrono::high_resolution_clock> now =
        std::chrono::high_resolution_clock::now();

    log_file = std::ofstream(getTime() + ".log");
  }
  void log(T message) {
    std::string stringMessage = getString(message);
    std::lock_guard<std::mutex> lock(mtx);
    {
      log_file << stringMessage;
      std::cout << stringMessage;
    }
  }
};
