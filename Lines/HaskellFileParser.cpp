#include "HaskellFileParser.h"

namespace lines {


    statement_vector HaskellFileParser::ParseFile(const std::string &file_name) {
        std::vector<std::string> lines;
        lines = GetLinesFromFile(file_name);
        lines = preprocess_lines(lines);

        indent_vector lines_with_indentation = indentation(lines);
        statement_vector statements = compress_lines(lines_with_indentation);
        return statements;
    }


    std::vector<std::string> HaskellFileParser::GetLinesFromFile(const std::string &fileName) {
        std::vector<std::string> lines;
        try {
            std::fstream istream;
            istream.open(fileName, std::fstream::in);
            if (istream.is_open()) {
                typedef std::istreambuf_iterator<char> buf_iter;
                char c;
                std::string current_line;
                while (std::getline(istream, current_line)) {
                    lines.push_back(current_line);
                }
            }
            istream.close();
        }
        catch (std::exception &ex) {
            std::cout << "Couldn't parse '" << fileName << "', got an error: " << ex.what() << std::endl;
        }
        return lines;
    }

    std::vector<string> HaskellFileParser::preprocess_lines(const std::vector<string> &lines) {
        std::vector<string> result;
        int current_line = 0;
        for (auto &line: lines) {
            result.emplace_back("");
            for (char c: line) {
                if (c == '\n') {
                    continue;
                } else if (c == '\t') {
                    result[current_line] += "    ";
                } else {
                    result[current_line] += c;
                }
            }
            current_line++;
        }
        return result;
    }

    indent_vector HaskellFileParser::indentation(const std::vector<std::string> &lines) {
        indent_vector result;
        for (auto &line: lines) {
            if (line.empty())
                continue;
            size_t ident = 0, i = 0, n = line.size();
            string new_string;
            while (i < n && line[i] == ' ') {
                ident++;
                i++;
            }
            if (i == n)
                continue;
            while (i < n) {
                if (line[i] == ' ') {
                    new_string += ' ';
                    while (line[i] == ' ') {
                        i++;
                    }
                } else if (line[i] == '-' && i < n - 1 && line[i + 1] == '-') {
                    break;
                } else {
                    new_string += line[i++];
                }
            }
            result.emplace_back(new_string, line, ident);
        }
        return result;
    }

    statement_vector HaskellFileParser::compress_lines(const indent_vector &lines) {
        size_t cur_line = 0, compressed_line_id = 0, next_line = 0, n = lines.size();

        LineParser parser = LineParser();
        for (auto &line: lines) {
            parser.parse_line(line);
        }
        return parser.get_result();
    }

}