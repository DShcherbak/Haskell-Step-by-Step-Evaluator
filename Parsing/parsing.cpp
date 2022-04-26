#include "parsing.h"


namespace parsing {

    std::vector <std::string> split_by_space(const std::string &line) {
        std::vector <std::string> result{};

        int bracket_depth = 0;
        std::string line_element;
        for (char current_symbol: line) {
            if (current_symbol == ' ' && bracket_depth == 0) {
                result.emplace_back(remove_brackets(line_element));
                line_element = "";
            } else {
                line_element.append(1u, current_symbol);
                if (current_symbol == '(') {
                    bracket_depth += 1;
                } else if (current_symbol == ')') {
                    bracket_depth -= 1;
                }
            }
        }
    }

    std::string remove_brackets(std::string line) {
        while (line.starts_with('(') && line.ends_with(')')) {
            line = line.substr(1, line.size() - 1);
        }
        return line;
    }

    std::string remove_spaces(const std::string &line){
        std::string no_spaces;

    }
}

