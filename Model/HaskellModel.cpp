#include "HaskellModel.h"
#include "../Lexer/Lexer.h"
#include "../Exception/IncorrectTokenException.h"

void HaskellModel::AddStatements(std::vector<string> &statements) {
    auto token_tree_vector = Lexer::functions_to_tokens(statements);
    token_tree_vector = process_headers(token_tree_vector);
   // statements = process_data_types(statements);
   // statements = process_type_classes(statements);
   // process_functions(statements);
    std::cout << functions.size() << std::endl;
    std::cout << functions.size() << std::endl;
}


std::shared_ptr<function::Function> HaskellModel::GetOrCreateFunction(const std::string& name){
    if(!functions.contains(name)){
        functions[name] = std::make_shared<function::Function>(name);
    }
    return functions[name];
}

void HaskellModel::function_type_define(const std::string& name, const lines::LineStatement& stmnt){
    auto func = GetOrCreateFunction(name);
    func->type_definition = stmnt.line;
    func->type_definition_string = stmnt.original_line;
}

  //  auto func = GetOrCreateFunction(name);
  //  func->add_mask(mask);

void HaskellModel::process_functions(const std::vector<lines::LineStatement> &statements) {

}

std::vector<lines::LineStatement> HaskellModel::process_type_classes(const std::vector<lines::LineStatement> &statements) {
 //   std::pair<statement_vector, statement_vector> filtered = filter_by_prefixes(statements, type_prefixes);

    //TODO : Do something with type classes

 //   return filtered.second;
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

