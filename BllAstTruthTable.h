//
// Created by wcobalt on 2/7/21.
//

#ifndef INC_1_BLLASTTRUTHTABLE_H
#define INC_1_BLLASTTRUTHTABLE_H

#include <set>
#include <string>
#include <vector>
#include "TextCanvas.h"
#include "TextCanvasUtils.h"

using namespace textcanvas;

namespace bllast {
    class BllAstTruthTable {
        static inline const unsigned VARIABLE_MARGIN = 1;
        static inline const char VERTICAL_BORDER = '|';
        static inline const char HORIZONTAL_BORDER = '-';
        static inline const char CROSS_BORDER = '+';
        static inline const char FILLER = ' ';
        static inline const char* RESULT_TITLE = "Result";

        std::set<std::string> variableNames;
        std::vector<bool> truthTable;

        void placeVerticalCrossBorders(TextCanvas &canvas, unsigned x) const;

        void placeHeader(TextCanvas& canvas, TextCanvasUtils& textCanvasUtils, unsigned varCellSize) const;

        void placeBody(TextCanvas& canvas, TextCanvasUtils& textCanvasUtils, unsigned varCellSize) const;

        void placeBorders(TextCanvas &canvas, unsigned varCellSize) const;
    public:
        BllAstTruthTable(std::set<std::string> variableNames, std::vector<bool> truthTable);

        const std::set<std::string>& getVariables() const;

        bool getValue(uint64_t number) const;

        virtual std::string toString() const;
    };
}

#endif //INC_1_BLLASTTRUTHTABLE_H
