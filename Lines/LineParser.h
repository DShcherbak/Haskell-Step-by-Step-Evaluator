#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_LINEPARSER_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_LINEPARSER_H
#include <vector>
#include "CompressedLine.h"

class LineParser {
    enum State {Regular, WhereUnknown, Where};
    State state;
    int current_indent;

    std::vector<lines::CompressedLine> result;
    lines::CompressedLine line_under_construction;
public:
    LineParser();
    std::vector<lines::CompressedLine> get_result();
    void parse_line(const std::pair<std::string, int>& parsed_line);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_LINEPARSER_H
