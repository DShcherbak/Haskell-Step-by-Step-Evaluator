#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTOPERATORNODE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTOPERATORNODE_H

#include "HastNode.h"

#define HAST_ON HastOperatorNode

enum class OperatorAssociativity {Left,Right,None};

class HastOperatorNode : public HastNode {
public:
    int precedence = 0;
    OperatorAssociativity associativity = OperatorAssociativity::Left;
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTOPERATORNODE_H
