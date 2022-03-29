cmake_minimum_required(VERSION 3.21)
project(Haskell_Step_by_Step_Evaluator)

set(CMAKE_CXX_STANDARD 20)

add_executable(Haskell_Step_by_Step_Evaluator main.cpp HaskellModel.cpp HaskellModel.h dataType.cpp dataType.h HaskellFunction.cpp HaskellFunction.h ExpressionElement.cpp ExpressionElement.h ExpressionMask.cpp ExpressionMask.h Lines/CompressedLine.cpp Lines/CompressedLine.h Lines/LineParser.cpp Lines/LineParser.h)
