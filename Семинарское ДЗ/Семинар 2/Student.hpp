#pragma once

#include "Person.hpp"
#include "RecordBook.hpp"

class Student : public Person {
private:
  RecordBook recordBook;

public:
  Student(std::string pName, int rBNumber);
  double getAverage();
  void print();
  void addMark(Mark mark);
  void addMark(std::string mSubject, double mGrade);
  RecordBook getRecordBook();
};