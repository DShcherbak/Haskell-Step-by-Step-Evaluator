#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTNODEFACTORY_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTNODEFACTORY_H


#include <memory>
#include "HastNode.h"
#include "HastMaskNode.h"
#include "HastOperatorNode.h"
#include "HastFunctionNode.h"

class HastNodeFactory {
private:
    std::shared_ptr<HastNode> node;
public:
    std::shared_ptr<HastNode> get_node();
    static HastNodeFactory create_node(int type);
    HastNodeFactory with_type(HastNodeType type);
    HastNodeFactory with_value(const std::string& val);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTNODEFACTORY_H
