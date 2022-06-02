#include "HaskellModel.h"
#include "../HAST/Additional/HastPrinter.h"

void HaskellModel::add_statements(std::vector<string> &statements) {
    auto token_tree_vector = Lexer::functions_to_tokens(statements);
    token_tree_vector = process_headers(token_tree_vector);
    token_tree_vector = process_data_types(token_tree_vector);
    token_tree_vector = process_type_classes(token_tree_vector);
    //operators and function type definitions
    process_functions(token_tree_vector);
    std::cout << functions.size() << std::endl;
    std::cout << functions.size() << std::endl;
}

HAST_FN HaskellModel::get_or_create_function(const string &name) {
    if(!functions.contains(name)){
        HAST_FN func_node = std::make_shared<HastFunctionNode>();
        functions.insert({name, func_node});
    }
    return functions[name];
}

size_t find_next_token(const TokenList& list_to_scan, const std::string& token_to_find, size_t start = 0){
    size_t i = start, n = list_to_scan.size();
    while(i < n){
        if(list_to_scan[i].which() == 1){
            string token_value = get<std::string>(list_to_scan[i]);
            if(token_value == token_to_find)
                return i;
        }
        i++;
    }
    return i;
}

std::tuple<TokenList, GuardVector> process_guards(const TokenTree& tree){
    auto list_begin = tree.children.begin(), list_end = tree.children.end();
    size_t guard_position, eq_position, next_guard_position, n = tree.children.size();

    guard_position = find_next_token(tree.children, "|");
    eq_position = find_next_token(tree.children, "=");
    if(eq_position == n)
        return {{}, {}};
    if(guard_position == n){
        TokenList mask = TokenList(list_begin, list_begin+eq_position);
        TokenList body = TokenList(list_begin+eq_position+1, list_end);
        return std::tuple<TokenList, GuardVector>(mask, {{{},body}});
    }
    TokenList mask = TokenList(list_begin, list_begin+guard_position);
    auto result = std::tuple<TokenList, GuardVector>(mask, {});
    next_guard_position = find_next_token(tree.children, "|", guard_position+1);
    while(guard_position < n){
        TokenList guard_expression = TokenList(list_begin+guard_position+1, list_begin+eq_position);
        TokenList guard_body = TokenList(list_begin+eq_position+1, list_begin+next_guard_position);
        std::get<1>(result).emplace_back(std::pair<TokenList, TokenList>(guard_position, guard_body));
        guard_position = next_guard_position;
        eq_position = find_next_token(tree.children, "=", guard_position+1);
        next_guard_position = find_next_token(tree.children, "|", guard_position+1);
    }
    return result;
}

std::vector<std::tuple<TokenList, GuardVector>> HaskellModel::add_function_arity(const std::vector<TokenTree>& trees) {
    std::vector<std::tuple<TokenList, GuardVector>> result;
    for(auto & tree : trees){
        auto processed_guards = process_guards(tree);
        result.emplace_back(processed_guards);
        auto function_mask = std::get<0>(processed_guards);
        if(function_mask.empty() || function_mask[0].which() == 0)
            continue;
        std::string function_name = get<std::string>(function_mask[0]);
        HAST_FN fn = get_or_create_function(function_name);
        fn->number_of_arguments = function_mask.size()-1;
    }
    for(const auto& f : functions){
        std::cout << f.first << " : " << f.second->number_of_arguments << std::endl;
    }
    std::cout << "OPERATORS: \n";
    for(const auto& f : operators){
        std::cout << f.second->value << " : " << f.second->precedence << std::endl;
    }
    return result;
}


std::shared_ptr<HastNode> HaskellModel::make_mask(const TokenNode& token){
    std::shared_ptr<HastNode> node = HastNodeFactory::create_node(1).get_node();
    if(token.which() == 0){
        auto tree = get<TokenTree>(token);
        node = make_mask(tree);
    } else {
        auto value_str = get<std::string>(token);
        node = HastNodeFactory::create_node(1).with_value(value_str).get_node();
    }
    return node;
}

std::vector<std::shared_ptr<HastNode>> HaskellModel::apply_data_constructors(std::vector<std::shared_ptr<HastNode>>& list) {
    int i = 0, n = (int) list.size();
    if (n == 0)
        return {};
    std::vector<std::shared_ptr<HastNode>> result;
    if (list[0]->type == HastNodeType::InfixDataConstructor) {//n == 1
        list[0]->type = HastNodeType::DataConstructor;
        return {list.begin(), list.begin() + 1};
    }
    while (i < n) {
        if (list[i]->type == HastNodeType::DataConstructor) {
            if (data_constructor_arity.contains(list[i]->value)) {
                int arity = data_constructor_arity[list[i]->value];
                auto node = HastNodeFactory::create_node(1).with_value(list[i]->value).get_node();
                result.emplace_back(node);
                while (arity > 0 && i < n - 1) {
                    ++i;
                    node->first = list[i];
                    --arity;
                    if (arity > 0) {
                        node->rest = HastNodeFactory::create_node(1).with_type(
                                HastNodeType::DataConstructor).get_node();
                        node = node->rest;
                    }
                }
            } else if (infix_data_constructors.contains(list[i]->value)) {
                auto node = HastNodeFactory::create_node(1).with_value(list[i]->value).get_node();
                result.emplace_back(node);
                if (i < n - 1) node->first = list[++i];
                if (i < n - 1) node->rest = list[i + 1];
            } else {
                throw CustomException("Unknown data constructor: " + list[i]->value);
            }
        } else if (list[i]->type == HastNodeType::InfixDataConstructor && list[i]->value != ":") {
            if (infix_data_constructors.contains(list[i]->value)) {
                auto node = HastNodeFactory::create_node(1).with_value(list[i]->value).get_node();
                if (!result.empty()) {
                    node->first = result.back();
                    result.pop_back();
                }
                if (i < n - 1) node->rest = list[++i];
                result.emplace_back(node);
            } else {
                throw CustomException("Unknown data constructor: " + list[i]->value);
            }
        } else {
            result.emplace_back(list[i]);
        }
        i++;
    }
    return result;
}
std::vector<std::shared_ptr<HastNode>> HaskellModel::apply_list_constructors(std::vector<std::shared_ptr<HastNode>>& list) {
    //special case for desugared lists, since they are not completely desugared, as it turns out
    std::vector<HAST_N> result;
    int i = (int) list.size()-1;
    while(i >= 0){
        if(list[i]->value == ":"){
            auto node = HastNodeFactory::create_node(1).with_type(HastNodeType::List).get_node();
            if(list[i]->type == HastNodeType::InfixDataConstructor){
                if(!result.empty()){
                    node->rest = result.back();
                    result.pop_back();
                }
                if(i > 0) node->first = list[--i];
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
            result.emplace_back(list[i]);
        }
        --i;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::shared_ptr<HastNode> HaskellModel::make_mask(const TokenTree& tree){
    std::shared_ptr<HastNode> node = HastNodeFactory::create_node(1).get_node();
    std::vector<std::shared_ptr<HastNode>> elements;
    for(auto const& elem : std::vector(tree.children.begin(), tree.children.end()-1)){
        elements.emplace_back(make_mask(elem));
    }
    elements = apply_data_constructors(elements);
    elements = apply_list_constructors(elements);
    std::string recursion_type = get<std::string>(tree.children[tree.children.size()-1]);

    if(elements.empty()){
        node->set_value(recursion_type);
        return node;
    }

    if(recursion_type == "[]"){
        node->type = HastNodeType::List;
        std::shared_ptr<HastNode> cur = node;
        cur->first = elements[0];
        size_t i = 1, n = elements.size();
        while(i < n){
            if(elements[i]->value != ",")
                throw CustomException("Wrong list parsing in mask definition: \"" + elements[i]->value + "\" should have been a ','");
            i++;
            cur->rest = HastNodeFactory::create_node(1).with_type(HastNodeType::List).get_node();
            cur = cur->rest;
            cur->first = elements[i];
            i++;
        }
        cur->rest = HastNodeFactory::create_node(1).with_value("[]").get_node();
    } else { //it is a tuple cause a mask can't have a {}
        std::shared_ptr<HastNode> cur = node;
        cur->first = elements[0];
        size_t i = 1, n = elements.size();
        if(n == 1){
            return elements[0];
        }

        if(elements[1]->value == ","){
            node->type = HastNodeType::Tuple;
            while(i < n){
                if(elements[i]->value != ",")
                    throw CustomException("Wrong list parsing in mask definition: \"" + elements[i]->value + "\" should have been a ','");
                i++;
                cur->rest = HastNodeFactory::create_node(1).with_type(HastNodeType::Tuple).get_node();
                cur->rest->parent = cur;
                cur = cur->rest;
                cur->first = elements[i];
                i++;
            }
        } else {
            throw CustomException("Error while parsing mask: \"" + elements[1]->value + "\" should have been a ','");
        }
    }
    return node;
}

HAST_FULL_MASK HaskellModel::build_full_function_mask(const TokenList& list){
    std::pair<std::string, std::vector<std::shared_ptr<HastNode>>> result;
    if(list.empty())
        return result;
    result.first = get<std::string>(list[0]);
    for(size_t i = 1, n = list.size(); i < n; i++){
        result.second.emplace_back(make_mask(list[i]));
    }
    return result;
}

std::shared_ptr<HastNode> HaskellModel::build_expression(const TokenNode& token){
    std::shared_ptr<HastNode> node = HastNodeFactory::create_node(1).get_node();
    if(token.which() == 0){
        auto tree = get<TokenTree>(token);
        node = build_expression(tree.children);
    } else {
        auto value_str = get<std::string>(token);
        node = HastNodeFactory::create_node(1).with_value(value_str).get_node();
    }
    return node;
}

std::vector<std::shared_ptr<HastNode>> HaskellModel::apply_all_functions(const std::vector<std::shared_ptr<HastNode>>& nodes) {
    size_t i = 0, n = nodes.size();
    std::vector<std::shared_ptr<HastNode>> result;

    while(i < n){
        if(nodes[i]->type == HastNodeType::Variable && functions.contains(nodes[i]->value)){
            auto node = functions[nodes[i]->value];
            size_t arity = node->number_of_arguments;
            while(arity > 0){
                ++i;
                node->arguments.push_back(nodes[i]);
            }
            result.emplace_back(node);
        } else {
            result.emplace_back(nodes[i]);
        }
        i++;
    }
    return result;
}

std::vector<std::shared_ptr<HastNode>> HaskellModel::apply_operators(std::vector<std::shared_ptr<HastNode>>& nodes, size_t from , size_t to) {

    std::vector<std::shared_ptr<HastNode>> result;

    for(size_t precedence = to; precedence >= from; --precedence){
        std::vector<std::shared_ptr<HastNode>> result;
        int i = 0, n = (int) nodes.size();

        while(i < n){
            if((nodes[i]->type == HastNodeType::Operator || nodes[i]->type == HastNodeType::PrefixOperator)
                    && operators.contains(nodes[i]->value)){
                auto node = operators[nodes[i]->value];
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
               && operators.contains(nodes[i]->value)){
                auto node = operators[nodes[i]->value];
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
    }
    return nodes;
}

std::shared_ptr<HastNode> HaskellModel::build_expression(const TokenList& list){
    std::vector<std::shared_ptr<HastNode>> nodes;
    if(list.empty())
        return {};
    for(const auto & i : list){
        nodes.emplace_back(build_expression(i));
    }
    nodes = apply_all_functions(nodes);
    nodes = apply_data_constructors(nodes);
    nodes = apply_operators(nodes, 6, 9);
    nodes = apply_list_constructors(nodes);
    nodes = apply_operators(nodes, 1, 5);
    assert(nodes.size() == 1);
    return nodes[0];
}

void HaskellModel::process_functions(const std::vector<TokenTree>& trees) {
    auto function_expression_vector = add_function_arity(trees);
    for(auto const& expr : function_expression_vector){
        auto mask = build_full_function_mask(std::get<0>(expr));
        for(auto const& node : mask.second )
            HastPrinter::print_node(node);
        auto bodies = std::get<1>(expr);

        std::string func_name = mask.first;
        auto func = get_or_create_function(func_name);
        std::cout << "BODY OF " << func_name << " :" << std::endl;

        if(bodies.size() == 1 && bodies[0].first.empty()){ //has no guards
            auto body = build_expression(bodies[0].second);
            HastPrinter::print_node(body);
            func->add_expression(mask, body);
        } else {
            std::vector<HAST_GUARD> built_guards = {};
            for(auto const & body_part : bodies){
                HAST_N guard_expr = build_expression(body_part.first);
                HAST_N guard_body = build_expression(body_part.second);

                std::cout << "GUARD: " << std::endl;
                HastPrinter::print_node(guard_expr);
                std::cout << "BODY: " << std::endl;
                HastPrinter::print_node(guard_body);
                built_guards.emplace_back(guard_expr, guard_body);
            }
            func->add_expression_with_guards(mask, built_guards);
        }
    }
}

std::vector<TokenTree> HaskellModel::process_type_classes(const std::vector<TokenTree> &trees) {
    std::vector<TokenTree> filtered;
    for(auto & tree : trees){
        if(tree.children[0].which() == 1){
            std::string name = get<std::string>(tree.children[0]);
            if(name == "class"){
                add_data_structure(tree);
            } else if (name == "instance"){
                //add class instances, how would I check types??
            } else {
                filtered.emplace_back(tree);
            }
        } else {
            filtered.emplace_back(tree);
        }
    }
    return filtered;
}

std::vector<TokenTree> HaskellModel::process_data_types(const std::vector<TokenTree> &trees) {
    std::vector<TokenTree> filtered;
    for(auto & tree : trees){
        if(tree.children[0].which() == 1){
            std::string name = get<std::string>(tree.children[0]);
            if(name == "data"){
                add_data_structure(tree);
            } else { //TODO: type, newtype, ???
                filtered.emplace_back(tree);
            }
        } else {
            filtered.emplace_back(tree);
        }
    }
    return filtered;
}

std::vector<TokenTree> HaskellModel::process_headers(const std::vector<TokenTree> &trees) {
    std::vector<TokenTree> filtered;
    for(auto & tree : trees){
        if(tree.children[0].which() == 1){
            std::string name = get<std::string>(tree.children[0]);
            if(name == "module"){
                try{
                    setName(get<std::string>(tree.children[1]));
                } catch(std::exception& ex){
                    throw IncorrectTokenException(get<TokenTree>(tree.children[1]), "Module name is");
                }
            } else if (name == "import"){
                addImportToModel(tree.children);
            } else {
                filtered.emplace_back(tree);
            }
        } else {
            filtered.emplace_back(tree);
        }
    }
    return filtered;
}

void HaskellModel::setName(const string &name) {
    moduleName = name;
}

void HaskellModel::addImportToModel(const std::vector<TokenNode> &import) {
    //TODO: Implement
}

void HaskellModel::add_data_structure(const TokenTree &tree) {
    //TODO: Implement
}

void HaskellModel::read_prelude(const std::vector<std::string> &lines) {
    size_t i = 0, n = lines.size();
    while(i < n && !lines[i].empty() && !lines[i].starts_with("infix")){
        std::string func_name = lines[i].substr(0,lines[i].find(' '));
        std::string func_arity = lines[i].substr(lines[i].find(' ')+1);
        int arity = std::stoi(func_arity);
        HAST_FN func_node = std::make_shared<HastFunctionNode>();
        func_node->number_of_arguments = arity;
        functions.insert({func_name, func_node});
        i++;
    }
    if(i < n && lines[i].empty()) i++;
    while(i < n  && !lines[i].empty()&& !lines[i].starts_with(":")){
        size_t first_space = lines[i].find(' ');
        size_t second_space = lines[i].find(' ', first_space+1);

        HAST_ON oper_node = std::make_shared<HastOperatorNode>();
        if(lines[i].starts_with("infixr")){
            oper_node->associativity = OperatorAssociativity::Right;
        } else if(lines[i].starts_with("infixl")){
            oper_node->associativity = OperatorAssociativity::Left;
        } else {
            oper_node->associativity = OperatorAssociativity::None;
        }
        std::string oper_name = lines[i].substr(second_space + 1);
        std::string oper_precedence = lines[i].substr(first_space+1,first_space);
        int precedence = std::stoi(oper_precedence);
        oper_node->precedence = precedence;
        oper_node->set_value(oper_name);
        operators.insert({oper_name, oper_node});
        i++;
    }
    if(i < n && lines[i].empty()) i++;
    while(i < n  && !lines[i].empty()&& lines[i].starts_with(":")){
        infix_data_constructors.insert(lines[i]);
        i++;
    }
    if(i < n && lines[i].empty()) i++;
    while(i < n){
        size_t first_space = lines[i].find(' ');

        std::string data_name = lines[i].substr(0,first_space);
        std::string data_arity = lines[i].substr(first_space+1);
        int arity = std::stoi(data_arity);
        data_constructor_arity.insert({data_name, arity});
        i++;
    }
}

