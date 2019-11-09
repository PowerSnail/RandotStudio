#pragma once

#include <iostream>
#include <QString>

namespace logging {

constexpr int ERROR = 0;
constexpr int INFO = 10;
constexpr int DEBUG = 50;

std::ostream& operator<<(std::ostream& stream, QString value);

template <typename T>
void printList(T arg) {
  std::cerr << arg;
}

template <typename T, typename... Types> 
void printList(T firstArg, Types... args) {
  std::cerr << firstArg << " ";
  printList(args...);
}

template <typename... Types>
void logError(Types... args) {
    std::cerr << "ERROR: ";
    printList(args...);
    std::cerr << std::endl;
}

template <typename... Types>
void logDebug(Types... args) {
    std::cerr << "DEBUG: ";
    printList(args...);
    std::cerr << std::endl;
}

}
