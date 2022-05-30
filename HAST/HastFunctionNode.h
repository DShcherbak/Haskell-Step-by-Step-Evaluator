#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTFUNCTIONNODE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTFUNCTIONNODE_H

#include <map>
#include "HastNode.h"
#include "HastMaskNode.h"

#define HAST_FN HastFunctionNode

class HastFunctionNode : HastNode {
public:
    size_t number_of_arguments = 0;
    bool has_guards = false;
    std::vector<HastNode> arguments;
    std::map<HastMaskNode, std::map<HastNode, HastNode>> guards;
    std::map<HastMaskNode, HastNode> function_definitions;
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTFUNCTIONNODE_H
