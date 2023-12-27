#include "CommonUtility.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

std::string ReadFileContent(std::string_view filename) {
  const std::ifstream file(filename.data());
  std::stringstream str_stream;
  str_stream << file.rdbuf();
  return str_stream.str();
}
