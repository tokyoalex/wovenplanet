#include <iostream>
#include "buggy.h"

int main() {
  int exitCode = 0;
  try {
      readInputWords(stdin);
      
      // Print the word list
      std::cout << "\n=== Word list:" << std::endl;

      for (const auto & [str, count]: wordsMap) {
        std::cout << str << " " << count << std::endl;
      }

      lookupWords(stdin);

      std::cout << "\n=== Total words found: " << wordsMap.size() << std::endl;
  }
  catch (std::exception & e) {
    exitCode = -1;
    std::cout << "error " << e.what() << std::endl;
  }
  return exitCode;
}

