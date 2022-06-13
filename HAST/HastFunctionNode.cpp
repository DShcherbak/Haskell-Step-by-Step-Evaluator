#include "HastFunctionNode.h"

HastFunctionNode::HastFunctionNode() = default;

void HastFunctionNode::add_expression(HaskellModel& model, const std::pair<std::string, std::vector<std::shared_ptr<HastMaskNode>>>& mask,
                                      const TokenList &body) {
    auto mask_element = std::make_shared<function::Mask>();
    name = mask.first;
    mask_element->add_arg_templates(mask.second);
    mask_element->add_body(model, body);
    masks.emplace_back(mask_element);
}

void HastFunctionNode::add_expression_with_guards(HaskellModel& model, const std::pair<std::string, std::vector<std::shared_ptr<HastMaskNode>>>& mask,
                                                  const std::vector<std::pair<TokenList, TokenList>> &guards) {
    auto mask_element = std::make_shared<function::Mask>();
    name = mask.first;
    mask_element->add_arg_templates(mask.second);
    mask_element->add_guards(model, guards);
    masks.emplace_back(mask_element);
}


std::shared_ptr<HastNode> HastFunctionNode::build_expression(HaskellModel& model, const TokenNode& token){
    std::shared_ptr<HastNode> node = HastNodeFactory::create_node(1).get_node();
    if(token.which() == 0){
        auto tree = get<TokenTree>(token);
        node = build_expression_from_list(model, tree.children);
    } else {
        auto value_str = get<std::string>(token);
        node = HastNodeFactory::create_node(1).with_value(value_str).get_node();
    }
    return node;
}

std::vector<std::shared_ptr<HastNode>> HastFunctionNode::apply_all_functions(HaskellModel& model, const std::vector<std::shared_ptr<HastNode>>& nodes) {
    size_t i = 0, n = nodes.size();
    std::vector<std::shared_ptr<HastNode>> result;

    while(i < n){
        if(nodes[i]->type == HastNodeType::Variable && model.functions.contains(nodes[i]->value)){
            std::shared_ptr<HastNode> node = HastNodeFactory::create_node(model.functions[nodes[i]->value]).get_node();

            size_t arity = node->function_call->arity;
            while(arity > 0){
                ++i;
                --arity;
                node->function_call->arguments.push_back(nodes[i]);
            }
            result.emplace_back(node);
        } else {
            result.emplace_back(nodes[i]);
        }
        i++;
    }
    return result;
}

std::vector<std::shared_ptr<HastNode>> HastFunctionNode::apply_operators(HaskellModel& model, std::vector<std::shared_ptr<HastNode>>& nodes, size_t from , size_t to) {

    std::vector<std::shared_ptr<HastNode>> result;

    for(size_t precedence = to; precedence >= from; --precedence){
        std::vector<std::shared_ptr<HastNode>> result;
        int i = 0, n = (int) nodes.size();

        while(i < n){
            if((nodes[i]->type == HastNodeType::Operator || nodes[i]->type == HastNodeType::PrefixOperator)
               && model.operators.contains(nodes[i]->value)){
                auto node = model.operators[nodes[i]->value];
                if(node->precedence == precedence && node->associativity != OperatorAssociativity::Right){
                    if(nodes[i]->type == HastNodeType::Operator){
                        if (!result.empty()) {
                            node->first = result.back();
                            result.pop_back();
                        }
                        if (i < n - 1) node->rest = nodes[++i];
                        result.emplace_back(node);
                    } else {
                        if (i < n - 1) node->first = nodes[++i];
                        if (i < n - 1) node->rest = nodes[++i];
                        result.emplace_back(node);
                    }
                } else {
                    result.emplace_back(nodes[i]);
                }
            } else {
                result.emplace_back(nodes[i]);
            }
            i++;
        }

        nodes = result;
        n = (int) nodes.size();
        i = n - 1;
        result = {};

        while(i >= 0){
            if((nodes[i]->type == HastNodeType::Operator || nodes[i]->type == HastNodeType::PrefixOperator)
               && model.operators.contains(nodes[i]->value)){
                auto node = model.operators[nodes[i]->value];
                if(node->precedence == precedence && node->associativity != OperatorAssociativity::Left){
                    if(nodes[i]->type == HastNodeType::Operator){
                        if (!result.empty()) {
                            node->rest = result.back();
                            result.pop_back();
                        }
                        if (i > 0) node->first = nodes[--i];
                        result.emplace_back(node);
                    } else {
                        if (!result.empty()) {
                            node->first = result.back();
                            result.pop_back();
                        }
                        if (!result.empty()) {
                            node->rest = result.back();
                            result.pop_back();
                        }
                        result.emplace_back(node);
                    }
                } else {
                    result.emplace_back(nodes[i]);
                }
            } else {
                result.emplace_back(nodes[i]);
            }
            i--;
        }
        nodes = result;
        std::reverse(nodes.begin(), nodes.end());
    }
    return nodes;
}

std::vector<std::shared_ptr<HastNode>> HastFunctionNode::apply_declaration(const std::vector<std::shared_ptr<HastNode>>& nodes) {
    return nodes;
}

std::shared_ptr<HastNode> HastFunctionNode::build_expression_from_list(HaskellModel& model, const std::vector<TokenNode>& list){
    std::vector<std::shared_ptr<HastNode>> nodes;
    if(list.empty())
        return {};
    for(const auto & element : list){
        nodes.emplace_back(build_expression(model, element));
    }
    nodes = apply_all_functions(model, nodes);
    nodes = apply_data_constructors(model, nodes);
    nodes = apply_operators(model, nodes, 6, 9);
    nodes = apply_list_constructors(nodes);
    nodes = apply_operators(model,nodes, 1, 5);
    nodes = apply_declaration(nodes);
    if(nodes.size() == 2 && nodes[1]->value == "()"){
        nodes.pop_back();
    }
    assert(nodes.size() == 1);
    HastPrinter::print_node(nodes[0]);
    return nodes[0];
}

std::vector<std::shared_ptr<HastNode>> HastFunctionNode::apply_data_constructors(HaskellModel& model, const std::vector<std::shared_ptr<HastNode>>&nodes) {
    int i = 0, n = (int) nodes.size();
    if (n == 0)
        return {};
    std::vector<std::shared_ptr<HastNode>> result;
    if (nodes[0]->type == HastNodeType::InfixDataConstructor) {//n == 1
        nodes[0]->type = HastNodeType::DataConstructor;
        return {nodes.begin(), nodes.begin() + 1};
    }
    while (i < n) {
        if (nodes[i]->type == HastNodeType::DataConstructor) {
            if (model.data_constructor_arity.contains(nodes[i]->value)) {
                int arity = model.data_constructor_arity[nodes[i]->value];
                auto node = HastNodeFactory::create_node(1).with_value(nodes[i]->value).get_node();
                result.emplace_back(node);
                while (arity > 0 && i < n - 1) {
                    ++i;
                    node->first = nodes[i];
                    --arity;
                    if (arity > 0) {
                        node->rest = HastNodeFactory::create_node(1).with_type(
                                HastNodeType::DataConstructor).get_node();
                        node = node->rest;
                    }
                }
            } else if (model.infix_data_constructors.contains(nodes[i]->value)) {
                auto node = HastNodeFactory::create_node(1).with_value(nodes[i]->value).get_node();
                result.emplace_back(node);
                if (i < n - 1) node->first = nodes[++i];
                if (i < n - 1) node->rest = nodes[i + 1];
            } else {
                throw CustomException("Unknown data constructor: " + nodes[i]->value);
            }
        } else if (nodes[i]->type == HastNodeType::InfixDataConstructor && nodes[i]->value != ":") {
            if (model.infix_data_constructors.contains(nodes[i]->value)) {
                auto node = HastNodeFactory::create_node(1).with_value(nodes[i]->value).get_node();
                if (!result.empty()) {
                    node->first = result.back();
                    result.pop_back();
                }
                if (i < n - 1) node->rest = nodes[++i];
                result.emplace_back(node);
            } else {
                throw CustomException("Unknown data constructor: " + nodes[i]->value);
            }
        } else {
            result.emplace_back(nodes[i]);
        }
        i++;
    }
    return result;
}

std::vector<std::shared_ptr<HastNode>> HastFunctionNode::apply_list_constructors(const std::vector<std::shared_ptr<HastNode>>&nodes) {
    //special case for desugared lists, since they are not completely desugared, as it turns out
    std::vector<HAST_N> result;
    int i = (int) nodes.size()-1;
    while(i >= 0) {
        if(nodes[i]->value == ":") {
            auto node = HastNodeFactory::create_node(1).with_type(HastNodeType::List).get_node();
            if(nodes[i]->type == HastNodeType::InfixDataConstructor) {
                if(!result.empty()){
                    node->rest = result.back();
                    result.pop_back();
                }
                if(i > 0) node->first = nodes[--i];
            } else {
                if(!result.empty()){
                    node->first = result.back();
                    result.pop_back();
                }
                if(!result.empty()){
                    node->rest = result.back();
                    result.pop_back();
                }
            }
            result.emplace_back(node);
        } else {
            result.emplace_back(nodes[i]);
        }
        --i;
    }
    std::vector<std::shared_ptr<HastNode>> parsed_sugared_list;
    std::reverse(result.begin(), result.end());
    auto type = HastNodeType::List;
    i = (int) result.size()-1;
    if(result[i]->value == "[]"){
        type = HastNodeType::List;
    }
    else if (result[i]->value == "()"){
        type = HastNodeType::Tuple;
    } else {
        return result;
    }
    i--;
    while(i >= 0) {
        if(result[i]->value == ",") {
            auto node = HastNodeFactory::create_node(1).with_type(type).get_node();
            if(result[i]->type == HastNodeType::InfixDataConstructor) {
                if(!result.empty()){
                    node->rest = result.back();
                    parsed_sugared_list.pop_back();
                }
                if(i > 0) node->first = result[--i];
            } else {
                if(!parsed_sugared_list.empty()){
                    node->first = result.back();
                    parsed_sugared_list.pop_back();
                }
                if(!parsed_sugared_list.empty()){
                    node->rest = parsed_sugared_list.back();
                    parsed_sugared_list.pop_back();
                }
            }
            parsed_sugared_list.emplace_back(node);
        } else {
            parsed_sugared_list.emplace_back(result[i]);
        }
        --i;
    }
    std::reverse(parsed_sugared_list.begin(), parsed_sugared_list.end());
    return parsed_sugared_list;
}

