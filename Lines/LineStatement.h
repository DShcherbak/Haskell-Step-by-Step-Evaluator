#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_LINESTATEMENT_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_LINESTATEMENT_H
#include <string>
#include <vector>

namespace lines {
    using std::string, std::vector;

    class LineStatement {
    public:
        string line;
        string original_line;
        vector<string> where_clause;

        LineStatement();

        explicit LineStatement(const std::string& s);

        void append(const std::string& new_line);
        void append_to_original(const string &new_line);
        void append_both(const string &new_line, const string &originalLine);

        void append_to_where(const std::string& new_line);
    };

}


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_LINESTATEMENT_H
