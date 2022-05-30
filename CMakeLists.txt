cmake_minimum_required(VERSION 3.21)
project(Haskell_Step_by_Step_Evaluator)

set(CMAKE_CXX_STANDARD 20)

add_executable(Haskell_Step_by_Step_Evaluator main.cpp


        Lines/IndentedLine.cpp Lines/IndentedLine.h
        Lines/HaskellFileParser.cpp Lines/HaskellFileParser.h

        Model/HaskellModel.cpp Model/HaskellModel.h

        Model/HaskellFunction/Function.cpp Model/HaskellFunction/Function.h
        Model/HaskellFunction/Mask.cpp Model/HaskellFunction/Mask.h
        Model/HaskellFunction/Guard.cpp Model/HaskellFunction/Guard.h
        Model/HaskellFunction/Body.cpp Model/HaskellFunction/Body.h
        Model/HaskellFunction/MaskTemplate.cpp Model/HaskellFunction/MaskTemplate.h

        Parsing/parsing.cpp Parsing/parsing.h
        Parsing/PartiallyParsedString.cpp Parsing/PartiallyParsedString.h

        Lexer/Lexer.cpp Lexer/Lexer.h
        Lexer/TokenTree.h Lexer/TokenTree.cpp
        Lexer/Lexer.h Lexer/Lexer.cpp
        Lexer/expr_grammar.h Exception/CustomException.h Exception/IncorrectTokenException.h HAST/HastNode.cpp HAST/HastNode.h HAST/HastMaskNode.cpp HAST/HastMaskNode.h HAST/HastFunctionNode.cpp HAST/HastFunctionNode.h)
