#include "FunctionArity.h"

FunctionArity FunctionArity::build_arity(const std::vector<TokenNode> &nodes) {
    FunctionArity functionArity{};
    if(nodes.size() == 2 && nodes[1].which() == 1 && get<std::string>(nodes[1]) == "[]"){
        return functionArity;
    }
    for(size_t i = 0, n = nodes.size()-1; i < n; i++){
        FunctionArity argument_arity{};
        if(nodes[i].which() == 0)
            argument_arity = FunctionArity::build_arity(get<TokenTree>(nodes[i]).children);
        else if (get<std::string>(nodes[i]) == "->")
            continue;
        functionArity.argument_arity.push_back(argument_arity);
    }
    return functionArity;
}

int FunctionArity::arity() {
    return argument_arity.size()-1;
}
