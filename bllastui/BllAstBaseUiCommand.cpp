//
// Created by wcobalt on 2/8/21.
//

#include <regex>
#include "BllAstBaseUiCommand.h"

using namespace bllast;
using namespace ui;

BllAstBaseUiCommand::BllAstBaseUiCommand(BllAstCalculator *bllAstCalculator,
                                         const BllAstTruthTableComputer *truthTableComputer,
                                         const BllAstSimplifier* bllAstSimplifier)
                                         : bllAstCalculator(bllAstCalculator), truthTableComputer(truthTableComputer),
                                         bllAstSimplifier(bllAstSimplifier) {
    //todo strings to consts
    printAstParameter = std::move(std::make_unique<UiParameter>("print_ast",
            std::vector<std::string>{"-a", "--print-ast"}, UiParameter::Type::FLAG));

    printTruthTableParameter = std::move(std::make_unique<UiParameter>("print_tt",
            std::vector<std::string>{"-t", "--print-tt"}, UiParameter::Type::FLAG));

    simplifyParameter = std::move(std::make_unique<UiParameter>("simplify",
            std::vector<std::string>{"-s", "--simplify"}, UiParameter::Type::STRING));
}

const UiParameterInstance *BllAstBaseUiCommand::findParameterInstance(
        const std::vector<std::unique_ptr<UiParameterInstance>> &instances,
        const UiParameter *parameter) const {
    for (auto& instance : instances) {
        if (instance->getUiParameter().getId() == parameter->getId())
            return instance.get();
    }

    return nullptr;
}

std::string BllAstBaseUiCommand::printAst(const BllAstNode *root) const {
    return printAst(root, DEFAULT_AST_TITLE);
}

std::string BllAstBaseUiCommand::printAst(const BllAstNode *root, std::string_view title) const {
    return std::string(title) + "\n" + root->toAstInStringForm();
}

std::string BllAstBaseUiCommand::printTruthTable(const BllAstNode *root) const {
    return printTruthTable(root, DEFAULT_TT_TITLE);
}

std::string BllAstBaseUiCommand::printTruthTable(const BllAstNode *root, std::string_view title) const {
    return std::string(title) + "\n" + truthTableComputer->computeTruthTable(bllAstCalculator, root)->toString();
}

std::unique_ptr<BllAstNode>
BllAstBaseUiCommand::simplify(const BllAstNode *root, std::string_view simplificationLevel) const {
    std::map<std::string, BllAstSimplifier::mask_type> masks;

    {
        BllAstSimplifier::mask_type temp_mask = 0;

        masks.emplace(std::make_pair(SIMPLIFICATION_LEVEL_0, temp_mask |= BllAstSimplifier::FULL_LITERAL_CALCULATION));
        masks.emplace(std::make_pair(SIMPLIFICATION_LEVEL_1, temp_mask |= BllAstSimplifier::REDUNDANT_DISJUNCTION_AND_CONJUNCTION));
        masks.emplace(std::make_pair(SIMPLIFICATION_LEVEL_2, temp_mask |= BllAstSimplifier::APRIORI_KNOWN_DISJUNCTION_AND_CONJUNCTION));
        masks.emplace(std::make_pair(SIMPLIFICATION_LEVEL_3, temp_mask | BllAstSimplifier::MULTIPLE_NEGATIONS));
    }

    auto it = masks.find(std::string(simplificationLevel));

    if (it != masks.end()) {
        BllAstSimplifier::mask_type mask = it->second;

        return bllAstSimplifier->simplify(root, mask);
    }
}

const UiParameter &BllAstBaseUiCommand::getPrintAstParameter() const {
    return *printAstParameter;
}

const UiParameter &BllAstBaseUiCommand::getPrintTruthTableParameter() const {
    return *printTruthTableParameter;
}

const UiParameter &BllAstBaseUiCommand::getSimplifyParameter() const {
    return *simplifyParameter;
}

BllAstBaseUiCommand::StandardFormParseResult::StandardFormParseResult(std::string_view command) {
    std::regex pattern(STANDARD_FORM_PATTERN);
    std::cmatch match;

    std::regex_match(command.begin(), command.end(), match, pattern);

    if (!match.empty()) {
        commandName = match[1];
        expression = match[2];
        paramsString = match[3];
    }
}

const std::string &BllAstBaseUiCommand::StandardFormParseResult::getCommandName() const {
    return commandName;
}

const std::string &BllAstBaseUiCommand::StandardFormParseResult::getExpression() const {
    return expression;
}

const std::string &BllAstBaseUiCommand::StandardFormParseResult::getParamsString() const {
    return paramsString;
}
