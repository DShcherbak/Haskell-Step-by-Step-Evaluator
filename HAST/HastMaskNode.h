#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTMASKNODE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTMASKNODE_H


#include "HastNode.h"

class HastMaskNode : public HastNode{
public:
    std::pair<bool, std::map<std::string, HAST_N>> fits(const HAST_N& argument);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTMASKNODE_H
