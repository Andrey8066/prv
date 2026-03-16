#include "FileManager.hpp"
#include "Group.hpp"
#include "Student.hpp"
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <string>

FileManager::FileManager() {}

int FileManager::saveGroup(Group g, int fVersion) {
  std::ofstream out = std::ofstream(getFileName(g), std::ios::binary);

  Header h{{'G', 'R', 'P', 'T'},
           fVersion,
           static_cast<int>(g.getAllStudents().size())};
  std::cout << "Размер структуры заголовка " << sizeof(h) << std::endl;
  out.write(reinterpret_cast<const char *>(&h), sizeof(h));
  for (std::shared_ptr<Student> s : g.getAllStudents()) {
    SaveStudent(out, s);
  }
  out.close();
  return 0;
}

void FileManager::SaveStudent(std::ofstream &out, std::shared_ptr<Student> s) {
  int nameLen = s->getName().size();
  out.write(reinterpret_cast<const char *>(&nameLen), sizeof(nameLen));
  out.write(s->getName().data(), nameLen);
  int number = s->getRecordBook().getNumber();
  out.write(reinterpret_cast<const char *>(&number), sizeof(number));
  int marksQuantity = s->getRecordBook().getMarksQuantity();
  out.write(reinterpret_cast<const char *>(&marksQuantity),
            sizeof(marksQuantity));
  for (std::shared_ptr<Mark> m : s->getRecordBook().getMarks()) {
    int subjectLen = m->subject.size();
    out.write(reinterpret_cast<const char *>(&subjectLen), sizeof(subjectLen));
    char *subject = m->subject.data();
    out.write(reinterpret_cast<const char *>(&subject), subjectLen);
    out.write(reinterpret_cast<const char *>(&m->grade), sizeof(m->grade));
  }
}

Group FileManager::loadGroup(std::string fileName) {
  std::string name = fileName.substr(0, fileName.find("_"));
  Group g(name);
  std::ifstream in = std::ifstream(fileName, std::ios::binary);
  Header h;
  in.read(reinterpret_cast<char *>(&h), sizeof(h));
  std::cout << h.signature << " " << h.size << " " << h.version << std::endl;
  for (int i = 0; i < h.size; i++) {
    g.addStudent(loadStudent(in));
  }
  in.close();
  return g;
}

Student FileManager::loadStudent(std::ifstream &in) {
  std::string name = readString(in);
  int number;
  in.read(reinterpret_cast<char *>(&number), sizeof(number));

  Student s(name, number);

  int marksQuantity;
  in.read(reinterpret_cast<char *>(&marksQuantity), sizeof(marksQuantity));

  for (int i = 0; i < marksQuantity; i++) {
    std::string subject = readString(in);
    double grade;
    in.read(reinterpret_cast<char *>(&grade), sizeof(grade));
    s.addMark(subject, grade);
  }
  return s;
}

std::string FileManager::readString(std::ifstream &in) {
  int len;
  in.read(reinterpret_cast<char *>(&len), sizeof(len));

  std::string s(len, '\0');
  in.read(s.data(), len);

  return s;
}

std::string FileManager::getFileName(Group g) {
  return "./" + g.getName() + "_" + std::to_string(g.getStudentQuantity());
}