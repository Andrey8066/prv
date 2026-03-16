
#include "Group.hpp"
#include "Student.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

Group::Group(string pName) : name(pName){};

void Group::addStudent(Student gStudent) {
  students.push_back(make_shared<Student>(gStudent));
}
void Group::removeStudentByName(string name) {
  for (int i = 0; i < students.size(); i++) {
    if (students.at(i)->getName() == name) {
      students.erase(students.begin() + i);
      return;
    }
  }
}
void Group::print() {
  cout << "Группа " << name << ". Колличество студентов " << students.size()
       << endl;
  for (shared_ptr<Student> student : students) {
    student->print();
  }
}
void Group::print(vector<shared_ptr<Student>> inputStudents) {
  for (shared_ptr<Student> student : inputStudents) {
    student->print();
  }
}
vector<shared_ptr<Student>> Group::getAllStudents() { return students; }
double Group::getAverage() {
  double res = 0;
  for (shared_ptr<Student> student : students) {
    res += student->getAverage();
  }
  return res / students.size();
}
vector<shared_ptr<Student>> Group::getSortedStudents() {

  sort(students.begin(), students.end(),
       [](shared_ptr<Student> a, shared_ptr<Student> b) {
         return a->getAverage() < b->getAverage();
       });
  print();
  return students;
}
vector<shared_ptr<Student>> Group::getStudentsByAverage(double average) {
  vector<shared_ptr<Student>> res;

  for (shared_ptr<Student> student : students) {
    if (student->getAverage() >= average) {
      res.push_back(student);
    }
  }

  cout << "Группа " << name << ". Колличество студентов с баллом выше "
       << average << ": " << res.size() << endl;
  print(res);
  return res;
}

int Group::getStudentQuantity() { return students.size(); }
string Group::getName() { return name; }