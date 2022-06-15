#include <iostream>
#include <map>
#include "Mask.h"
#include "../../HAST/Additional/HastNodeFactory.h"

std::shared_ptr<HastNode>
apply_variables(const std::shared_ptr<HastNode> &body_node, std::map<std::string, HAST_N > mask_declarations) {
    if (body_node == nullptr) {
        return body_node;
    }
    if (body_node->type == HastNodeType::Variable) {
        assert(mask_declarations.contains(body_node->value));
        return mask_declarations[body_node->value];
    } else {
        auto result = HastNodeFactory::create_node(0).with_value(body_node->value).get_node();
        result->first = apply_variables(body_node->first, mask_declarations);
        result->rest = apply_variables(body_node->rest, mask_declarations);
        return result;
    }
}

std::shared_ptr<HastNode> function::Mask::apply_args(const std::vector<std::shared_ptr<HastNode>> &arguments) {
    assert(arguments.size() == argument_templates.size());
    std::map <std::string, HAST_N> mask_declarations;
    for (int i = 0, n = (int) arguments.size(); i < n; i++) {
        std::pair<bool, std::map<std::string, HAST_N>> mask_result = {false, {}};//argument_templates[i]->fits(arguments[i]);
        if (!mask_result.first)
            return nullptr;
        for (auto const &mask_variable: mask_result.second) {
            mask_declarations.insert(mask_variable);
        }
    }
    if (this->body_node != nullptr) {
        return apply_variables(body_node, mask_declarations);
    } else { //guards
        for (auto &guard_pairs: this->guards) {
            auto guard_condition = guard_pairs.first;
            guard_condition = apply_variables(guard_condition, mask_declarations);
            auto guard_result = guard_condition->compute_fully();
            if (guard_result->value == "True" || guard_result->type == HastNodeType::DataConstructor)
                return apply_variables(guard_pairs.second, mask_declarations);
        }
    }
    return nullptr;
}

void function::Mask::add_body(HaskellModel& model, const std::vector<TokenNode> &body) {
    body_node = HastFunctionNode::build_expression_from_list(model, body, 0, function_variables);
}

void function::Mask::add_guards(HaskellModel& model, const std::vector<std::pair<TokenList, TokenList>> &guards) {
    for (auto const &guard_pair: guards) {
        this->guards.push_back({HastFunctionNode::build_expression_from_list(model, guard_pair.first, 0, function_variables),
                                HastFunctionNode::build_expression_from_list(model, guard_pair.second, 0, function_variables)});
    }
}

std::map<std::string, int> find_function_variables(std::vector<std::shared_ptr<HastMaskNode>> masks, FunctionArity arity){
    std::map<std::string, int> result;
    int i = 0;
    for(auto & mask : masks){
        if(mask->type == HastNodeType::Variable && arity.argument_arity.size() > i && arity.argument_arity[i].arity() > 0){
            result.insert({mask->value, arity.argument_arity[i].arity()});
        }
        i++;
    }
    return result;
}

void function::Mask::add_arg_templates(const std::vector<std::shared_ptr<HastMaskNode>> &arg_templates, FunctionArity& arity) {
    argument_templates = arg_templates;
    function_variables = find_function_variables(arg_templates, arity);
}

std::pair<bool, std::map<std::string, HAST_N>> function::Mask::fits(const std::vector<std::shared_ptr<HastNode>> &arguments) {
    size_t n = arguments.size();
    assert(argument_templates.size() == n);
    std::map<std::string, HAST_N> variable_map;
    for(size_t i = 0; i < n; i++){
        auto match_arg = argument_templates[i]->fits(arguments[i]);
        if(!match_arg.first)
            return {false, {}};
        for(auto & pair : match_arg.second){
            variable_map.insert(pair);
        }
    }
    return {true, variable_map};
}
