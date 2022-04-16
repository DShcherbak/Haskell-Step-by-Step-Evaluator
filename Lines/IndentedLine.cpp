#include "IndentedLine.h"

lines::IndentedLine::IndentedLine(const std::string &ind_line, const std::string &orig_line, int ind) {
    parsed_line = ind_line;
    original_line = orig_line;
    indentation = ind;
}
