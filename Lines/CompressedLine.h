#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_COMPRESSEDLINE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_COMPRESSEDLINE_H
#include <string>
#include <vector>

namespace lines {
    using std::string, std::vector;

    class CompressedLine {
    public:
        CompressedLine(){
            line = "";
            where_clause = vector<string>();
        }

        explicit CompressedLine(const std::string& s);

        void append(const std::string& new_line);
        void append_to_where(const std::string& new_line);

        string line;
        vector<string> where_clause;
    };

}


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_COMPRESSEDLINE_H
