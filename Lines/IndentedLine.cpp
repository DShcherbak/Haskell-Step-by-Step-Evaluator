#include "IndentedLine.h"

lines::IndentedLine::IndentedLine(const std::string &ind_line, const std::string &orig_line, int ind) {
    parsed_line = ind_line;
    original_line = orig_line;
    indentation = ind;
}

void lines::IndentedLine::insert_at(char c, size_t position, bool remove) {
    auto first_part = parsed_line.substr(0, position);
    auto second_part = parsed_line.substr(position + (remove ? 1 : 0));
    parsed_line = first_part + c + second_part;
    --indentation;
}
