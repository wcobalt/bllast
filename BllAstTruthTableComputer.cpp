//
// Created by Артём Драпун (wcobalt), 821702 on 2/7/21.
//

#include <iostream>
#include "BllAstTruthTableComputer.h"

using namespace bllast;

std::unique_ptr<BllAstTruthTable> bllast::BllAstTruthTableComputer::computeTruthTable(BllAstCalculator* bllAstCalculator,
                                                                                      const BllAstNode *formula) const {
    bllAstCalculator->putBllAst(formula);

    const std::set<std::string>& variables = bllAstCalculator->getExtractedVariableNames();
    unsigned variablesCount = variables.size();

    if (variablesCount > 0) {
        if (variablesCount <= MAXIMAL_VARIABLE_COUNT) {
            uint64_t combinationsCount = 1;
            combinationsCount <<= variablesCount;

            std::vector<bool> truthTable(combinationsCount);

            for (uint64_t c = 0; c < combinationsCount; ++c) {
                auto it = variables.end();
                --it;

                for (unsigned v = 0; v < variablesCount; ++v, --it) {
                    bool value = (c >> v) & 1u;

                    bllAstCalculator->setVariableValue(*it, value);
                }

                truthTable[c] = bllAstCalculator->calculate();
            }

            return std::make_unique<BllAstTruthTable>(variables, truthTable);
        } else
            throw std::invalid_argument("The formula is too big for me");
    } else
        return std::make_unique<BllAstTruthTable>(variables, std::vector<bool>{}, bllAstCalculator->calculate());
}
