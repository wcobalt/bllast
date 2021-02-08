//
// Created by wcobalt on 2/7/21.
//

#ifndef INC_1_BLLASTCONVERTERTOPNF_H
#define INC_1_BLLASTCONVERTERTOPNF_H

#include <memory>
#include "BllAstNode.h"
#include "BllAstCalculator.h"
#include "BllAstTruthTableComputer.h"

namespace bllast {
    class BllAstConverterToPnf {
        BllAstCalculator* bllAstCalculator;
        const BllAstTruthTableComputer* truthTableComputer;

        const BllAstOperator* disjunctionOp, *conjunctionOp, *negationOp;

        std::unique_ptr<BllAstNode> createMultiOperatorNode(std::vector<std::unique_ptr<BllAstNode>>& nodes,
                const BllAstOperator* op) const;

        std::unique_ptr<BllAstNode> createBinaryOperator(const BllAstOperator* op,
                std::unique_ptr<BllAstNode> child1, std::unique_ptr<BllAstNode> child2) const;

        std::unique_ptr<BllAstNode> createVariableNode(std::string variableName, bool isNegated) const;

        std::unique_ptr<BllAstNode> convertToPnf(const BllAstNode* root, bool isPdnf) const;
    public:
        BllAstConverterToPnf(BllAstCalculator *bllAstCalculator, const BllAstTruthTableComputer *truthTableComputer,
                             const BllAstOperator *disjunctionOp, const BllAstOperator *conjunctionOp,
                             const BllAstOperator *negationOp);

        virtual std::unique_ptr<BllAstNode> transformToPcnf(const BllAstNode* root) const;

        virtual std::unique_ptr<BllAstNode> transformToPdnf(const BllAstNode* root) const;
    };
}

#endif //INC_1_BLLASTCONVERTERTOPNF_H
