#include "Teacher.hpp"
#include "Person.hpp"
#include <iostream>
#include <string>

Teacher::Teacher(std::string pName, std::string tSubject) : Person(pName) {
  subject = tSubject;
}

void Teacher::print() {
  std::cout << "Преподаватель предмета " << subject << ", " << name
            << std::endl;
}