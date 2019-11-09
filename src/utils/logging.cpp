#include "logging.h"

std::ostream& logging::operator<<(std::ostream& stream, QString value) {
   stream << value.toStdString();
   return stream; 
}
