#include <iostream>
#include <map>
#include "Mask.h"
#include "../../HAST/Additional/HastNodeFactory.h"

std::shared_ptr<HastNode> apply_variables(const std::shared_ptr<HastNode>& body_node, std::map<std::string, HAST_N> mask_declarations){
    if(body_node == nullptr){
        return body_node;
    }
    if(body_node->type == HastNodeType::Variable){
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
    std::map<std::string, HAST_N> mask_declarations;
    for(int i = 0, n = (int) arguments.size(); i < n; i++){
        std::pair<bool, std::map<std::string, HAST_N>> mask_result = argument_templates[i]->fits(arguments[i]);
        if(!mask_result.first)
            return nullptr;
        for(auto const & mask_variable : mask_result.second){
            mask_declarations.insert(mask_variable);
        }
    }
    if(this->body_node != nullptr){
        return apply_variables(body_node, mask_declarations);
    } else { //guards
        for(auto & guard_pairs : this->guards){
            auto guard_condition = guard_pairs.first;
            guard_condition = apply_variables(guard_condition, mask_declarations);
            auto guard_result = guard_condition->compute_fully();
            if(guard_result->value == "True")
                return apply_variables(guard_pairs.second, mask_declarations);

        }
    }
    return nullptr;
}

void function::Mask::add_body(const std::shared_ptr<HastNode> &body) {
    body_node = body;
}

void function::Mask::add_guards(const  std::vector<std::pair<HAST_N, HAST_N>>& guards) {
    this->guards = guards;
}

void function::Mask::add_arg_templates(const std::vector<std::shared_ptr<HastNode>> &arg_templates) {
    this->argument_templates = arg_templates;
}
