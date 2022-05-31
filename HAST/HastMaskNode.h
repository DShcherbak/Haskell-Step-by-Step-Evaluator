#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTMASKNODE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTMASKNODE_H


#include "HastNode.h"

#define HAST_MN std::shared_ptr<HastMaskNode>;
#define HAST_FULL_MASK std::pair<std::string, std::vector<std::shared_ptr<HastMaskNode>>>

class HastMaskNode : public HastNode{

};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTMASKNODE_H
