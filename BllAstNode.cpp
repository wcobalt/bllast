//
// Created by wcobalt on 2/7/21.
//

#include "BllAstNode.h"
#include "BllAstOperator.h"

using namespace bllast;

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

    char** buffer = new char*[height];

    for (size_t y = 0; y < height; ++y) {
        buffer[y] = new char[width];

        for (size_t x = 0; x < width; ++x)
            buffer[y][x] = FILLER;
    }

    placeNodeOnCanvas(buffer, this, 0, 0, width, height);

    std::string result;

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x)
            result += buffer[y][x];

        delete[] buffer[y];

        result += '\n';
    }

    delete[] buffer;

    return result;
}

void BllAstNode::placeTextOnCanvas(char **canvas, std::string_view text, unsigned width, unsigned height,
                                                unsigned x, unsigned y) const {

    if (y < height) {
        for (size_t p = 0; p < text.length(); ++p) {
            if (x + p < width)
                canvas[y][x + p] = text[p];
        }
    }
}

void BllAstNode::placeNodeOnCanvas(char **canvas, const BllAstNode *node,
                                                unsigned currentDepth, unsigned currentOffset,
                                                unsigned width, unsigned height) const {
    unsigned cellSize = (width / (1ul << currentDepth));
    unsigned y = currentDepth * 2;

    std::string head;

    if (node->type == BllAstNodeType::OPERATOR) {
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

            canvas[nextLevelY][branchX] = (c == 0 ? '/' : '\\');

            placeNodeOnCanvas(canvas, child, currentDepth + 1, n++, width, height);
        }
    } else if (node->type == BllAstNodeType::VARIABLE)
        head = node->getVariableName();
    else if (node->type == BllAstNodeType::LITERAL)
        head = static_cast<char>('0' + node->getValue());
    else
        throw std::runtime_error("Undefined type of a node");

    //it's supposed that NODE_WIDTH / 2 >= max head width
    unsigned x = (currentOffset * cellSize + (cellSize - head.length()) / 2);

    placeTextOnCanvas(canvas, head, width, height, x, y);
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
