#include "HastFunctionNode.h"

HastFunctionNode::HastFunctionNode() = default;



void HastFunctionNode::add_expression(HaskellModel& model, const std::pair<std::string, std::vector<std::shared_ptr<HastMaskNode>>>& mask,
                                      const TokenList &body) {
    auto mask_element = std::make_shared<function::Mask>();
    name = mask.first;
    mask_element->add_arg_templates(mask.second, arity);
    mask_element->add_body(model, body);
    masks.emplace_back(mask_element);
}

void HastFunctionNode::add_expression_with_guards(HaskellModel& model, const std::pair<std::string, std::vector<std::shared_ptr<HastMaskNode>>>& mask,
                                                  const std::vector<std::pair<TokenList, TokenList>> &guards) {
    auto mask_element = std::make_shared<function::Mask>();
    name = mask.first;
    mask_element->add_arg_templates(mask.second, arity);
    mask_element->add_guards(model, guards);
    masks.emplace_back(mask_element);
}

std::shared_ptr<HastNode> HastFunctionNode::apply(const std::vector<std::shared_ptr<HastNode>>& args) {
    for(auto & mask : masks){
        auto match_mask = mask->fits(args);
        if(match_mask.first){
            return nullptr;
        }
    }
}

std::shared_ptr<HastNode> HastFunctionNode::build_expression(HaskellModel& model, const TokenNode& token, int type, const std::map<std::string, int> &function_vars){
    std::shared_ptr<HastNode> node = HastNodeFactory::create_node(type).get_node();
    if(token.which() == 0){
        auto tree = get<TokenTree>(token);
        node = build_expression_from_list(model, tree.children, type, function_vars);
    } else {
        auto value_str = get<std::string>(token);
        node = HastNodeFactory::create_node(type).with_value(value_str).get_node();
    }
    return node;
}

std::vector<std::shared_ptr<HastNode>>
HastFunctionNode::apply_all_functions(HaskellModel &model, const std::vector<std::shared_ptr<HastNode>> &nodes,
                                      std::map<std::string, int> function_vars) {
    size_t i = 0, n = nodes.size();
    std::vector<std::shared_ptr<HastNode>> result;

    while(i < n){
        if(nodes[i]->type == HastNodeType::Variable) {
            std::shared_ptr<HastNode> node;
            if (model.functions.contains(nodes[i]->value))
                node = HastNodeFactory::create_node(model.functions[nodes[i]->value]).get_node();
            else if (function_vars.contains(nodes[i]->value)){
                node = std::make_shared<HastNode>();
                node->type = HastNodeType::FunctionCall;
                node->function_call = std::make_shared<HastFunctionCallNode>();
                node->function_call->arity = function_vars[nodes[i]->value];
            } else {
                result.emplace_back(nodes[i]);
                ++i;
                continue;
            }
            size_t arity = node->function_call->arity;
            while (arity > 0) {
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

std::shared_ptr<HastNode> HastFunctionNode::build_expression_from_list(HaskellModel& model, const std::vector<TokenNode>& list, int type, const std::map<std::string, int> &function_vars){
    std::vector<std::shared_ptr<HastNode>> nodes;
    if(list.empty())
        return nullptr;
    for(const auto & element : list){
        nodes.emplace_back(build_expression(model, element, type, function_vars));
    }
    nodes = apply_all_functions(model, nodes, function_vars);
    nodes = apply_data_constructors(model, nodes, type);
    nodes = apply_operators(model, nodes, 6, 9);
    nodes = apply_list_constructors(nodes, type);
    nodes = apply_operators(model,nodes, 1, 5);
    nodes = apply_declaration(nodes);
    if(nodes.size() == 2 && nodes[1]->value == "()"){
        nodes.pop_back();
    }
    assert(nodes.size() == 1);
    //HastPrinter::print_node(nodes[0]);
    return nodes[0];
}

std::vector<std::shared_ptr<HastNode>> HastFunctionNode::apply_data_constructors(HaskellModel& model, const std::vector<std::shared_ptr<HastNode>>&nodes, int type) {
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
                auto node = HastNodeFactory::create_node(type).with_value(nodes[i]->value).get_node();
                result.emplace_back(node);
                while (arity > 0 && i < n - 1) {
                    ++i;
                    node->first = nodes[i];
                    --arity;
                    if (arity > 0) {
                        node->rest = HastNodeFactory::create_node(type).with_type(
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
                auto node = HastNodeFactory::create_node(type).with_value(nodes[i]->value).get_node();
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

void add_an_empty_list_in_the_end(std::shared_ptr<HastNode> node, bool list){
    auto cur = node;
    while(cur && cur->rest && (cur->rest->type == HastNodeType::List || cur->rest->type == HastNodeType::Tuple)){
        cur = cur->rest;
    }
    std::shared_ptr<HastNode> new_node;
    if(cur->rest && cur->type == HastNodeType::List){
        new_node = HastNodeFactory::create_node(0).with_type(HastNodeType::List).get_node();
        new_node->rest = HastNodeFactory::create_node(0).with_value("[]").get_node();
    } else if (cur->rest && cur->type == HastNodeType::Tuple){
        new_node = HastNodeFactory::create_node(0).with_type(HastNodeType::Tuple).get_node();
    }
    new_node->first = cur->rest;
    cur->rest = new_node;
}

std::vector<std::shared_ptr<HastNode>> HastFunctionNode::apply_list_constructors(const std::vector<std::shared_ptr<HastNode>>&nodes, int type) {
    //special case for desugared lists, since they are not completely desugared, as it turns out
    std::vector<HAST_N> result;
    int i = (int) nodes.size()-1;
    while(i >= 0) {
        if(nodes[i]->value == ":") {
            auto node = HastNodeFactory::create_node(type).with_type(HastNodeType::List).get_node();
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
    auto node_type = HastNodeType::List;
    i = (int) result.size()-1;
    if(result.size() <= 1)
        return result;
    if(result[i]->value == "[]"){
        node_type = HastNodeType::List;
    }
    else if (result[i]->value == "()"){
        node_type = HastNodeType::Tuple;
    } else {
        return result;
    }
    i--;
    bool has_comma = false;
    while(i >= 0) {
        if(result[i]->value == ",") {
            has_comma = true;
            auto node = HastNodeFactory::create_node(type).with_type(node_type).get_node();
            if(result[i]->type == HastNodeType::Comma) {
                if(!parsed_sugared_list.empty()){
                    node->rest = parsed_sugared_list.back();
                    parsed_sugared_list.pop_back();
                }
                if(i > 0) node->first = result[--i];
            } else {
                if(!parsed_sugared_list.empty()){
                    node->first = parsed_sugared_list.back();
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
    if(has_comma)
        add_an_empty_list_in_the_end(parsed_sugared_list[0], node_type == HastNodeType::List);

    std::reverse(parsed_sugared_list.begin(), parsed_sugared_list.end());
    return parsed_sugared_list;
}

