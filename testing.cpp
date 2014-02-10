// toupper example (C++)
#include <iostream>       // std::cout
#include <string>         // std::string
#include <locale>         // std::locale, std::toupper

int main ()
{
  std::locale loc;
  std::string str="Test$String12.\n";
  for (std::string::size_type i=0; i<str.length(); ++i)
    std::cout << std::toupper(str[i],loc);
  return 0;
}