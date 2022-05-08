#include <map>
#include <set>
#include "parsing.h"


namespace parsing {

    std::vector <std::string> split_by(const std::string &line, char separator) {
        std::vector <std::string> result{};
        std::string line_element;
        for (char current_symbol: line) {
            if(current_symbol != separator){
                line_element += current_symbol;
            } else {
                if (!line_element.empty())
                    result.push_back(line_element);
                line_element = "";
            }
        }
        if(!line_element.empty())
            result.push_back(line_element);
        return result;

    }


    std::string remove_brackets(std::string line) {
        while (line.starts_with('(') && line.ends_with(')')) {
            line = line.substr(1, line.size() - 1);
        }
        return line;
    }

    std::string remove_spaces(const std::string &line){
        std::string no_spaces;
        for(char c : line){
            if(c != ' ')
                no_spaces += c;
        }
        return no_spaces;
    }
}

