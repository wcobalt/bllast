//
// Created by wcobalt on 2/8/21.
//

#include "BllAstBaseUiCommand.h"

using namespace bllast;
using namespace ui;

BllAstBaseUiCommand::BllAstBaseUiCommand(BllAstCalculator *bllAstCalculator,
                                         const BllAstTruthTableComputer *truthTableComputer) : bllAstCalculator(
        bllAstCalculator), truthTableComputer(truthTableComputer) {}

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
    return std::unique_ptr<BllAstNode>();
}

BllAstBaseUiCommand::StandardFormParseResult::StandardFormParseResult(std::string_view command) {
    uint8_t state = 0; //0 - reading com. name, 1 - reading subcom. name, 2 - reading expression, 3 - reading params list
    std::string currentValue;

    for (char c : command) {
        if (c == SPACE) {
            if (!currentValue.empty()) {
                putValue(currentValue, state);

                ++state;
                currentValue.clear();
            }

        } else
            currentValue += c;
    }
}

void BllAstBaseUiCommand::StandardFormParseResult::putValue(std::string value, uint8_t state) {
    switch (state) {
        case 0:
            commandName = std::move(value);

            break;

        case 1:
            subCommandName = std::move(value);

            break;

        case 2:
            expression = std::move(value);

            break;

        case 3:
            paramsString = std::move(value);

            break;
    }
}

const std::string &BllAstBaseUiCommand::StandardFormParseResult::getCommandName() const {
    return commandName;
}

const std::string &BllAstBaseUiCommand::StandardFormParseResult::getSubCommandName() const {
    return subCommandName;
}

const std::string &BllAstBaseUiCommand::StandardFormParseResult::getExpression() const {
    return expression;
}

const std::string &BllAstBaseUiCommand::StandardFormParseResult::getParamsString() const {
    return paramsString;
}
