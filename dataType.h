#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_DATATYPE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_DATATYPE_H


#include <string>
#include <vector>

class dataType {
    std::string name;
    std::vector <std::pair<std::string, int>> definitions;
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_DATATYPE_H
