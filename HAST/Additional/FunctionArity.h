#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_FUNCTIONARITY_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_FUNCTIONARITY_H

#include <vector>
#include "../../Lexer/TokenTree.h"

class FunctionArity {
public:
    std::vector<FunctionArity> argument_arity;
    int arity();
    static FunctionArity build_arity(const std::vector<TokenNode>& nodes);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_FUNCTIONARITY_H
