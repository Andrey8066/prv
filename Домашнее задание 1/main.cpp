
#include "FileManager.hpp"
#include "Group.hpp"
#include "Student.hpp"
#include <iostream>
#include <string>

using namespace std;

int main() {
  Student s1(string("Андрей"), 0);
  s1.addMark("ПРВ", 5);
  s1.addMark("OTC", 4);
  s1.print();
  Student s2(string("Фёдор"), 1);
  s2.addMark("ПРВ", 5);
  s2.addMark("OTC", 4);
  s2.print();
  Student s3(string("Егор"), 2);
  s3.addMark("ПРВ", 3);
  s3.addMark("OTC", 4);
  s3.print();

  Group g1("ИУ1-11Б");

  g1.addStudent(s1);
  g1.addStudent(s3);
  g1.addStudent(s2);

  g1.print();

  g1.getSortedStudents();

  g1.getStudentsByAverage(4);

  cout << g1.getAverage() << endl;

  g1.removeStudentByName("Егор");
  g1.print();

  FileManager fm;
  fm.saveGroup(g1, 0);

  Group g2 = fm.loadGroup("ИУ1-11Б_2");
  g2.print();
}