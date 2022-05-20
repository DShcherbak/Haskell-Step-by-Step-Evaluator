#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLFILEPARSER_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLFILEPARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "LineStatement.h"
#include "IndentedLine.h"
#include "LineParser.h"

#define indent_vector std::vector<lines::IndentedLine>
#define statement_vector std::vector<lines::LineStatement>

const std::vector<std::string> header_prefixes = {"module", "import"};
const std::vector<std::string> data_prefixes = {"data", "type", "newtype"};
const std::vector<std::string> type_prefixes = {"class", "instance"};


namespace lines {
    class HaskellFileParser {
    public:
        HaskellFileParser()= default;
        statement_vector parse_file(const std::string& file_name);
    private:
        std::vector<std::string> preprocess_lines(const std::vector<string> &lines);
        statement_vector compress_lines(const indent_vector& lines);
        indent_vector indentation(const std::vector<std::string> &lines);
        std::vector<std::string> get_lines_from_file(const std::string &file_name);
    };
}


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLFILEPARSER_H
