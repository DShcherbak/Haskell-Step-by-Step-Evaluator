cmake_minimum_required(VERSION 3.21)
project(Haskell_Step_by_Step_Evaluator)

set(CMAKE_CXX_STANDARD 20)

add_executable(Haskell_Step_by_Step_Evaluator main.cpp Model/HaskellModel.cpp Model/HaskellModel.h deprecated/dataType.cpp deprecated/dataType.h Model/HaskellFunction.cpp Model/HaskellFunction.h deprecated/ExpressionElement.cpp deprecated/ExpressionElement.h deprecated/ExpressionMask.cpp deprecated/ExpressionMask.h Lines/Statement.cpp Lines/Statement.h Lines/LineParser.cpp Lines/LineParser.h Lines/IndentedLine.cpp Lines/IndentedLine.h Lines/HaskellFileParser.cpp Lines/HaskellFileParser.h)
