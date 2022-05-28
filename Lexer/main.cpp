#include <iostream>
#include "file_reader.h"
#include "Lexer.h"


int main() {
    auto text_from_file = file_reader::get_strings();
    std::cout << text_from_file.size() << std::endl;

    expression_parser::try_parse(text_from_file);
    return 0;
}
