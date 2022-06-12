#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTFUNCTIONNODE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTFUNCTIONNODE_H

class HaskellModel;

#include <map>
#include "HastNode.h"
#include "HastMaskNode.h"
#include "../Model/HaskellFunction/Mask.h"
#include "../Model/HaskellModel.h"

#define HAST_FN std::shared_ptr<HastFunctionNode>
#define HAST_GUARD std::pair<HAST_N, HAST_N>

class HastFunctionNode : public HastNode {
public:
    size_t number_of_arguments = 0;
    std::vector<HAST_N> arguments;
    std::vector<std::shared_ptr<function::Mask>> masks;

    explicit HastFunctionNode();
    void add_expression(HaskellModel& model,
                        const std::pair<std::string, std::vector<std::shared_ptr<HastMaskNode>>> &mask,
                        const std::vector<TokenNode> &body);

    void add_expression_with_guards(HaskellModel& model,
                                    const std::pair<std::string, std::vector<std::shared_ptr<HastMaskNode>>> &mask,
                                    const std::vector<std::pair<std::vector<TokenNode>, std::vector<TokenNode>>> &guards);

    static std::shared_ptr<HastNode> build_expression_from_list(HaskellModel& model, const std::vector<TokenNode> &list);

    static std::shared_ptr<HastNode> build_expression(HaskellModel& model, const TokenNode &token);

    static std::vector<std::shared_ptr<HastNode>> apply_all_functions(HaskellModel& model, const std::vector<std::shared_ptr<HastNode>> &list);

    static std::vector<std::shared_ptr<HastNode>>
    apply_operators(HaskellModel& model, std::vector<std::shared_ptr<HastNode>> &nodes, size_t from, size_t to);

    static std::vector<std::shared_ptr<HastNode>> apply_data_constructors(HaskellModel& model,const std::vector<std::shared_ptr<HastNode>>&nodes);
    static std::vector<std::shared_ptr<HastNode>> apply_list_constructors(const std::vector<std::shared_ptr<HastNode>>&nodes);

    static std::vector<std::shared_ptr<HastNode>> apply_declaration(const std::vector<std::shared_ptr<HastNode>> &nodes);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTFUNCTIONNODE_H
