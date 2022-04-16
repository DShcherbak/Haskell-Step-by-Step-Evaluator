#include "LineStatement.h"

lines::LineStatement::LineStatement(const std::string &s) {
    line = s;
    where_clause = vector<string>();
}

void lines::LineStatement::append(const std::string &new_line) {
    line += " " + new_line;
}

void lines::LineStatement::append_to_original(const std::string &new_line) {
    original_line += "\n" + new_line;
}

void lines::LineStatement::append_both(const std::string &new_line, const std::string &originalLine) {
    append(new_line);
    append_to_original(originalLine);
}

void lines::LineStatement::append_to_where(const std::string &new_line) {
    where_clause[where_clause.size()-1] += " " + new_line;
}

lines::LineStatement::LineStatement(){
    line = "";
    where_clause = vector<string>();
}

