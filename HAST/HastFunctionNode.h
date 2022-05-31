#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTFUNCTIONNODE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTFUNCTIONNODE_H

#include <map>
#include "HastNode.h"
#include "HastMaskNode.h"

#define HAST_FN std::shared_ptr<HastFunctionNode>
#define HAST_GUARD std::pair<HAST_N, HAST_N>

class HastFunctionNode : HastNode {
public:
    size_t number_of_arguments = 0;
    bool has_guards = false;
    std::vector<HastNode> arguments;
    std::map<HastMaskNode, std::map<HastNode, HastNode>> guards;
    std::map<HastMaskNode, HastNode> function_definitions;

    explicit HastFunctionNode();

    void add_expression(HAST_FULL_MASK mask, HAST_N body);
    void add_expression_with_guards(HAST_FULL_MASK mask, std::vector<HAST_GUARD> guards);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTFUNCTIONNODE_H
