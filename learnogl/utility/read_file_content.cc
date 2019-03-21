// Copyright 2019.

#include <fstream>
#include <string>
#include <iterator>

#include "utility/utility.h"
#include "glog/logging.h"

std::string ReadFileContent(const char* file_name) {
  std::ifstream file_stream(file_name, std::ifstream::in);
  LOG_IF(FATAL, !file_stream) << "File does not exist: " << file_name;

  std::string file_content((std::istreambuf_iterator<char>(file_stream)),
                           (std::istreambuf_iterator<char>()));
  return file_content;
}


