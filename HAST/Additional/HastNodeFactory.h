#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTNODEFACTORY_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTNODEFACTORY_H

class HastFunctionCallNode;

#include <memory>
#include "../HastNode.h"
#include "../HastMaskNode.h"
#include "../HastOperatorNode.h"
#include "../HastFunctionNode.h"
#include "../HastFunctionCallNode.h"

class HastNodeFactory {
public:
    std::shared_ptr<HastNode> node;
    std::shared_ptr<HastFunctionCallNode> func_node;
    std::shared_ptr<HastNode> get_node() const;

    HastNodeFactory with_type(HastNodeType type);
    HastNodeFactory with_value(const std::string& val);

    static HastNodeFactory create_node(int type);
    static HastNodeFactory create_node(std::shared_ptr<HastFunctionNode> node);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTNODEFACTORY_H
