#pragma once

#include "Student.hpp"
#include <vector>
class Group {
private:
  std::string name;
  std::vector<std::shared_ptr<Student>> students;

public:
  Group(std::string pName);
  void addStudent(Student gStudent);
  void removeStudentByName(std::string name);
  void print();
  void print(std::vector<std::shared_ptr<Student>> inputStudents);
  std::vector<std::shared_ptr<Student>> getAllStudents();
  double getAverage();
  std::vector<std::shared_ptr<Student>> getSortedStudents();
  std::vector<std::shared_ptr<Student>> getStudentsByAverage(double average);
  int getStudentQuantity();
  std::string getName();
};