#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_EXPRESSIONMASK_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_EXPRESSIONMASK_H

#include <vector>

class MaskElement{

};

class ExpressionMask {
    std::vector<MaskElement> elements;

    bool fits();
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_EXPRESSIONMASK_H
