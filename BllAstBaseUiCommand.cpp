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
