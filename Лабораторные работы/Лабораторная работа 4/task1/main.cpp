#include <chrono>
#include <cmath>
#include <iostream>
#include <sycl/sycl.hpp>
#include <vector>

using namespace sycl;

// ================= CPU =================

void scaleCPU(const std::vector<float> &A, std::vector<float> &B, float k) {
  for (size_t i = 0; i < A.size(); i++) {
    B[i] = A[i] * k;
  }
}

int main() {
  try {

    queue q(default_selector_v);
    std::cout << "Device: " << q.get_device().get_info<info::device::name>()
              << "\n";

    const size_t N = 1'000'000;
    const float k = 2.5f;

    std::vector<float> A(N);
    std::vector<float> B_cpu(N);
    std::vector<float> B_gpu(N);

    // Инициализация
    for (size_t i = 0; i < N; i++) {
      A[i] = static_cast<float>(i) * 0.1f;
    }

    // ================= CPU =================

    auto cpu_start = std::chrono::high_resolution_clock::now();

    scaleCPU(A, B_cpu, k);

    auto cpu_end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> cpu_time = cpu_end - cpu_start;

    // ================= SYCL =================

    {
      buffer<float> bufA(A.data(), range<1>(N));
      buffer<float> bufB(B_gpu.data(), range<1>(N));

      auto gpu_start = std::chrono::high_resolution_clock::now();

      q.submit([&](handler &h) {
        accessor accA(bufA, h, read_only);
        accessor accB(bufB, h, write_only);

        h.parallel_for(range<1>(N),
                       [=](id<1> idx) { accB[idx] = accA[idx] * k; });
      });

      q.wait();

      auto gpu_end = std::chrono::high_resolution_clock::now();

      std::chrono::duration<double> gpu_time = gpu_end - gpu_start;

      // Проверка
      bool correct = true;

      for (size_t i = 0; i < N; i++) {

        if (std::fabs(B_cpu[i] - B_gpu[i]) > 1e-5f) {
          correct = false;
          break;
        }
      }

      std::cout << "Results: " << (correct ? "correct" : "incorrect") << "\n";

      std::cout << "CPU time: " << cpu_time.count() << " s\n";

      std::cout << "SYCL time: " << gpu_time.count() << " s\n";

      std::cout << "Acceleration: " << cpu_time.count() / gpu_time.count()
                << "x\n";
    }
  } catch (exception const &e) {

    std::cout << "SYCL exception: " << e.what() << "\n";

    return 1;
  }

  return 0;
}