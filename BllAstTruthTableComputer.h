//
// Created by Артём Драпун (wcobalt), 821702 on 2/7/21.
//

#ifndef INC_1_BLLASTTRUTHTABLECOMPUTER_H
#define INC_1_BLLASTTRUTHTABLECOMPUTER_H

#include <vector>
#include "BllAstNode.h"
#include "BllAstCalculator.h"
#include "BllAstTruthTable.h"

namespace bllast {
    class BllAstTruthTableComputer {
    public:
        const static inline unsigned MAXIMAL_VARIABLE_COUNT = 32;

        std::unique_ptr<BllAstTruthTable> computeTruthTable(BllAstCalculator* bllAstCalculator, const BllAstNode* formula) const;
    };
}


#endif //INC_1_BLLASTTRUTHTABLECOMPUTER_H
