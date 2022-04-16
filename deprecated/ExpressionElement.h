//
// Created by denys on 12.03.2022.
//

#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_EXPRESSIONELEMENT_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_EXPRESSIONELEMENT_H


#include <memory>

class ExpressionElement {
    std::shared_ptr<ExpressionElement> next;
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_EXPRESSIONELEMENT_H
