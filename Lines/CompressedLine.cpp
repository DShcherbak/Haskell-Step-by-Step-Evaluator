#include "CompressedLine.h"

lines::CompressedLine::CompressedLine(const std::string &s) {
    line = s;
}

void lines::CompressedLine::append(const std::string &new_line) {
    line += " " + new_line;
}

void lines::CompressedLine::append_to_where(const std::string &new_line) {
    where_clause[where_clause.size()-1] += " " + new_line;
}
