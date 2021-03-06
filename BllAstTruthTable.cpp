//
// Created by Артём Драпун (wcobalt), 821702 on 2/7/21.
//

#include "BllAstTruthTable.h"

#include <utility>
#include <cstring>
#include "textcanvas/TextCanvas.h"
#include "textcanvas/TextCanvasUtils.h"

using namespace textcanvas;
using namespace bllast;

BllAstTruthTable::BllAstTruthTable(std::set<std::string> variableNames, std::vector<bool> truthTable, bool alwaysEquals)
        : variableNames(std::move(variableNames)),
          truthTable(std::move(truthTable)), alwaysEquals(alwaysEquals) {}

const std::set<std::string> &BllAstTruthTable::getVariableNames() const {
    return variableNames;
}

bool BllAstTruthTable::getValue(uint64_t number) const {
    return truthTable[number];
}

std::vector<bool> BllAstTruthTable::computeParametersSet(uint64_t number) const {
    size_t variablesCount = variableNames.size();
    std::vector<bool> parametersSet(variablesCount);

    for (size_t v = 0; v < variablesCount; ++v) {
        bool value = (number >> (variablesCount - 1 - v)) & 1u;

        parametersSet[v] = value;
    }

    return parametersSet;
}

std::string BllAstTruthTable::toString() const {
    unsigned variablesCount = variableNames.size();

    if (variablesCount > 0) {
        size_t maximalVariableNameLength = 0;

        for (auto &name : variableNames) {
            if (name.size() > maximalVariableNameLength) {
                maximalVariableNameLength = name.size();
            }
        }

        unsigned variableCellSize = maximalVariableNameLength + VARIABLE_MARGIN * 2;
        unsigned width = 2 + (variableCellSize + 1) * variablesCount + (VARIABLE_MARGIN * 2 + strlen(RESULT_TITLE));
        unsigned height = 4 + truthTable.size();

        TextCanvasUtils textCanvasUtils;
        TextCanvas canvas = textCanvasUtils.createCanvas(width, height, FILLER);

        placeHeader(canvas, textCanvasUtils, variableCellSize);
        placeBody(canvas, textCanvasUtils, variableCellSize);
        placeBorders(canvas, variableCellSize);

        std::string result = textCanvasUtils.transform(canvas);

        textCanvasUtils.deleteCanvas(canvas);

        return result;
    } else
        return "Always equals " + std::string(1, static_cast<char>('0' + alwaysEquals));
}

void BllAstTruthTable::placeHeader(TextCanvas &canvas, TextCanvasUtils &textCanvasUtils,
                                           unsigned varCellSize) const {
    unsigned borderedCellSize = varCellSize + 1;

    auto it = variableNames.begin();
    for (size_t cell = 0; cell < variableNames.size(); ++cell, ++it) {
        unsigned x = (cell * borderedCellSize) + borderedCellSize / 2 - (*it).size() / 2;

        textCanvasUtils.placeText(canvas, x, 1, *it);
    }

    textCanvasUtils.placeText(canvas, borderedCellSize * variableNames.size() + VARIABLE_MARGIN + 1, 1, RESULT_TITLE);
}

void BllAstTruthTable::placeBody(TextCanvas &canvas, TextCanvasUtils &textCanvasUtils, unsigned varCellSize) const {
    unsigned borderedCellSize = varCellSize + 1;

    for (size_t c = 0; c < truthTable.size(); ++c) {
        std::vector<bool> values = computeParametersSet(c);

        for (size_t v = 0; v < variableNames.size(); ++v) {
            unsigned valueX = borderedCellSize * v + borderedCellSize / 2;

            canvas.getPointer()[c + 3][valueX] = static_cast<char>('0' + values[v]);
        }

        unsigned resultX = canvas.getWidth() - (canvas.getWidth() - borderedCellSize * variableNames.size()) / 2;

        canvas.getPointer()[c + 3][resultX] = static_cast<char>('0' + truthTable[c]);
    }
}

void BllAstTruthTable::placeBorders(TextCanvas &canvas, unsigned varCellSize) const {
    unsigned borderedCellSize = varCellSize + 1;

    for (size_t x = 0; x < canvas.getWidth(); ++x) {
        canvas.getPointer()[0][x] = HORIZONTAL_BORDER;
        canvas.getPointer()[2][x] = HORIZONTAL_BORDER; //2 is not a magic number
        canvas.getPointer()[canvas.getHeight() - 1][x] = HORIZONTAL_BORDER;
    }

    unsigned resultBarX = borderedCellSize * variableNames.size();

    for (size_t y = 0; y < canvas.getHeight(); ++y) {
        canvas.getPointer()[y][0] = VERTICAL_BORDER;
        canvas.getPointer()[y][resultBarX] = VERTICAL_BORDER;
        canvas.getPointer()[y][canvas.getWidth() - 1] = VERTICAL_BORDER;
    }

    for (size_t cell = 0; cell < variableNames.size(); ++cell) {
        unsigned cellRightBorderX = (cell + 1) * borderedCellSize;

        canvas.getPointer()[0][cellRightBorderX] = CROSS_BORDER;
        canvas.getPointer()[1][cellRightBorderX] = VERTICAL_BORDER;
        canvas.getPointer()[2][cellRightBorderX] = CROSS_BORDER;
    }

    placeVerticalCrossBorders(canvas, 0);
    placeVerticalCrossBorders(canvas, resultBarX);
    placeVerticalCrossBorders(canvas, canvas.getWidth() - 1);
}

void BllAstTruthTable::placeVerticalCrossBorders(TextCanvas &canvas, unsigned x) const {
    canvas.getPointer()[0][x] = CROSS_BORDER;
    canvas.getPointer()[2][x] = CROSS_BORDER;
    canvas.getPointer()[canvas.getHeight() - 1][x] = CROSS_BORDER;
}

uint64_t BllAstTruthTable::getSize() const {
    return truthTable.size();
}
