//
// Created by wcobalt on 2/8/21.
//

#include "BllAstTransformUiCommand.h"
#include "../ui/UiParametersParser.h"

using namespace bllast;
using namespace ui;

BllAstTransformUiCommand::BllAstTransformUiCommand(BllAstCalculator *bllAstCalculator,
                                                   const BllAstTruthTableComputer *truthTableComputer,
                                                   const BllAstConverterToPnf *bllAstConverterToPnf,
                                                   const BllAstParser *bllAstParser) : BllAstBaseUiCommand(
        bllAstCalculator, truthTableComputer), bllAstConverterToPnf(bllAstConverterToPnf), bllAstParser(bllAstParser) {
    pcnfParameter = std::move(std::make_unique<UiParameter>("pcnf",
            std::vector<std::string>{"-c", "--pcnf"}, UiParameter::Type::FLAG));

    pdnfParameter = std::move(std::make_unique<UiParameter>("pdnf",
            std::vector<std::string>{"-d", "--pdnf"}, UiParameter::Type::FLAG));

    printNewAstParameter = std::move(std::make_unique<UiParameter>("print_new_ast",
            std::vector<std::string>{"-A", "--print-new-ast"}, UiParameter::Type::FLAG));

    printNewTruthTableParameter = std::move(std::make_unique<UiParameter>("print_new_tt",
            std::vector<std::string>{"-T", "--print-new-tt"}, UiParameter::Type::FLAG));
}

bool BllAstTransformUiCommand::check(std::string_view command) const {
    StandardFormParseResult parseResult(command);

    return parseResult.getCommandName() == TRANSFORM_COMMAND_NAME;
}

UiCommand::Result BllAstTransformUiCommand::execute(std::string_view command) {
    std::string buffer;

    try {
        StandardFormParseResult parseResult(command);
        UiParametersParser uiParametersParser;
        std::vector<std::unique_ptr<UiParameterInstance>> parameters = uiParametersParser.parse(
                parseResult.getParamsString(),
                {&getPrintAstParameter(), &getPrintTruthTableParameter(), &getSimplifyParameter(),
                 pcnfParameter.get(), pdnfParameter.get(), printNewAstParameter.get(), printNewTruthTableParameter.get()});

        std::unique_ptr<BllAstNode> originalFormula = bllAstParser->parse(parseResult.getExpression());

        buffer += printAstTruthTable(originalFormula.get(), findParameterInstance(parameters, &getPrintAstParameter()),
                                     findParameterInstance(parameters, &getPrintTruthTableParameter()), "");

        bool doPrintNewAst = findParameterInstance(parameters, printNewAstParameter.get()),
            doPrintNewTruthTable = findParameterInstance(parameters, printNewTruthTableParameter.get());

        std::unique_ptr<BllAstNode> handledFormula = std::move(originalFormula);

        const UiParameterInstance *simplifyParameter = findParameterInstance(parameters, &getSimplifyParameter());
        if (simplifyParameter) {
            handledFormula = std::move(simplify(originalFormula.get(), simplifyParameter->getStringValue()));

            buffer += "Simplified expression: " + handledFormula->toFormulaInStringForm() + '\n';

            buffer += printAstTruthTable(handledFormula.get(), doPrintNewAst, doPrintNewTruthTable,
                                         "Simplified expression");
        }

        if (findParameterInstance(parameters, pdnfParameter.get())) {
            std::unique_ptr<BllAstNode> pdnf = bllAstConverterToPnf->transformToPdnf(handledFormula.get());

            buffer += std::string("PDNF: ") + pdnf->toFormulaInStringForm() + '\n';
            buffer += printAstTruthTable(pdnf.get(), doPrintNewAst, doPrintNewTruthTable, "PDNF");
        }

        if (findParameterInstance(parameters, pcnfParameter.get())) {
            std::unique_ptr<BllAstNode> pcnf = bllAstConverterToPnf->transformToPcnf(handledFormula.get());

            buffer += std::string("PCNF: ") + pcnf->toFormulaInStringForm() + '\n';
            buffer += printAstTruthTable(pcnf.get(), doPrintNewAst, doPrintNewTruthTable, "PCNF");
        }

        return {false, 0, buffer};
    } catch (std::invalid_argument& exc) {
        buffer += "\nException: " + std::string(exc.what()) + '\n';

        return {false, 1, buffer};
    }
}

std::string
BllAstTransformUiCommand::printAstTruthTable(const BllAstNode *node, bool doPrintAst, bool doPrintTruthTable,
                                             std::string object) const {
    std::string buffer;

    if (doPrintAst)
        buffer += printAst(node, object + " " + DEFAULT_AST_TITLE) + '\n';

    if (doPrintTruthTable)
        buffer += printTruthTable(node, object + " " + DEFAULT_TT_TITLE) + '\n';

    return buffer;
}
