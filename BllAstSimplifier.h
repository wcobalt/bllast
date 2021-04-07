//
// Created by Артём Драпун (wcobalt), 821702 on 2/8/21.
//

#ifndef INC_1_BLLASTSIMPLIFIER_H
#define INC_1_BLLASTSIMPLIFIER_H

#include <memory>
#include <cstdint>
#include "BllAstNode.h"
#include "BllAstCalculator.h"

namespace bllast {
    class BllAstSimplifier {
    public:
        using mask_type = uint64_t;
    private:
        struct OptimizationFeatures {
            bool doCalculateFullLiterals{};
            bool doEliminateRedundantOperations{};
            bool doCalculateAprioriKnownOperations{};
            bool doEliminateMultipleNegations{};
            bool doUseControversyLaw{};

            explicit OptimizationFeatures(mask_type mask);
        };

        BllAstCalculator* bllAstCalculator;

        std::string conjunctionOpCode, disjunctionOpCode, negationOpCode;

        std::unique_ptr<BllAstNode> makeLiteral(bool value) const;

        BllAstNode *findChildOfType(BllAstNode *parent, bool isLiteral) const;

        bool isFullLiteralOperation(const BllAstNode* node) const;

        std::unique_ptr<BllAstNode> calculate(const BllAstNode* node) const;

        virtual std::unique_ptr<BllAstNode>
        optimizeJunction(BllAstNode *node, std::string_view opCode, bool optimizeAprioriKnown,
                         bool optimizeRedundant, bool optimizeControversyLaw, bool specificity) const;

        virtual std::unique_ptr<BllAstNode> optimizeDirectly(BllAstNode *node, OptimizationFeatures mask) const;

        virtual std::unique_ptr<BllAstNode> optimize(BllAstNode *node, OptimizationFeatures mask) const;
    public:
        const static inline mask_type FULL_LITERAL_CALCULATION = 1u << 0u;
        const static inline mask_type REDUNDANT_OPERATIONS_ELIMINATION = 1u << 1u;
        const static inline mask_type APRIORI_KNOWN_OPERATIONS_CALCULATION = 1u << 2u;
        const static inline mask_type MULTIPLE_NEGATIONS_ELIMINATION = 1u << 3u;
        const static inline mask_type CONTROVERSY_LAW = 1u << 4u;

        BllAstSimplifier(BllAstCalculator *bllAstCalculator, std::string conjunctionOpCode,
                         std::string disjunctionOpCode, std::string negationOpCode);

        virtual std::unique_ptr<BllAstNode> simplify(const BllAstNode *node, mask_type optimizationsMask) const;
    };
}

#endif //INC_1_BLLASTSIMPLIFIER_H
