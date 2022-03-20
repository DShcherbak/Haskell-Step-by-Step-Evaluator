#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLFUNCTION_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLFUNCTION_H

#include <string>
#include <vector>
#include "ExpressionElement.h"
#define bodies_vector std::vector<std::shared_ptr<ExpressionElement>>

using std::string;

class HaskellFunction {
public:
    string type_definition;
    string name;


private:
    bodies_vector masks;
    int number_of_params;

};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLFUNCTION_H
