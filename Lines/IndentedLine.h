#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_INDENTEDLINE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_INDENTEDLINE_H

#include <string>

namespace lines{

    using std::string;


    class IndentedLine {
    public:
        string parsed_line;
        string original_line;
        int indentation;
        IndentedLine(const string& ind_line, const string& orig_line, int ind);
    };
}



#endif //HASKELL_STEP_BY_STEP_EVALUATOR_INDENTEDLINE_H
