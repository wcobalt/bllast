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
     return node->clone();
}
