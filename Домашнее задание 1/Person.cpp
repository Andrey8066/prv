#include "Person.hpp"
#include "iostream"
void Person::print() { std::cout << "Имя пользователя: " << name << std::endl; }

Person::Person(std::string p_name) { name = p_name; }
Person::~Person() { name.clear(); }
std::string Person::getName() { return name; }
void Person::setName(std::string pName) { name = pName; }