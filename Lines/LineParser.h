#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_LINEPARSER_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_LINEPARSER_H
#include <vector>
#include "Statement.h"
#include "IndentedLine.h"

class LineParser {
    enum State {Regular, WhereUnknown, Where};
    State state;
    int current_indent;

    std::vector<lines::Statement> result;
    lines::Statement line_under_construction;

    void push_next_line(const lines::IndentedLine &line, int prefix_length = 0);
    void add_body_to_current_line(const lines::IndentedLine &indented_line, int prefix_length = 0);
    void update_where_state(const lines::IndentedLine& line, int offset = 0);

public:
    LineParser();
    std::vector<lines::Statement> get_result();
    void parse_line(const lines::IndentedLine& line);


    bool parse_special_cases(const lines::IndentedLine &line);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_LINEPARSER_H
