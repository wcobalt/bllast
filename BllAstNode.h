//
// Created by wcobalt on 2/7/21.
//

#ifndef INC_1_BLLASTNODE_H
#define INC_1_BLLASTNODE_H

#include <string>
#include <memory>
#include <vector>
#include "BllAstOperator.h"

namespace bllast {
    class BllAstNode {
    public:
        enum class BllAstNodeType {
            OPERATOR, VARIABLE, LITERAL
        };
    private:
        const static inline unsigned NODE_WIDTH = 3;
        const static inline char LEFT_BRANCH = '/', RIGHT_BRANCH = '\\', FILLER = ' ';

        BllAstNodeType type;
        std::string variableName;
        bool value;
        const BllAstOperator* bllOperator;
        std::vector<std::unique_ptr<BllAstNode>> children;

        unsigned findDepth(const BllAstNode* node, unsigned depth) const;

        void placeNodeOnCanvas(char** canvas, const BllAstNode* node, unsigned currentDepth, unsigned currentOffset,
                               unsigned width, unsigned height) const;

        void placeTextOnCanvas(char** canvas, std::string_view text, unsigned width, unsigned height, unsigned x, unsigned y) const;
    public:
        BllAstNode(BllAstNodeType type, std::string variableName, bool value, const BllAstOperator* op,
                   std::vector<std::unique_ptr<BllAstNode>> &children);

        BllAstNodeType getType() const;

        const std::string &getVariableName() const;

        const BllAstOperator* getOp() const;

        bool getValue() const;

        std::vector<BllAstNode*> getChildren();

        std::vector<const BllAstNode*> getChildren() const;

        std::string toAstInStringForm(unsigned width = NODE_WIDTH) const;
    };
}

#endif //INC_1_BLLASTNODE_H