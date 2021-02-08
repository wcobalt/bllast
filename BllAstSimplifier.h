//
// Created by wcobalt on 2/8/21.
//

#ifndef INC_1_BLLASTSIMPLIFIER_H
#define INC_1_BLLASTSIMPLIFIER_H

#include <memory>
#include <cstdint>
#include <stdint-gcc.h>
#include "BllAstNode.h"
#include "BllAstCalculator.h"

namespace bllast {
    class BllAstSimplifier {
    private:
        BllAstCalculator* bllAstCalculator;

        std::string conjunctionOpCode, disjunctionOpCode, negationOpCode;
    public:
        using mask_type = uint64_t;

        const static inline mask_type FULL_LITERAL_CALCULATION = 1u << 0u;
        const static inline mask_type REDUNDANT_DISJUNCTION_AND_CONJUNCTION = 1u << 1u;
        const static inline mask_type APRIORI_KNOWN_DISJUNCTION_AND_CONJUNCTION = 1u << 2u;
        const static inline mask_type MULTIPLE_NEGATIONS = 1u << 3u;

        BllAstSimplifier(BllAstCalculator *bllAstCalculator, std::string conjunctionOpCode,
                         std::string disjunctionOpCode, std::string negationOpCode);

        virtual std::unique_ptr<BllAstNode> simplify(const BllAstNode *node, uint64_t optimizationsMask) const;
    };
}

#endif //INC_1_BLLASTSIMPLIFIER_H
