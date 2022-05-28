#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_CUSTOMEXCEPTION_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_CUSTOMEXCEPTION_H

#include <exception>
#include <string>

struct CustomException : public std::exception{

    explicit CustomException(std::string exception_text){
        text = std::move(exception_text);
    }
    std::basic_string<char> text;
    [[nodiscard]] const char * what() const noexcept override{
        return text.c_str();
    }
};



#endif //HASKELL_STEP_BY_STEP_EVALUATOR_CUSTOMEXCEPTION_H
