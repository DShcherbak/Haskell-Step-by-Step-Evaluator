#include <map>
#include "parsing.h"


namespace parsing {

    std::vector <std::string> split_by_space(const std::string &line) {
        std::vector <std::string> result{};

        enum MaskParserState {Default, NewWord, SingleQuotes, DoubleQuotes};
        MaskParserState state;

        std::string line_element;
        int bracket_depth = 0;

        for (char current_symbol: line) {
            if (current_symbol == ' ') {
                if (state == NewWord) {
                    continue;
                } else if (state == Default) {
                    if (!line_element.empty())
                        result.push_back(line_element);
                    line_element = "";
                    state = NewWord;
                }
            } else {
                line_element += current_symbol;
                if(current_symbol == '"'){
                    if(state == DoubleQuotes)
                        state = Default;
                    else if(state == Default || state == NewWord)
                        state = DoubleQuotes;
                } else if (current_symbol == '\'') {
                    if(state == SingleQuotes)
                        state = Default;
                    else if(state == NewWord)
                        state = SingleQuotes;
                } else if (current_symbol == '(') {
                    if(state == Default || state == NewWord){
                        state = NewWord; // оце мб треба після оператора, що починається з ':'
                        ++bracket_depth;
                    }
                } else if (current_symbol == ')') {
                    if(state == Default || state == NewWord)
                        --bracket_depth;
                }
            }
         /*       //state = DoubleQuotes;
            if(state == Default) {
                if(bracket_depth == 0 && current_symbol == ' ') {
                    if(line_element.size() > 0)
                        result.push_back(line_element);
                    line_element = "";
                    state = NewWord;
                } else {
                    if(current_symbol == '"')
                        state = DoubleQuotes;
                    else {
                        if(current_symbol == '(')
                            bracket_depth++;
                        state = Default;
                    }
                }
            } else if(state == NewWord){
                if(current_symbol != ' ') {
                    line_element += current_symbol;
                    if (current_symbol == '"')
                        state = DoubleQuotes;
                    else if (current_symbol == '\'')
                        state = SingleQuotes;
                    else {
                        if (current_symbol == '(')
                            bracket_depth++;
                        state = Default;
                    }
                }
            } else if(state == SingleQuotes){
                line_element += current_symbol;
                if (current_symbol == '\'')
                    state = Default;
            } else if(state == DoubleQuotes){
                line_element += current_symbol;
                if(current_symbol == '"')
                    state = Default;
            }*/
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

