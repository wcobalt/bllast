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

std::vector<BllAstNode *> BllAstNode::getChildren() {//todo i don't like the duplication
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

std::string BllAstNode::serialize(const BllAstNode *node) const {
    switch (node->type) {
        case BllAstNodeType::LITERAL:
            return std::string(1, static_cast<char>('0' + node->getValue()));

        case BllAstNodeType::VARIABLE:
            return node->getVariableName();

        case BllAstNodeType::OPERATOR:
            switch (node->getOp()->getArity()) {
                case BllAstOperator::Arity::UNARY:
                    return '(' + node->getOp()->getRepresentation() + serialize(node->getChildren()[0]) + ')';

                case BllAstOperator::Arity::BINARY:
                    return '(' + serialize(node->getChildren()[0]) +
                        node->getOp()->getRepresentation() + serialize(node->getChildren()[1]) + ')';

                default:
                    throw std::runtime_error("Unsupported arity");
            }

        default:
            throw std::runtime_error("Undefined type of node");
    }
}

std::string BllAstNode::toFormulaInStringForm() const {
    return serialize(this);
}

std::string BllAstNode::determineHead(const BllAstNode *node) const {
    switch (node->type) {
        case BllAstNodeType::OPERATOR:
            return node->getOp()->getCode();

        case BllAstNodeType::VARIABLE:
            return node->getVariableName();

        case BllAstNodeType::LITERAL:
            return std::string(1, static_cast<char>('0' + node->getValue()));

        default:
            throw std::runtime_error("Undefined type of a node");
    }
}

void BllAstNode::placeNodeOnCanvas(TextCanvas& canvas, const BllAstNode *node, TextCanvasUtils& textCanvasUtils,
                                                unsigned currentDepth, unsigned currentOffset) const {
    unsigned width = canvas.getWidth();
    unsigned cellSize = (width / (1ul << currentDepth));
    unsigned y = currentDepth * 2;

    std::string head = determineHead(node);

    if (node->type == BllAstNodeType::OPERATOR)
        placeOperatorOnCanvas(canvas, textCanvasUtils, node, currentDepth, currentOffset);

    //it's supposed that NODE_WIDTH / 2 >= max head width
    unsigned x = (currentOffset * cellSize + (cellSize - head.length()) / 2);

    textCanvasUtils.placeText(canvas, x, y, head);
}

void BllAstNode::placeOperatorOnCanvas(TextCanvas& canvas, TextCanvasUtils& textCanvasUtils, const BllAstNode* node,
        unsigned currentDepth, unsigned currentOffset) const {
    unsigned width = canvas.getWidth(), height = canvas.getHeight();
    unsigned cellSize = (width / (1ul << currentDepth));
    unsigned y = currentDepth * 2;

    size_t n = currentOffset * 2;

    unsigned center = (currentOffset * cellSize + cellSize / 2);
    unsigned nextLevelCellSize = (width / (1ul << (currentDepth + 1)));
    unsigned nextLevelY = y + 1;
    unsigned halfNameWidth = node->getOp()->getCode().size() / 2;
    unsigned leftSide = center - halfNameWidth, rightSide = center + halfNameWidth;

    for (size_t c = 0; c < node->getChildren().size(); ++c) {
        const BllAstNode *child = node->getChildren()[c];

        unsigned x = (n * nextLevelCellSize + nextLevelCellSize / 2);
        unsigned branchX = center + (static_cast<long>(x) - center) / 2
                           - ((y != height - 3 && c == 0) ? 1 : 0); //3 is not a magic number, okay?
        unsigned nextX = x + (c == 0 ? 1 : -1) * determineHead(child).size() / 2;

        for (size_t xx = (c == 0 ? nextX : rightSide); xx < (c == 0 ? leftSide : nextX); ++xx) {
            if (xx != branchX)
                canvas.getPointer()[((xx < branchX) ^ (c == 0)) ? y : y + 1][xx] = '_';
        }

        canvas.getPointer()[nextLevelY][branchX] = (c == 0 ? '/' : '\\');

        placeNodeOnCanvas(canvas, child, textCanvasUtils, currentDepth + 1, n++);
    }
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
