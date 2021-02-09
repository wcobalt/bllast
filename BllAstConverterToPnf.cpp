//
// Created by wcobalt on 2/7/21.
//

#include "BllAstConverterToPnf.h"

using namespace bllast;

BllAstConverterToPnf::BllAstConverterToPnf(BllAstCalculator *bllAstCalculator,
                                           const BllAstTruthTableComputer *truthTableComputer, const BllAstOperator *disjunctionOp,
                                           const BllAstOperator *conjunctionOp, const BllAstOperator *negationOp):
                                           bllAstCalculator(bllAstCalculator), truthTableComputer(truthTableComputer),
                                           disjunctionOp(disjunctionOp), conjunctionOp(conjunctionOp), negationOp(negationOp) {}

std::unique_ptr<BllAstNode> BllAstConverterToPnf::transformToPcnf(const BllAstNode *root) const {
    return convertToPnf(root, false);
}

std::unique_ptr<BllAstNode> BllAstConverterToPnf::transformToPdnf(const BllAstNode *root) const {
    return convertToPnf(root, true);
}

std::unique_ptr<BllAstNode> BllAstConverterToPnf::convertToPnf(const BllAstNode *root, bool isPdnf) const {
    //isPdnf: true - pdnf, false - pcnf
    std::unique_ptr<BllAstTruthTable> truthTable = truthTableComputer->computeTruthTable(bllAstCalculator, root);
    std::vector<std::unique_ptr<BllAstNode>> secondLayerRoots;

    size_t ttsize = truthTable->getSize();
    for (size_t c = 0; c < ttsize; ++c) {
        if (truthTable->getValue(c) == isPdnf) {
            std::vector<std::unique_ptr<BllAstNode>> vars;

            std::vector<bool> values = truthTable->computeParametersSet(c);
            size_t variablesCount = truthTable->getVariableNames().size();
            auto it = truthTable->getVariableNames().begin();

            for (size_t v = 0; v < variablesCount; ++v, ++it)
                vars.emplace_back(std::move(createVariableNode(*it, values[v] ^ isPdnf)));

            secondLayerRoots.emplace_back(std::move(
                    createMultiOperatorNode(vars, isPdnf ? conjunctionOp : disjunctionOp)));
        }
    }

    return createMultiOperatorNode(secondLayerRoots, isPdnf ? disjunctionOp : conjunctionOp);
}

std::unique_ptr<BllAstNode>
BllAstConverterToPnf::createMultiOperatorNode(std::vector<std::unique_ptr<BllAstNode>>& nodes,
                                              const BllAstOperator *op) const {
    if (!nodes.empty()) {
        size_t operatorsCount = nodes.size() - 1;

        std::unique_ptr<BllAstNode> currentNode = std::move(nodes[operatorsCount]);
        nodes.erase(nodes.end() - 1);

        for (size_t o = 0; o < operatorsCount; ++o) {
            std::vector<std::unique_ptr<BllAstNode>> children;
            children.emplace_back(std::move(nodes[nodes.size() - 1]));
            children.emplace_back(std::move(currentNode));

            nodes.erase(nodes.end() - 1);

            currentNode = std::make_unique<BllAstNode>(BllAstNode::Type::OPERATOR,
                                                       "", false, op, children);
        }

        return currentNode;
    } else
        return nullptr;
}

std::unique_ptr<BllAstNode>
BllAstConverterToPnf::createBinaryOperator(const BllAstOperator *op, std::unique_ptr<BllAstNode> child1,
                                           std::unique_ptr<BllAstNode> child2) const {
    std::vector<std::unique_ptr<BllAstNode>> children;
    children.emplace_back(std::move(child1));
    children.emplace_back(std::move(child2));

    return std::make_unique<BllAstNode>(BllAstNode::Type::OPERATOR,
            "", false, op, children);
}

std::unique_ptr<BllAstNode> BllAstConverterToPnf::createVariableNode(std::string variableName, bool isNegated) const {
    std::vector<std::unique_ptr<BllAstNode>> stub{};
    std::unique_ptr<BllAstNode> variableNode = std::make_unique<BllAstNode>(BllAstNode::Type::VARIABLE,
            std::move(variableName), false, nullptr, stub);

    if (isNegated) {
        std::vector<std::unique_ptr<BllAstNode>> children;
        children.emplace_back(std::move(variableNode));

        return std::make_unique<BllAstNode>(BllAstNode::Type::OPERATOR,
                "", false, negationOp, children);
    } else
        return variableNode;
}
