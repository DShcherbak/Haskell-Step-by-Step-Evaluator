#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_PARSING_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_PARSING_H

#include <vector>
#include <string>

namespace parsing {

    std::string remove_spaces(const std::string &line);
    std::vector <std::string> split_by_space(const std::string &line);
    std::vector <std::string> split_by(const std::string &line, char separator);
    std::string remove_brackets(std::string line);

}
#endif //HASKELL_STEP_BY_STEP_EVALUATOR_PARSING_H
