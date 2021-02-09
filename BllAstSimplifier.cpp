//
// Created by wcobalt on 2/8/21.
//

#include <utility>
#include "BllAstSimplifier.h"

using namespace bllast;

BllAstSimplifier::BllAstSimplifier(BllAstCalculator *bllAstCalculator,
                                   std::string conjunctionOpCode, std::string disjunctionOpCode,
                                   std::string negationOpCode) : bllAstCalculator(bllAstCalculator),
                                                                 conjunctionOpCode(std::move(conjunctionOpCode)),
                                                                 disjunctionOpCode(std::move(disjunctionOpCode)),
                                                                 negationOpCode(std::move(negationOpCode)) {}

std::unique_ptr<BllAstNode> BllAstSimplifier::simplify(const BllAstNode *node, mask_type optimizationsMask) const {
    std::unique_ptr<BllAstNode> clone = node->clone();

    OptimizationFeatures optimizationFeatures(optimizationsMask);

    std::unique_ptr<BllAstNode> ret = optimize(clone.get(), optimizationFeatures);

    return std::move(ret ? ret : clone);
}

std::unique_ptr<BllAstNode>
BllAstSimplifier::optimize(BllAstNode *node, OptimizationFeatures mask) const {
    if (node->getType() == BllAstNode::Type::OPERATOR) {
        bool repeat;
        std::vector<bool> simplifiedChildren(node->getChildren().size(), false);

        do {
            repeat = false;

            auto directOptimizationResult = optimizeDirectly(node, mask);

            if (directOptimizationResult)
                return directOptimizationResult;

            std::vector<BllAstNode*> children = node->getChildren();

            for (size_t c = 0; c < children.size(); ++c) {
                if (!simplifiedChildren[c]) {
                    BllAstNode *child = children[c];

                    std::unique_ptr<BllAstNode> ret = optimize(child, mask);

                    if (ret) {
                        repeat = true;

                        node->replaceChild(child, std::move(ret));
                    }
                }
            }
        } while (repeat);

        return nullptr;
    } else
        return nullptr;
}

//node can be an operator only
std::unique_ptr<BllAstNode>
BllAstSimplifier::optimizeDirectly(BllAstNode *node, BllAstSimplifier::OptimizationFeatures mask) const {
    if (mask.doCalculateFullLiterals && isFullLiteralOperation(node))
        return calculate(node);

    auto ret = optimizeJunction(node, disjunctionOpCode, mask.doCalculateAprioriKnownOperations,
                                mask.doEliminateRedundantOperations, true);

    if (ret)
        return ret;

    ret = optimizeJunction(node, conjunctionOpCode, mask.doCalculateAprioriKnownOperations,
                           mask.doEliminateRedundantOperations, false);

    if (ret)
        return ret;

    if (mask.doEliminateMultipleNegations && node->getOp()->getCode() == negationOpCode) {
        BllAstNode* probableSecondNegation = node->getChildren()[0];

        if (probableSecondNegation->getType() == BllAstNode::Type::OPERATOR &&
            probableSecondNegation->getOp()->getCode() == negationOpCode)
            return probableSecondNegation->extractChild(probableSecondNegation->getChildren()[0]);
    }

    return nullptr;
}

//specificity: value with at least one operand of which the operation returns the specificity
//e.g.: disj: 1, conj: 0
std::unique_ptr<BllAstNode>
BllAstSimplifier::optimizeJunction(BllAstNode *node, std::string_view opCode, bool optimizeAprioriKnown,
                                   bool optimizeRedundant, bool specificity) const {
    if (node->getOp()->getCode() == opCode) {
        auto literalChild = findChildOfType(node, true);

        if (literalChild) {
            if (!(literalChild->getValue() ^ specificity)) {
                if (optimizeAprioriKnown)
                    return makeLiteral(specificity);
            } else if (optimizeRedundant)
                return std::move(node->extractChild(findChildOfType(node, false)));
        }
    }

    return nullptr;
}

std::unique_ptr<BllAstNode> BllAstSimplifier::makeLiteral(bool value) const {
    std::vector<std::unique_ptr<BllAstNode>> children;

    return std::make_unique<BllAstNode>(BllAstNode::Type::LITERAL, "", value, nullptr, children);
}

BllAstNode *BllAstSimplifier::findChildOfType(BllAstNode *parent, bool isLiteral) const {
    for (auto child : parent->getChildren()) {
        if (!((child->getType() == BllAstNode::Type::LITERAL) ^ isLiteral))
            return child;
    }

    return nullptr;
}

bool BllAstSimplifier::isFullLiteralOperation(const BllAstNode *node) const {
    bool result = true;

    for (auto child : node->getChildren()) {
        if (child->getType() != BllAstNode::Type::LITERAL) {
            result = false;

            break;
        }
    }

    return result;
}

//it's supposed that isFullLiteralOperation(node) is true
std::unique_ptr<BllAstNode> BllAstSimplifier::calculate(const BllAstNode *node) const {
    bllAstCalculator->putBllAst(node);

    return makeLiteral(bllAstCalculator->calculate());
}

BllAstSimplifier::OptimizationFeatures::OptimizationFeatures(uint64_t mask) {
    doCalculateFullLiterals = (mask & FULL_LITERAL_CALCULATION);
    doEliminateRedundantOperations = (mask & REDUNDANT_OPERATIONS_ELIMINATION);
    doCalculateAprioriKnownOperations = (mask & APRIORI_KNOWN_OPERATIONS_CALCULATION);
    doEliminateMultipleNegations = (mask & MULTIPLE_NEGATIONS_ELIMINATION);
}
