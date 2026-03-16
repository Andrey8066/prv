#include <functional>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

template <typename T> class MatrixProcessor {
private:
  std::vector<std::vector<T>> matrix;
  size_t numThreads;

public:
  MatrixProcessor(size_t n, size_t m, size_t threads) : numThreads(threads) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<T> dist(1, 1000);
    for (int i = 0; i < n; i++) {
      matrix.emplace_back(std::vector<T>());
      for (int j = 0; j < m; j++) {
        matrix[i].emplace_back(dist(gen));
      }
    }
  }

  template <typename Func> void apply(Func func) {
    size_t N = matrix.size();
    if (N == 0)
      return;
    size_t M = matrix[0].size();

    size_t rowsPerThread = (N + numThreads - 1) / numThreads;

    std::vector<std::thread> threads;

    for (size_t t = 0; t < numThreads; ++t) {
      size_t startRow = t * rowsPerThread;
      size_t endRow = std::min(startRow + rowsPerThread, N);

      threads.emplace_back([&, startRow, endRow]() {
        for (size_t i = startRow; i < endRow; ++i) {
          for (size_t j = 0; j < M; ++j) {
            matrix[i][j] = func(matrix[i][j]);
          }
          std::this_thread::yield();
        }
      });
    }

    for (auto &th : threads) {
      th.join();
    }
  }
  void print() {
    for (int i = 0; i < matrix.size(); i++) {
      std::vector<T> line = matrix.at(i);
      for (int j = 0; j < line.size(); j++) {
        std::cout << line[j] << " ";
      }
      std::cout << std::endl;
    }
  }
};