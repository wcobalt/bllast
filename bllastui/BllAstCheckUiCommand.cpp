//
// Created by wcobalt on 2/8/21.
//

#include "BllAstCheckUiCommand.h"
#include "../ui/UiParametersParser.h"

using namespace bllast;
using namespace ui;

BllAstCheckUiCommand::BllAstCheckUiCommand(BllAstCalculator *bllAstCalculator,
                                           const BllAstTruthTableComputer *truthTableComputer,
                                           const BllAstSimplifier* bllAstSimplifier,
                                           const BllAstPnfChecker *bllAstPnfChecker, const BllAstParser *bllAstParser)
        : BllAstBaseUiCommand(bllAstCalculator, truthTableComputer, bllAstSimplifier), bllAstPnfChecker(bllAstPnfChecker),
          bllAstParser(bllAstParser) {
    pcnfParameter = std::move(std::make_unique<UiParameter>("pcnf",
            std::vector<std::string>{"-c", "--pcnf"}, UiParameter::Type::FLAG));

    pdnfParameter = std::move(std::make_unique<UiParameter>("pdnf",
            std::vector<std::string>{"-d", "--pdnf"}, UiParameter::Type::FLAG));
}

bool bllast::BllAstCheckUiCommand::check(std::string_view command) const {
    StandardFormParseResult parseResult(command);

    return parseResult.getCommandName() == CHECK_COMMAND_NAME;
}

UiCommand::Result bllast::BllAstCheckUiCommand::execute(std::string_view command) {
    std::string buffer;

    try {
        StandardFormParseResult parseResult(command);
        UiParametersParser uiParametersParser;
        std::vector<std::unique_ptr<UiParameterInstance>> parameters = uiParametersParser.parse(
                parseResult.getParamsString(),
                {&getPrintAstParameter(), &getPrintTruthTableParameter(), &getSimplifyParameter(),
                 pcnfParameter.get(), pdnfParameter.get()});

        std::unique_ptr<BllAstNode> originalFormula = bllAstParser->parse(parseResult.getExpression());

        std::string originalFormulaTitle = "Original formula ";

        if (findParameterInstance(parameters, &getPrintAstParameter()))
            buffer += printAst(originalFormula.get(), originalFormulaTitle + DEFAULT_AST_TITLE) + '\n';

        if (findParameterInstance(parameters, &getPrintTruthTableParameter()))
            buffer += printTruthTable(originalFormula.get(), originalFormulaTitle + DEFAULT_TT_TITLE) + '\n';

        std::unique_ptr<BllAstNode> handledFormula = std::move(originalFormula);

        const UiParameterInstance *simplifyParameter = findParameterInstance(parameters, &getSimplifyParameter());
        if (simplifyParameter) {
            handledFormula = std::move(simplify(handledFormula.get(), simplifyParameter->getStringValue()));

            buffer += "Simplified expression: " + handledFormula->toFormulaInStringForm() + '\n';
        }

        std::vector<std::string> bools = {"no", "yes"};

        if (findParameterInstance(parameters, pdnfParameter.get())) {
            buffer += std::string("Is PDNF: ") +
                      bools[bllAstPnfChecker->isPerfectDisjunctiveNormalForm(handledFormula.get())];
            buffer += '\n';
        }

        if (findParameterInstance(parameters, pcnfParameter.get())) {
            buffer += std::string("Is PCNF: ") +
                      bools[bllAstPnfChecker->isPerfectConjunctiveNormalForm(handledFormula.get())];
            buffer += '\n';
        }

        return {false, 0, buffer};
    } catch (std::invalid_argument& exc) {
        buffer += "\nException: " + std::string(exc.what()) + '\n';

        return {false, 1, buffer};
    }
}
