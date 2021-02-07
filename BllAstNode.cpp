//
// Created by wcobalt on 2/7/21.
//

#include "BllAstNode.h"
#include "BllAstOperator.h"
#include "TextCanvas.h"
#include "TextCanvasUtils.h"

using namespace bllast;
using namespace textcanvas;

BllAstNode::BllAstNodeType BllAstNode::getType() const {
    return type;
}

const std::string& BllAstNode::getVariableName() const {
    return variableName;
}

const BllAstOperator *BllAstNode::getOp() const {
    return bllOperator;
}

std::vector<BllAstNode *> BllAstNode::getChildren() {//fixme i don't like the duplication
    std::vector<BllAstNode*> result;

    for (auto& node_ptr : children)
        result.emplace_back(node_ptr.get());

    return result;
}

std::vector<const BllAstNode *> BllAstNode::getChildren() const {
    std::vector<const BllAstNode*> result;

    for (auto& node_ptr : children)
        result.emplace_back(node_ptr.get());

    return result;
}

std::string BllAstNode::toAstInStringForm(unsigned nodeWidth) const {
    unsigned depth = findDepth(this, 1) - 1;

    size_t width = (1ul << depth) * nodeWidth;
    size_t height = (depth + 1) * 2 - 1;

    std::unique_ptr<TextCanvasUtils> textCanvasUtils = std::make_unique<TextCanvasUtils>();
    TextCanvas canvas = textCanvasUtils->createCanvas(width, height, FILLER);

    placeNodeOnCanvas(canvas, this, *textCanvasUtils, 0, 0);

    std::string result = textCanvasUtils->transform(canvas);

    textCanvasUtils->deleteCanvas(canvas);

    return result;
}

void BllAstNode::placeNodeOnCanvas(TextCanvas& canvas, const BllAstNode *node, TextCanvasUtils& textCanvasUtils,
                                                unsigned currentDepth, unsigned currentOffset) const {
    unsigned width = canvas.getWidth(), height = canvas.getHeight();
    unsigned cellSize = (width / (1ul << currentDepth));
    unsigned y = currentDepth * 2;

    std::string head;

    switch (node->type) {
        case BllAstNodeType::OPERATOR: {
            head = node->getOp()->getCode();

            size_t n = currentOffset * 2;

            unsigned center = (currentOffset * cellSize + cellSize / 2);
            unsigned nextLevelCellSize = (width / (1ul << (currentDepth + 1)));
            unsigned nextLevelY = y + 1;

            for (size_t c = 0; c < node->getChildren().size(); ++c) {
                const BllAstNode *child = node->getChildren()[c];

                unsigned x = (n * nextLevelCellSize + nextLevelCellSize / 2);
                unsigned branchX = center + (static_cast<long>(x) - center) / 2
                                   - ((y != height - 3 && c == 0) ? 1 : 0); //3 is not a magic number, okay?

                canvas.getPointer()[nextLevelY][branchX] = (c == 0 ? '/' : '\\');

                placeNodeOnCanvas(canvas, child, textCanvasUtils, currentDepth + 1, n++);
            }

            break;
        }

        case BllAstNodeType::VARIABLE:
            head = node->getVariableName();

            break;
        case BllAstNodeType::LITERAL:
            head = static_cast<char>('0' + node->getValue());

            break;

        default:
            throw std::runtime_error("Undefined type of a node");
    }

    //it's supposed that NODE_WIDTH / 2 >= max head width
    unsigned x = (currentOffset * cellSize + (cellSize - head.length()) / 2);

    textCanvasUtils.placeText(canvas, x, y, head);
}

unsigned BllAstNode::findDepth(const BllAstNode* node, unsigned depth) const {
    unsigned max = depth;

    for (const auto& child : node->getChildren()) {
        unsigned result = findDepth(child, depth + 1);

        if (result > max)
            max = result;
    }

    return max;
}

bool BllAstNode::getValue() const {
    return value;
}

BllAstNode::BllAstNode(BllAstNode::BllAstNodeType type, std::string variableName, bool value,
                                    const BllAstOperator *op,
                                    std::vector<std::unique_ptr<BllAstNode>> &children) : type(type), value(value),
                                    variableName(std::move(variableName)), bllOperator(op) {
    for (auto& node : children)
        this->children.emplace_back(std::move(node));
}
