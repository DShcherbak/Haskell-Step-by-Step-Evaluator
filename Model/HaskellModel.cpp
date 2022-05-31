#include "HaskellModel.h"

void HaskellModel::AddStatements(std::vector<string> &statements) {
    auto token_tree_vector = Lexer::functions_to_tokens(statements);
    token_tree_vector = process_headers(token_tree_vector);
    token_tree_vector = process_data_types(token_tree_vector);
    token_tree_vector = process_type_classes(token_tree_vector);
    //operators and function type definitions
    process_functions(token_tree_vector);
    std::cout << functions.size() << std::endl;
    std::cout << functions.size() << std::endl;
}


std::shared_ptr<HastFunctionNode> HaskellModel::get_or_create_function(const string &name, const TokenList &tokens) {
    if(!functions.contains(name)){
        auto func_node = std::make_shared<HAST_FN>();
        func_node->number_of_arguments = tokens.size()-1;
        functions.insert({name, func_node});
    }
    return functions[name];
}

//  auto func = get_or_create_function(name);
//  func->add_mask(mask);

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

void HaskellModel::process_functions(const std::vector<TokenTree>& trees) {
    size_t i = 0, n = trees.size();
    for(auto & tree : trees){
        auto processed_guards = process_guards(tree);
        auto function_mask = std::get<0>(processed_guards);
        if(function_mask.empty() || function_mask[0].which() == 0)
            continue;
        std::string function_name = get<std::string>(function_mask[0]);
        get_or_create_function(function_name, function_mask);
    }
    for(const auto& f : functions){
        std::cout << f.first << " : " << f.second->number_of_arguments << std::endl;
    }
    std::cout << "OPERATORS: \n";
    for(const auto& f : operators){
        std::cout << f.second->value << " : " << f.second->precedence << std::endl;
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
        auto func_node = std::make_shared<HAST_FN>();
        func_node->number_of_arguments = arity;
        functions.insert({func_name, func_node});
        i++;
    }
    if(i < n && lines[i].empty()) i++;
    while(i < n){
        size_t first_space = lines[i].find(' ');
        size_t second_space = lines[i].find(' ', first_space+1);

        auto oper_node = std::make_shared<HAST_ON>();
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
        oper_node->value = oper_name;
        operators.insert({oper_name, oper_node});
        i++;
    }
}

