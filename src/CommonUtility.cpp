#include "CommonUtility.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include <SFML/System/Vector2.hpp>

std::string ReadFileContent(std::string_view filename) {
  const std::ifstream file(filename.data());
  std::stringstream str_stream;
  str_stream << file.rdbuf();
  return str_stream.str();
}
