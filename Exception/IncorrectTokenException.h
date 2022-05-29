#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_INCORRECTTOKENEXCEPTION_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_INCORRECTTOKENEXCEPTION_H

#include <iostream>
#include "CustomException.h"
#include "../Lexer/TokenTree.h"


struct IncorrectTokenException : CustomException{
    explicit IncorrectTokenException(const TokenTree& token_tree, std::string exception_text)
            : CustomException(exception_text){
        tree = token_tree;
        text = std::move(exception_text);
    }
    std::basic_string<char> text;
    TokenTree tree;
    [[nodiscard]] const char * what() const noexcept override{
        std::cout << "Error in tree: " << std::endl;
        tree.print();
        return text.c_str();
    }
};

#endif //HASKELL_STEP_BY_STEP_EVALUATOR_INCORRECTTOKENEXCEPTION_H
