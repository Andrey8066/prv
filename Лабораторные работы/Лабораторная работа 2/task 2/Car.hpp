#pragma once

class Car {
public:
  int id;
  char direction;
  bool is_emergency;
  

  Car(int id, char dir, bool emergency = false)
      : id(id), direction(dir), is_emergency(emergency) {}
};