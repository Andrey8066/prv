#pragma once

#include "Person.hpp"
#include <string>
class Teacher : private Person {
private:
  std::string subject;

public:
  Teacher(std::string pName, std::string tSubject);
  void print();
};