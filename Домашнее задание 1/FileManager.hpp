#pragma once

#include "Group.hpp"
#include <fstream>
#include <string>

#pragma pack(push, 1)
struct Header {
  char signature[4];
  int version;
  int size;
};
#pragma pack(pop)

class FileManager {
public:
  FileManager();
  int saveGroup(Group g, int fVersion);
  Group loadGroup(std::string fileName);
  Student loadStudent(std::ifstream &in);
  std::string readString(std::ifstream &in);
  int checkFileFormat();
  std::string getFileName(Group g);

private:
  template <typename T> void writeString(T s);
  void SaveStudent(std::ofstream &out, std::shared_ptr<Student> s);
};