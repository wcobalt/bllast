//
// Created by wcobalt on 2/7/21.
//

#include "BllAstCalculator.h"

using namespace bllast;

void BllAstCalculator::extractVariableNames(const BllAstNode *node) {
    switch (node->getType()) {
        case BllAstNode::BllAstNodeType::VARIABLE:
            extractedVariableNames.insert(node->getVariableName());

            break;

        case BllAstNode::BllAstNodeType::OPERATOR:
            for (const auto& child : node->getChildren())
                extractVariableNames(child);

            break;

        default:;
    }
}

void BllAstCalculator::putBllAst(const BllAstNode *bllAstNode) {
    reset();

    root = bllAstNode;

    extractVariableNames(root);
}

void BllAstCalculator::setVariableValue(std::string_view variableName, bool value) {
    values.insert(std::make_pair(variableName, value));
}

void BllAstCalculator::resetValues() {
    values.clear();
}

void BllAstCalculator::reset() {
    resetValues();
    extractedVariableNames.clear();
    root = nullptr;
}

bool BllAstCalculator::calculate() const {
    //todo check for existence of root and so on...
    return calculate(root);
}

bool BllAstCalculator::calculate(const BllAstNode *node) const {
    switch (node->getType()) {
        case BllAstNode::BllAstNodeType::OPERATOR:{
            std::vector<bool> operands;

            for (const auto& child : node->getChildren())
                operands.emplace_back(calculate(child));

            return operations.at(node->getOp()->getRepresentation())->compute(operands);
        }

        case BllAstNode::BllAstNodeType::VARIABLE:
            return values.at(node->getVariableName());

        case BllAstNode::BllAstNodeType::LITERAL:
            return node->getValue();

        default:
            throw std::runtime_error("Undefined type of node");
    }
}

const std::set<std::string> &BllAstCalculator::getExtractedVariableNames() const {
    return extractedVariableNames;
}

void BllAstCalculator::extendsWithOperation(const BllAstCalculator::Operation *operation) {
    operations.insert(std::make_pair(operation->getOperator()->getRepresentation(), operation));
}

BllAstOperator *BllAstCalculator::Operation::getOperator() const {
    return op;
}

BllAstCalculator::Operation::Operation(BllAstOperator *op, const std::function<bool(const std::vector<bool> &)> &logic)
        : op(op), logic(logic) {}

bool BllAstCalculator::Operation::compute(const std::vector<bool> &operands) const {
    return logic(operands);
}
