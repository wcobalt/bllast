cmake_minimum_required(VERSION 3.15)
project(1)

set(CMAKE_CXX_STANDARD 17)

add_executable(1 main.cpp BllAstParser.cpp BllAstParser.h BllAstPnfChecker.cpp BllAstPnfChecker.h BllAstOperator.cpp BllAstOperator.h BllAstNode.cpp BllAstNode.h BllAstCalculator.cpp BllAstCalculator.h BllAstTruthTableComputer.cpp BllAstTruthTableComputer.h BllAstTruthTable.cpp BllAstTruthTable.h TextCanvasUtils.cpp TextCanvasUtils.h TextCanvas.cpp TextCanvas.h BllAstConverterToPnf.cpp BllAstConverterToPnf.h ui/UiCommand.h ui/UiParameter.cpp ui/UiParameter.h ui/UiParameterInstance.cpp ui/UiParameterInstance.h ui/UiParametersParser.cpp ui/UiParametersParser.h bllastui/BllAstBaseUiCommand.cpp bllastui/BllAstBaseUiCommand.h bllastui/BllAstCheckUiCommand.cpp bllastui/BllAstCheckUiCommand.h ui/UiMainLoop.cpp ui/UiMainLoop.h bllastui/BllAstExitUiCommand.h)