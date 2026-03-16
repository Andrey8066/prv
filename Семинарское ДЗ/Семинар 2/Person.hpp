#pragma once

#include <string>

class Person {
protected:
  std::string name;

public:
  Person(std::string pName);
  virtual void print();
  virtual ~Person();
  std::string getName();
  void setName(std::string pName);
};