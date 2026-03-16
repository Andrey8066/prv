#include "RecordBook.hpp"
#include <iostream>
#include <memory>
#include <vector>

void RecordBook::addMark(Mark mark) {
  marks.push_back(std::make_shared<Mark>(mark));
}
double RecordBook::getAverage() {
  double res = 0;
  for (std::shared_ptr<Mark> mark : marks) {
    res += mark->grade;
  }
  return res / marks.size();
}
void RecordBook::print() {
  std::cout << "Зачетная книжка №" << recordBookNumber << " со средним баллом"
            << getAverage() << std::endl;
}
RecordBook::RecordBook(int number) { recordBookNumber = number; }
int RecordBook::getMarksQuantity() { return marks.size(); }
int RecordBook::getNumber() { return recordBookNumber; }
std::vector<std::shared_ptr<Mark>> RecordBook::getMarks() { return marks; }