#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <threads.h>
#include <vector>

using namespace std;

// Однопоточное умножение двух матриц
void multiplexMatrices(vector<vector<int>> &A, vector<vector<int>> &B,
                       vector<vector<int>> &res) {
  int N = A.size();

  for (int i = 0; i < N; i++) {
    vector<int> row;
    for (int j = 0; j < N; j++) {
      row.push_back(A[i][0] * B[0][j]);
      for (int k = 1; k < N; k++) {
        row[j] = row[j] + A[i][k] * B[k][j];
      }
    }
    res.push_back(row);
  }
}

// Выисление i строки произведения матриц
void multiplexMatricesToRow(vector<vector<int>> &A, vector<vector<int>> &B,
                            vector<vector<int>> &res, int i) {
  vector<int> row;
  int N = A.size();

  for (int j = 0; j < N; j++) {
    row.push_back(A[i][0] * B[0][j]);
    for (int k = 1; k < N; k++) {
      row[j] = row[j] + A[i][k] * B[k][j];
    }
  }
  res[i] = row;
}

// Параллельное выисление произведения матриц
void multiplexMatricesMultiThreads(vector<vector<int>> &A,
                                   vector<vector<int>> &B,
                                   vector<vector<int>> &res) {
  vector<thread> threads;
  int N = A.size();
  for (int i = 0; i < N; i++) {
    threads.emplace_back(multiplexMatricesToRow, ref(A), ref(B), ref(res), i);
  }
  for (auto &t : threads)
    t.join();
}

// Метод для генерации матрицы по заданному размеру
vector<vector<int>> generateMatrix(int N) {
  vector<vector<int>> res;

  random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(1, 1000);

  for (int i = 0; i < N; i++) {
    vector<int> row;
    for (int j = 0; j < N; j++) {
      int el = dist(gen);
      row.push_back(el);
    }
    res.push_back(row);
  }
  return res;
}

// Метод для вывода матрицы
void printMatrinx(vector<vector<int>> X) {
  int N = X.size();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cout << X.at(i).at(j) << " ";
    }
    cout << endl;
  }
}

// Вычисление времени выполнения процесса
int timeProcces(vector<vector<int>> &A, vector<vector<int>> &B,
                vector<vector<int>> &res,
                void (*func)(vector<vector<int>> &A, vector<vector<int>> &B,
                             vector<vector<int>> &res)) {

  auto start = std::chrono::high_resolution_clock::now();

  func(A, B, res);

  auto end = std::chrono::high_resolution_clock::now();

  auto time = chrono::duration_cast<chrono::microseconds>(end - start);

  cout << "Матрица вычислялась " << time.count() << endl;

  return time.count();
}

int main() {
  int N = 0;
  cout << "Введите размер матрицы N \n";
  cin >> N;

  vector<vector<int>> A = generateMatrix(N);
  vector<vector<int>> B = generateMatrix(N);

  vector<vector<int>> C;
  vector<vector<int>> D(N);

  int timeC = timeProcces(A, B, C, multiplexMatrices);

  int timeD = timeProcces(A, B, D, multiplexMatricesMultiThreads);

  int dif = timeC - timeD;
  float val = float(timeC) / float(timeD);

  cout << "Параллельное вычисление быстрее на  " << dif << " микросекунд это в "
       << val << " раз" << endl;
  return 0;
}
