#include "TaskTimer.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

std::vector<int> generateVector(int n) {
  std::random_device rd;
  std::mt19937 m(rd());
  std::uniform_int_distribution<int> dist(0, n);

  std::vector<int> res;
  for (int i = 0; i < n; i++) {
    res.emplace_back(dist(m));
  }
  return res;
}

void bubbleSort(std::vector<int> &v) {
  for (size_t i = 0; i < v.size(); i++) {
    for (size_t j = 0; j + 1 < v.size() - i; j++) {
      if (v[j] > v[j + 1]) {
        std::swap(v[j], v[j + 1]);
      }
    }
  }
}

void insertionSort(std::vector<int> &v) {
  for (size_t i = 1; i < v.size(); i++) {
    int key = v[i];
    size_t j = i;
    while (j > 0 && v[j - 1] > key) {
      v[j] = v[j - 1];
      j--;
    }
    v[j] = key;
  }
}

void merge(std::vector<int> &v, size_t left, size_t mid, size_t right) {
  std::vector<int> temp(right - left + 1);
  size_t i = left, j = mid + 1, k = 0;

  while (i <= mid && j <= right) {
    temp[k++] = (v[i] <= v[j]) ? v[i++] : v[j++];
  }
  while (i <= mid)
    temp[k++] = v[i++];
  while (j <= right)
    temp[k++] = v[j++];
  for (size_t t = 0; t < temp.size(); t++)
    v[left + t] = temp[t];
}

void mergeSortHelper(std::vector<int> &v, size_t left, size_t right) {
  if (left >= right)
    return;
  size_t mid = left + (right - left) / 2;
  mergeSortHelper(v, left, mid);
  mergeSortHelper(v, mid + 1, right);
  merge(v, left, mid, right);
}

void mergeSort(std::vector<int> &v) {
  if (!v.empty())
    mergeSortHelper(v, 0, v.size() - 1);
}

size_t partition(std::vector<int> &v, size_t low, size_t high) {
  int pivot = v[high];
  size_t i = low;
  for (size_t j = low; j < high; j++) {
    if (v[j] <= pivot) {
      std::swap(v[i], v[j]);
      i++;
    }
  }
  std::swap(v[i], v[high]);
  return i;
}

void quickSortHelper(std::vector<int> &v, size_t low, size_t high) {
  if (low < high) {
    size_t pi = partition(v, low, high);
    if (pi > 0)
      quickSortHelper(v, low, pi - 1); // защита от size_t < 0
    quickSortHelper(v, pi + 1, high);
  }
}

void quickSort(std::vector<int> &v) {
  if (!v.empty())
    quickSortHelper(v, 0, v.size() - 1);
}

int main() {
  TaskTimer<std::chrono::nanoseconds> tt;
  std::vector<int> v = generateVector(10000);

  std::vector<int> copy1 = v;
  tt.measureSorting(copy1, [](std::vector<int> &copy1) {
    std::sort(copy1.begin(), copy1.end());
  });
  std::cout << "Время выполнения сортировки: " << tt.getDuration().count()
            << " наносекунд" << std::endl;

  std::vector<int> copy2 = v;
  tt.measureSorting(copy2, bubbleSort);
  std::cout << "Время выполнения bubbleSort сортировки: "
            << tt.getDuration().count() << " наносекунд" << std::endl;

  std::vector<int> copy3 = v;
  tt.measureSorting(copy3, insertionSort);
  std::cout << "Время выполнения insertionSort сортировки: "
            << tt.getDuration().count() << " наносекунд" << std::endl;

  std::vector<int> copy4 = v;
  tt.measureSorting(copy4, mergeSort);
  std::cout << "Время выполнения mergeSort сортировки: "
            << tt.getDuration().count() << " наносекунд" << std::endl;

  std::vector<int> copy5 = v;
  tt.measureSorting(copy5, quickSort);
  std::cout << "Время выполнения quickSort сортировки: "
            << tt.getDuration().count() << " наносекунд" << std::endl;
}
