cmake_minimum_required(VERSION 3.15)
project(1)

set(CMAKE_CXX_STANDARD 17)

add_executable(1 main.cpp BllAstMaker.cpp BllAstMaker.h BllAstPnfChecker.cpp BllAstPnfChecker.h BllAstOperator.cpp BllAstOperator.h BllAstNode.cpp BllAstNode.h BllAstCalculator.cpp BllAstCalculator.h BllAstTruthTableComputer.cpp BllAstTruthTableComputer.h BllAstTruthTable.cpp BllAstTruthTable.h TextCanvasUtils.cpp TextCanvasUtils.h TextCanvas.cpp TextCanvas.h BllAstConverterToPnf.cpp BllAstConverterToPnf.h)